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
    //float4 color2 : SV_Target1; #TOTHINK pas d'object id sinon decalage
    
    float4 Albedo : SV_Target2; // G-Buffer: Albedo
    float4 WorldPos : SV_Target3; // G-Buffer: World Position
    float4 Normal : SV_Target4; // G-Buffer: Normal
};

// Pixel shader
PSOutput PS(VertexOut pin) : SV_Target
{
    PSOutput output;
    
    float4 finalColor;
    finalColor.rgb = pin.Color.rgb;
    finalColor.a = pin.Color.a;
    
    output.color1 = finalColor;
    
    //output.Albedo = float4(0.89f, 0.49f, 0.09f, 1.0f);
    //output.WorldPos = float4(0.2f, 0.4f, 0.9f, 1.0f);
    //output.Normal = float4(0.2f, 0.8f, 0.3f, 1.0f);
    output.Albedo = float4(finalColor.rgb, finalColor.a);
    output.WorldPos = float4(pin.WorldPos, 1.0f);
    output.Normal = float4(pin.NormalW, 1.0f);
    
    //float r = float(objectId % 256) / 255.0f;
    //output.color2 = float4(r, 0.0f, 0.0f, 1.0f);
    
    return output;
}



