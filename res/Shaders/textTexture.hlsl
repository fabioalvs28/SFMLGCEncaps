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
    float4 Color : COLOR;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float2 UV : TEXCOORD;
    float4 Color : COLOR;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
    
    float4x4 gWorldTransposed = TransposeMatrix(gWorld);
    
    vout.PosH = mul(mul(float4(vin.PosL, 1.0f), gWorldTransposed), mul(gView, gProj));
    
    vout.UV = vin.UV;
    vout.Color = vin.Color;
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{    
    float4 texColor = g_texture.Sample(g_sampler, pin.UV);
    float4 white = (1.0f, 1.0f, 1.0f, 1.0f);
    float4 outColor = (texColor * pin.Color);
    
    outColor.a = texColor.a;
    return outColor;
}