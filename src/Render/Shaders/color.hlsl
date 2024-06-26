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
    float3 Normal : NORMAL;
    float4 Color : COLOR;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 NormalW : NORMAL;
    float4 Color : COLOR;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
    
    // Transform position to homogeneous clip space using gWorld, gView, and gProj matrices.
    float4 posW = mul(float4(vin.PosL, 1.0f), gWorld);
    vout.PosH = mul(mul(posW, gView), gProj);
    
    // Transform normal to world space
    vout.NormalW = mul(vin.Normal, (float3x3) gWorld);
    
    // Pass vertex color to the pixel shader.
    vout.Color = vin.Color;
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    float3 lightDirection = normalize(float3(1.0f, 1.0f, 1.0f)); // Example light direction
    float3 lightColor = float3(1.0f, 1.0f, 1.0f); // Example light color (white)

    // Diffuse calculation
    float diffuseFactor = max(0.0f, dot(normalize(pin.NormalW), lightDirection));
    float3 diffuseColor = pin.Color.rgb * diffuseFactor * lightColor;

    return float4(diffuseColor, pin.Color.a); // Use alpha from input color
}
