#include "Utils.hlsl"

Texture2D g_texture : register(t0); // Texture bound to t0, register space 0
SamplerState g_sampler : register(s0); // Sampler bound to s0, register space 0

cbuffer cbPerObject : register(b0)
{
    float4x4 gWorld;
    float objectId;
};

cbuffer cbPerCamera : register(b1)
{
    float4x4 gView;
    float4x4 gProj;
};

struct VertexIn
{
    float3 PosL : POSITION;
    float2 UV : TEXCOORD;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float2 UV : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
    
    float4x4 gWorldTransposed = TransposeMatrix(gWorld);
    
    // Transform to homogeneous clip space using gWorld, gView, and gProj matrices.
    vout.PosH = mul(mul(float4(vin.PosL, 1.0f), gWorldTransposed), mul(gView, gProj));
    
    // Pass vertex texture coordinates into the pixel shader.
    vout.UV = vin.UV;
    
    return vout;
}

//struct PSOutput
//{
//    float4 color1 : SV_Target0;
//    float4 color2 : SV_Target1;
//};

//PSOutput PS(VertexOut pin) : SV_Target
//{
//    float4 texColor = g_texture.Sample(g_sampler, pin.UV);
//    return texColor; // Apply transparency using alpha channel
    
//    PSOutput output;
//    output.color1 = texColor;
    
//    float r = float(objectId % 256) / 255.0f;
//    output.color2 = float4(r, 0.0f, 0.0f, 1.0f);
    
//    return output;
//}

float4 PS(VertexOut pin) : SV_Target
{
    float4 texColor = g_texture.Sample(g_sampler, pin.UV);
    return texColor; // Apply transparency using alpha channel
    
   
}
