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

struct VSInput
{
    float3 PosL : POSITION;
};

struct VSOutput
{
    float4 PosH : SV_POSITION;
};

VSOutput VS(VSInput input)
{
    VSOutput output;
    
    float4 posW = mul(float4(input.PosL, 1.0f), gWorld);
    output.PosH = mul(mul(posW, gView), gProj);

    return output;
}

float4 PS(VSOutput input) : SV_Target
{
 
    return float4(objectId, 0.2f, 0.5f, 1.0f);
}