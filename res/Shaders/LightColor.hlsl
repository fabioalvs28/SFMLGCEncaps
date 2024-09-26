#include "Utils.hlsl"

#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_SPOT 1
#define LIGHT_TYPE_POINT 2

cbuffer cbPerObject : register(b0)
{
    float4x4 gWorld; // World matrix
    float objectId;
};

cbuffer cbPerCamera : register(b1)
{
    float4x4 gView;
    float4x4 gProj;
};

cbuffer cbPerMaterial : register(b2)
{
    float4 cbPerMaterial_ambientLightColor;
    float4 cbPerMaterial_ambient;
    float4 cbPerMaterial_diffuse;
    float4 cbPerMaterial_specular;
    float cbPerMaterial_shininess;
    float cbPerMaterial_padding3[3]; 
};

struct Light
{
    float3 cbPerLight_position;
    float3 cbPerLight_direction;
    float3 cbPerLight_color;
    float cbPerLight_spotAngle;
    int cbPerLight_lightType;
    float cbPerLight_lightIntensity;
};

cbuffer cbPerLights : register(b3)
{
    Light lights[10];
};

struct VertexIn
{
    float3 PosL : POSITION;
    float4 Color : COLOR;
    float3 Normal : NORMAL;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float4 Color : COLOR;
    float3 NormalW : NORMAL;
    float3 WorldPos : POSITION; // Position in world space
};

// Vertex shader
VertexOut VS(VertexIn vin)
{
    VertexOut vout;

    float4x4 gWorldTransposed = TransposeMatrix(gWorld);
    
    // Transform position to world space
    float4 posW = mul(float4(vin.PosL, 1.0f), gWorldTransposed);

    // Transform position to homogeneous clip space using gWorld, gView, and gProj matrices.
    vout.PosH = mul(mul(posW, gView), gProj);

    // Transform normal to world space
    vout.NormalW = normalize(mul(vin.Normal, (float3x3) gWorldTransposed));

    // Pass vertex color to the pixel shader.
    vout.Color = vin.Color;

    // Pass world position to the pixel shader
    vout.WorldPos = posW.xyz;

    return vout;
}

struct PSOutput
{
    float4 color1 : SV_Target0;
    float4 color2 : SV_Target1;
};

// Pixel shader
PSOutput PS(VertexOut pin) : SV_Target
{
    PSOutput output;
    
    float3x3 invViewMatrix = (float3x3) transpose(gView); // Transpose of view matrix
    float3 cameraPosition = -mul(invViewMatrix, gView[3].xyz); // Camera position in world space

    float3 viewDirection = normalize(cameraPosition - pin.WorldPos); // View direction

    float4 finalColor = ComputeAmbient(cbPerMaterial_ambientLightColor, cbPerMaterial_ambient, pin.Color);

    float3 diffuseColor = float3(0.0f, 0.0f, 0.0f);
    float3 specularColor = float3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 10; ++i)
    {
        Light currentLight = lights[i];

        if (currentLight.cbPerLight_lightType == LIGHT_TYPE_DIRECTIONAL)
        {
            // Directional light calculations
            float3 lightColorDirectional = currentLight.cbPerLight_color * currentLight.cbPerLight_lightIntensity;
            float3 lightDirectionDirectional = -normalize(currentLight.cbPerLight_direction); // Invert direction for directional light

            float diffuseIntensity = dot(pin.NormalW, lightDirectionDirectional);
            diffuseIntensity = saturate(diffuseIntensity);

            diffuseColor += ComputeDiffuse(lightDirectionDirectional, pin.NormalW, lightColorDirectional, cbPerMaterial_diffuse) * diffuseIntensity;
            specularColor += ComputePhongSpecular(lightDirectionDirectional, pin.NormalW, viewDirection, lightColorDirectional, cbPerMaterial_specular, cbPerMaterial_shininess);
        }
        else if (currentLight.cbPerLight_lightType == LIGHT_TYPE_SPOT)
        {
            // Spot light calculations
            float3 lightColorSpot = currentLight.cbPerLight_color * currentLight.cbPerLight_lightIntensity;
            float3 lightPositionSpot = currentLight.cbPerLight_position;
            float3 lightDirectionSpot = normalize(currentLight.cbPerLight_direction);
            float spotAngle = radians(currentLight.cbPerLight_spotAngle);

            float spotIntensity = ComputeSpotIntensity(lightPositionSpot, lightDirectionSpot, pin.WorldPos, spotAngle);

            diffuseColor += ComputeDiffuse(lightDirectionSpot, pin.NormalW, lightColorSpot, cbPerMaterial_diffuse) * spotIntensity;
            specularColor += ComputePhongSpecular(lightDirectionSpot, pin.NormalW, viewDirection, lightColorSpot, cbPerMaterial_specular, cbPerMaterial_shininess) * spotIntensity;
        }
        else if (currentLight.cbPerLight_lightType == LIGHT_TYPE_POINT)
        {
            // Point light calculations
            float3 lightColorPoint = currentLight.cbPerLight_color;
            float3 lightPositionPoint = currentLight.cbPerLight_position;

            float pointLightIntensity = ComputePointLightIntensity(lightPositionPoint, pin.WorldPos, currentLight.cbPerLight_lightIntensity);

            float3 lightDirectionPoint = normalize(lightPositionPoint - pin.WorldPos);

            diffuseColor += ComputeDiffuse(lightDirectionPoint, pin.NormalW, lightColorPoint, cbPerMaterial_diffuse) * pointLightIntensity;
            specularColor += ComputePhongSpecular(lightDirectionPoint, pin.NormalW, viewDirection, lightColorPoint, cbPerMaterial_specular, cbPerMaterial_shininess) * pointLightIntensity;
        }
    }

    finalColor.rgb += diffuseColor * pin.Color.rgb;
    finalColor.rgb += specularColor * pin.Color.rgb;
    finalColor.a = pin.Color.a;
    
    output.color1 = finalColor;
    
    float r = float(objectId % 256) / 255.0f;
    output.color2 = float4(r, 0.0f, 0.0f, 1.0f);
    
    return output;
}



