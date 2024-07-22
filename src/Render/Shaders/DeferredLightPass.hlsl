#include "Utils.hlsl"

#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_SPOT 1
#define LIGHT_TYPE_POINT 2

#define MAX_MATERIALS 100

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

static float4 ambientLightColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
static float4 ambient = float4(0.2f, 0.2f, 0.2f, 1.0f);
static float4 diffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);
static float4 specular = float4(0.8f, 0.8f, 0.8f, 1.0f);
static float shininess = 128.0f;

Texture2D texture_pixelIdMapping : register(t0);
Texture2D g_buffer_albedo : register(t1);
Texture2D g_buffer_worldPosition : register(t2);
Texture2D g_buffer_normal : register(t3);

SamplerState g_sampler : register(s0);

cbuffer cbPerCamera : register(b0)
{
    float4x4 gView;
    float4x4 gProj;
};

cbuffer cbPerLights : register(b1)
{
    Light lights[10];
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

float4 PS(VSOutput pin) : SV_Target
{
    float4 albedo = g_buffer_albedo.Sample(g_sampler, pin.UV);
    float4 worldPos = g_buffer_worldPosition.Sample(g_sampler, pin.UV);
    float4 remappedNormal = g_buffer_normal.Sample(g_sampler, pin.UV);
    float4 pixelIdMapping = texture_pixelIdMapping.Sample(g_sampler, pin.UV);
    
    float objectId = pixelIdMapping.r * 255.0f;
    float materialId = pixelIdMapping.g * 255.0f;
    
    if (albedo.r == 0.678431392f || albedo.g == 0.847058892f || albedo.b == 0.901960850f)
    {
        return float4(0.678431392f, 0.847058892f, 0.901960850f, 1.0f);
    }
    
    SBMaterialDSL currentMaterial = GetMaterialById(materialId);
    //if (materialId == 200.0f)
    //{
    //    // blue
    //    return float4(0.0f, 1.0f, 1.0f, 1.0f);
    //}
    //else
    //{
    //    // red
    //    return float4(1.0, 0.0, 0.0, 1.0);
    //}
    
  
   
    // Remap des normales de [0, 1] à [-1, 1]
    float3 normal = remappedNormal.xyz * 2.0 - 1.0;

    float3x3 invViewMatrix = (float3x3) transpose(gView); // Transpose of view matrix
    float3 cameraPosition = -mul(invViewMatrix, gView[3].xyz); // Camera position in world space

    float3 viewDirection = normalize(cameraPosition - worldPos.xyz); // View direction

    float4 finalColor = ComputeAmbient(currentMaterial.ambientLightColor, currentMaterial.ambient, albedo);

    float3 diffuseColor = float3(0.0f, 0.0f, 0.0f);
    float3 specularColor = float3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 10; ++i)
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
            float3 lightColorSpot = currentLight.cbPerLight_color * currentLight.cbPerLight_lightIntensity;
            float3 lightPositionSpot = currentLight.cbPerLight_position;
            float3 lightDirectionSpot = normalize(currentLight.cbPerLight_direction);
            float spotAngle = radians(currentLight.cbPerLight_direction);

            float spotIntensity = ComputeSpotIntensity(lightPositionSpot, lightDirectionSpot, worldPos.xyz, spotAngle);

            diffuseColor += ComputeDiffuse(lightDirectionSpot, normal, lightColorSpot, currentMaterial.diffuse) * spotIntensity;
            specularColor += ComputePhongSpecular(lightDirectionSpot, normal, viewDirection, lightColorSpot, currentMaterial.specular, currentMaterial.shininess) * spotIntensity;
        }
        else if (currentLight.cbPerLight_lightType == LIGHT_TYPE_POINT)
        {
            float3 lightColorPoint = currentLight.cbPerLight_color;
            float3 lightPositionPoint = currentLight.cbPerLight_position;

            float pointLightIntensity = ComputePointLightIntensity(lightPositionPoint, worldPos.xyz, currentLight.cbPerLight_lightIntensity);

            float3 lightDirectionPoint = normalize(lightPositionPoint - worldPos.xyz);

            diffuseColor += ComputeDiffuse(lightDirectionPoint, normal, lightColorPoint, diffuse) * pointLightIntensity;
            specularColor += ComputePhongSpecular(lightDirectionPoint, normal, viewDirection, lightColorPoint, currentMaterial.specular, currentMaterial.shininess) * pointLightIntensity;
        }
    }

    finalColor.rgb += diffuseColor * albedo.rgb;
    finalColor.rgb += specularColor * albedo.rgb;
    finalColor.a = albedo.a;
   
    return finalColor;
}


