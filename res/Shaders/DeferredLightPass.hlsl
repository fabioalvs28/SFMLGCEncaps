#include "Utils.hlsl"

#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_SPOT 1
#define LIGHT_TYPE_POINT 2

#define MAX_MATERIALS 100
#define MAX_LIGHTS 100


struct Light
{
    float3 cbPerLight_position;
    float3 cbPerLight_direction;
    float3 cbPerLight_color;
    float cbPerLight_spotAngle;
    int cbPerLight_lightType;
    float cbPerLight_lightIntensity;
};

struct SBMaterialDSL
{
    float4 ambientLightColor;
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float shininess;
    float materialId;
};

Texture2D texture_pixelIdMapping : register(t0);
Texture2D g_buffer_albedo : register(t1);
Texture2D g_buffer_worldPosition : register(t2);
//Texture2D g_buffer_depth : register(t2);
Texture2D g_buffer_normal : register(t3);

SamplerState g_sampler : register(s0);

cbuffer cbPerCamera : register(b0)
{
    float4x4 gView;
    float4x4 gProj;
};

cbuffer cbPerLights : register(b1)
{
    Light lights[MAX_LIGHTS];
};

cbuffer MaterialBuffer : register(b2) 
{
    SBMaterialDSL materials[MAX_MATERIALS];
};


SBMaterialDSL GetMaterialById(float materialId)
{
    int id = int(materialId);
    return materials[id-1];
}

struct VSOutput
{
    float4 PosH : SV_POSITION;
    float2 UV : TEXCOORD;
};

VSOutput VS(float3 posL : POSITION, float2 uv : TEXCOORD)
{
    VSOutput output;
    output.PosH = float4(posL, 1.0f);
    output.UV = uv;
    return output;
}

// 3D

float4 PS(VSOutput pin) : SV_Target
{
    //return float4(0.678431392f, 0.847058892f, 0.901960850f, 1.0f);
    
    
    float4 albedo = g_buffer_albedo.Sample(g_sampler, pin.UV);
    float4 worldPosSample = g_buffer_worldPosition.Sample(g_sampler, pin.UV);
    //float depth = g_buffer_depth.Sample(g_sampler, pin.UV).r;
    
    float4 remappedNormal = g_buffer_normal.Sample(g_sampler, pin.UV);
    float4 pixelIdMapping = texture_pixelIdMapping.Sample(g_sampler, pin.UV);
    
    float objectId = pixelIdMapping.r * 255.0f;
    float materialId = pixelIdMapping.g * 255.0f;
    
    if (albedo.r == 0.678431392f || albedo.g == 0.847058892f || albedo.b == 0.901960850f)
    {
        return float4(0.678431392f, 0.847058892f, 0.901960850f, 1.0f);
    }
    
    SBMaterialDSL currentMaterial = GetMaterialById(materialId);


    float2 screenPos = pin.UV; // Assumed to be normalized; otherwise, adjust as needed
    float3 worldPos;

    //// Convert float2 to float3 and initialize z-component to 0
    //float3 position = normalize(float3(screenPos, 0.0f)) * 0.0f;
    //position += gView[3].xyz;

    // Assign position to worldPos directly
    //worldPos.xy = float2(-6.0f, 0.0f);
    worldPos.xy = worldPosSample.xy;
    worldPos.z = worldPosSample.z;

        // Reconstruct position from depth
    
   
    // Remap des normales de [0, 1] à [-1, 1]
    float3 normal = remappedNormal.xyz * 2.0 - 1.0;

    float3x3 invViewMatrix = (float3x3) transpose(gView); // Transpose of view matrix
    float3 cameraPosition = -mul(invViewMatrix, gView[3].xyz); // Camera position in world space
   

    float3 viewDirection = normalize(cameraPosition - worldPos); // View direction

    float4 finalColor = ComputeAmbient(currentMaterial.ambientLightColor, currentMaterial.ambient, albedo);

    float3 diffuseColor = float3(0.0f, 0.0f, 0.0f);
    float3 specularColor = float3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 100; ++i)
    {
        Light currentLight = lights[i];

        if (currentLight.cbPerLight_lightType == LIGHT_TYPE_DIRECTIONAL)
        {
            float3 lightColorDirectional = currentLight.cbPerLight_color * currentLight.cbPerLight_lightIntensity;
            float3 lightDirectionDirectional = -normalize(currentLight.cbPerLight_direction); // Invert direction for directional light

            float diffuseIntensity = dot(normal, lightDirectionDirectional);
            diffuseIntensity = saturate(diffuseIntensity);

            diffuseColor += ComputeDiffuse(lightDirectionDirectional, normal, lightColorDirectional, currentMaterial.diffuse) * diffuseIntensity;
            specularColor += ComputePhongSpecular(lightDirectionDirectional, normal, viewDirection, lightColorDirectional, currentMaterial.specular, currentMaterial.shininess);
        }
        else if (currentLight.cbPerLight_lightType == LIGHT_TYPE_SPOT)
        {
            // Spot light calculations
            float3 lightColorSpot = currentLight.cbPerLight_color * currentLight.cbPerLight_lightIntensity;
            float3 lightPositionSpot = currentLight.cbPerLight_position;
            float3 lightDirectionSpot = normalize(currentLight.cbPerLight_direction);
            float spotAngle = radians(currentLight.cbPerLight_spotAngle);

            float spotIntensity = ComputeSpotIntensity(lightPositionSpot, lightDirectionSpot, worldPos, spotAngle);

            diffuseColor += ComputeDiffuse(lightDirectionSpot, normal, lightColorSpot, currentMaterial.diffuse) * spotIntensity;
            specularColor += ComputePhongSpecular(lightDirectionSpot, normal, viewDirection, lightColorSpot, currentMaterial.specular, currentMaterial.shininess) * spotIntensity;
        }
        else if (currentLight.cbPerLight_lightType == LIGHT_TYPE_POINT)
        {
            float3 lightColorPoint = currentLight.cbPerLight_color;
            float3 lightPositionPoint = currentLight.cbPerLight_position;

            float pointLightIntensity = ComputePointLightIntensity(lightPositionPoint, worldPos, currentLight.cbPerLight_lightIntensity);

            float3 lightDirectionPoint = normalize(lightPositionPoint - worldPos);

            diffuseColor += ComputeDiffuse(lightDirectionPoint, normal, lightColorPoint, currentMaterial.diffuse) * pointLightIntensity;
            specularColor += ComputePhongSpecular(lightDirectionPoint, normal, viewDirection, lightColorPoint, currentMaterial.specular, currentMaterial.shininess) * pointLightIntensity;
        }
    }

    finalColor.rgb += diffuseColor * albedo.rgb;
    finalColor.rgb += specularColor * albedo.rgb;
    finalColor.a = albedo.a;
   
    return finalColor;
}





