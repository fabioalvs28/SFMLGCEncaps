#include "Utils.hlsl"

cbuffer cbPerObject : register(b0)
{
    float4x4 gWorld; // World matrix
    float objectId;
    float materialId;
};

cbuffer cbPerCamera : register(b1)
{
    float4x4 gView;
    float4x4 gProj;
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
    
    float r = float(objectId % 256) / 255.0f;
    float g = float(materialId % 256) / 255.0f;
    output.color2 = float4(r, g, 0.0f, 1.0f);
    
    float3 remappedNormal = (pin.NormalW * 0.5) + 0.5;
    output.Albedo = float4(finalColor.rgb, finalColor.a);
    output.WorldPos = float4(pin.WorldPos, 1.0f);
    output.Normal = float4(remappedNormal, 1.0f);
    
    
    //output.Albedo = float4(0.6f, 0.5f, 1.0f, 1.0f);
    //output.WorldPos = float4(0.3f, 0.8f, 0.0f, 1.0f);
    //output.Normal = float4(1.0f, 0.5f, 0.0f, 1.0f); // Normales remappées
    

    
    return output;
}