//float4 PS(VSOutput pin) : SV_Target
//{
//    float4 albedo = g_buffer_albedo.Sample(g_sampler, pin.UV);
//    float4 remappedNormal = g_buffer_normal.Sample(g_sampler, pin.UV);
    
//    // Don't calculate light on rtv clear color value
//    if (albedo.r == 0.678431392f || albedo.g == 0.847058892f || albedo.b == 0.901960850f)
//    {
//        return float4(0.678431392f, 0.847058892f, 0.901960850f, 1.0f);
//    }

//    float3 normal = remappedNormal.xyz * 2.0 - 1.0;
//    float4 finalColor = ComputeAmbient(ambientLightColor, ambient, albedo);

//    for (int i = 0; i < 10; ++i)
//    {
//        Light currentLight = lights[i];

//        if (currentLight.cbPerLight_lightType == LIGHT_TYPE_DIRECTIONAL)
//        {
//            float3 lightColorDirectional = currentLight.cbPerLight_color * currentLight.cbPerLight_lightIntensity;
//            float3 lightDirectionDirectional = -normalize(currentLight.cbPerLight_direction); // Direction inversée pour la lumière directionnelle

//            float diffuseIntensity = dot(normal, lightDirectionDirectional);
//            diffuseIntensity = saturate(diffuseIntensity);

//            finalColor.rgb += ComputeDiffuse(lightDirectionDirectional, normal, lightColorDirectional, diffuse) * diffuseIntensity;
//            finalColor.rgb += ComputePhongSpecular(lightDirectionDirectional, normal, float3(0, 0, -1), lightColorDirectional, specular, shininess); // Direction de vue fixe (par exemple, vers le bas)
//        }
//        else if (currentLight.cbPerLight_lightType == LIGHT_TYPE_SPOT)
//        {
 
//            float3 lightColorSpot = currentLight.cbPerLight_color * currentLight.cbPerLight_lightIntensity;
//            float3 lightPositionSpot = currentLight.cbPerLight_position;
//            float3 lightDirectionSpot = normalize(currentLight.cbPerLight_direction);
//            float spotAngle = radians(currentLight.cbPerLight_direction);

//            float spotIntensity = ComputeSpotIntensity(lightPositionSpot, lightDirectionSpot, float3(pin.UV, 0), spotAngle);

//            finalColor.rgb += ComputeDiffuse(lightDirectionSpot, normal, lightColorSpot, diffuse) * spotIntensity;
//            finalColor.rgb += ComputePhongSpecular(lightDirectionSpot, normal, float3(0, 0, -1), lightColorSpot, specular, shininess) * spotIntensity;
//        }
//        else if (currentLight.cbPerLight_lightType == LIGHT_TYPE_POINT)
//        {
//            float3 lightColorPoint = currentLight.cbPerLight_color;

//            float3 lightPositionPoint = currentLight.cbPerLight_position;

//            float pointLightIntensity = ComputePointLightIntensity(lightPositionPoint, float3(pin.UV, 0), currentLight.cbPerLight_lightIntensity);

//            float3 lightDirectionPoint = normalize(lightPositionPoint - float3(pin.UV, 0));
//            float3 diffuseComponent = ComputeDiffuse(lightDirectionPoint, normal, lightColorPoint, diffuse) * pointLightIntensity;

//            float3 viewDirection2D = float3(0, 0, -1);
//            float3 specularComponent = ComputePhongSpecular(lightDirectionPoint, normal, viewDirection2D, lightColorPoint, specular, shininess) * pointLightIntensity;

//            finalColor.rgb += diffuseComponent;
//            finalColor.rgb += specularComponent;
//        }
//    }

//    finalColor.rgb *= albedo.rgb;
//    finalColor.a = albedo.a;

//    return finalColor;
//}

