#include "Utils.hlsl"

Texture2D g_texture : register(t0); // Texture bound to t0, register space 0
SamplerState g_sampler : register(s0); // Sampler bound to s0

cbuffer cbPerObject : register(b0)
{
    float4x4 gWorld; // World matrix
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

// Vertex Shader
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

// Pixel Shader
float4 PS(VertexOut pin) : SV_Target
{
    // Échantillonner la texture à partir des coordonnées UV du vertex
    float4 texColor = g_texture.Sample(g_sampler, pin.UV);
    
    // Appliquer l'alpha pour la transparence
    float alpha = texColor.a;

    // Retourner la couleur texturée avec l'alpha
    return float4(texColor.rgb, alpha);
}
