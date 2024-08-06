#include "Utils.hlsl"

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

struct VertexIn
{
    float3 PosL : POSITION;
    float4 Color : COLOR;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float4 Color : COLOR;
};

// Vertex shader
VertexOut VS(VertexIn vin)
{
    float4x4 gWorldTransposed = TransposeMatrix(gWorld);
    
    VertexOut vout;

    // Transform position to homogeneous clip space using gWorld, gView, and gProj matrices.
    vout.PosH = mul(mul(mul(float4(vin.PosL, 1.0f), gWorldTransposed), gView), gProj);

    // Pass vertex color to the pixel shader.
    vout.Color = vin.Color;

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
    output.color1 = pin.Color;
    
    // Encode rgb
    float r = float(objectId % 256) / 255.0f; 
    //float g = float((objectId / 256) % 256) / 255.0f; 
    //float b = float((objectId / (256 * 256)) % 256) / 255.0f; 
    // #TODO Encoder d'autres paramètres | depth?
    
    output.color2 = float4(r, 0.0f, 0.0f, 1.0f);
    // Color & Alpha
    return output;
}