// 2D





//float4 PS(VSOutput pin) : SV_Target
//{
//    // Échantillonnage des textures
//    float4 albedo = g_buffer_albedo.Sample(g_sampler, pin.UV);
//    float4 remappedNormal = g_buffer_normal.Sample(g_sampler, pin.UV);
    
//    // Ne pas calculer la lumière sur la couleur de nettoyage de RTV
//    if (albedo.r == 0.678431392f || albedo.g == 0.847058892f || albedo.b == 0.901960850f)
//    {
//        return float4(0.678431392f, 0.847058892f, 0.901960850f, 1.0f);
//    }

//    // Remap des normales
//    float3 normal = remappedNormal.xyz * 2.0f - 1.0f;
    
//    // Obtenez l'ID du matériau à partir de la texture pixelIdMapping
//    float objectId = texture_pixelIdMapping.Sample(g_sampler, pin.UV).r * 255.0f;
//    float materialId = texture_pixelIdMapping.Sample(g_sampler, pin.UV).g * 255.0f;

//    // Récupérez le matériau courant
//    SBMaterialDSL currentMaterial = GetMaterialById(materialId);

//    // Calcul de la couleur ambiante
//    float4 finalColor = ComputeAmbient(currentMaterial.ambientLightColor, currentMaterial.ambient, albedo);

//    // Calcul des lumières
//    for (int i = 0; i < 10; ++i)
//    {
//        Light currentLight = lights[i];

//        // Lumière directionnelle
//        if (currentLight.cbPerLight_lightType == LIGHT_TYPE_DIRECTIONAL)
//        {
//            float3 lightColorDirectional = currentLight.cbPerLight_color * currentLight.cbPerLight_lightIntensity;
//            float3 lightDirectionDirectional = -normalize(currentLight.cbPerLight_direction); // Direction inversée

//            float diffuseIntensity = dot(normal, lightDirectionDirectional);
//            diffuseIntensity = saturate(diffuseIntensity);

//            finalColor.rgb += ComputeDiffuse(lightDirectionDirectional, normal, lightColorDirectional, currentMaterial.diffuse) * diffuseIntensity;
//            finalColor.rgb += ComputePhongSpecular(lightDirectionDirectional, normal, float3(0, 0, -1), lightColorDirectional, currentMaterial.specular, currentMaterial.shininess); // Direction de vue fixe
//        }
//        // Lumière spot
//        else if (currentLight.cbPerLight_lightType == LIGHT_TYPE_SPOT)
//        {
//            float3 lightColorSpot = currentLight.cbPerLight_color * currentLight.cbPerLight_lightIntensity;
//            float3 lightPositionSpot = currentLight.cbPerLight_position;
//            float3 lightDirectionSpot = normalize(currentLight.cbPerLight_direction);
//            float spotAngle = radians(currentLight.cbPerLight_spotAngle); // Corrigez ici

//            float spotIntensity = ComputeSpotIntensity(lightPositionSpot, lightDirectionSpot, float3(pin.UV, 0), spotAngle);

//            finalColor.rgb += ComputeDiffuse(lightDirectionSpot, normal, lightColorSpot, currentMaterial.diffuse) * spotIntensity;
//            finalColor.rgb += ComputePhongSpecular(lightDirectionSpot, normal, float3(0, 0, -1), lightColorSpot, currentMaterial.specular, currentMaterial.shininess) * spotIntensity;
//        }
//        // Lumière ponctuelle
//        else if (currentLight.cbPerLight_lightType == LIGHT_TYPE_POINT)
//        {
//            float3 lightColorPoint = currentLight.cbPerLight_color;
//            float3 lightPositionPoint = currentLight.cbPerLight_position;

//            float pointLightIntensity = ComputePointLightIntensity(lightPositionPoint, float3(pin.UV, 0), currentLight.cbPerLight_lightIntensity);

//            float3 lightDirectionPoint = normalize(lightPositionPoint - float3(pin.UV, 0));
//            float3 diffuseComponent = ComputeDiffuse(lightDirectionPoint, normal, lightColorPoint, currentMaterial.diffuse) * pointLightIntensity;

//            float3 viewDirection2D = float3(0, 0, -1); // Direction de vue fixe pour 2D
//            float3 specularComponent = ComputePhongSpecular(lightDirectionPoint, normal, viewDirection2D, lightColorPoint, currentMaterial.specular, currentMaterial.shininess) * pointLightIntensity;

//            finalColor.rgb += diffuseComponent;
//            finalColor.rgb += specularComponent;
//        }
//    }

//    finalColor.rgb *= albedo.rgb;
//    finalColor.a = albedo.a;

//    return finalColor;
//}