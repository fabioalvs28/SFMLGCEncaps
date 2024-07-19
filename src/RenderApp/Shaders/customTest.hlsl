Texture2D g_texture : register(t0); // Texture bound to t0, register space 0
SamplerState g_sampler : register(s0); // Sampler bound to s0, register space 0

cbuffer cbPerObject : register(b0)
{
    float4x4 gWorld;
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
    
    // Transform to homogeneous clip space using gWorld, gView, and gProj matrices.
    vout.PosH = mul(mul(float4(vin.PosL, 1.0f), gWorld), mul(gView, gProj));
    
    // Pass vertex texture coordinates into the pixel shader.
    vout.UV = vin.UV;
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    float4 texColor = g_texture.Sample(g_sampler, pin.UV);
    return texColor * texColor.a; // Apply transparency using alpha channel
}

    //vout.PosH = mul(mul(float4(vin.PosL, 1.0f), float4x4(1.0f, 0.0f, 0.0f, 0.0f,
    //                                                  0.0f, 1.0f, 0.0f, 0.0f,
    //                                                  0.0f, 0.0f, 1.0f, 0.0f,
    //                                                  0.0f, 0.0f, 0.0f, 1.0f)), mul(gView, gProj));