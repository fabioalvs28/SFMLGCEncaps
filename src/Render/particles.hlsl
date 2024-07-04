cbuffer ViewProjection : register(b0)
{
    matrix viewProjection;
};

struct Particle
{
    float3 position : POSITION;
    float3 velocity : VELOCITY;
    float lifetime : LIFETIME;
    float age : AGE;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VS_OUTPUT VSMain(Particle input)
{
    VS_OUTPUT output;
    output.position = mul(float4(input.position, 1.0f), viewProjection);
    output.color = float4(1.0f, 1.0f, 1.0f, 1.0f - input.age / input.lifetime); // Fade out based on age
    return output;
}

float4 PSMain(VS_OUTPUT input) : SV_TARGET
{
    return input.color;
}
