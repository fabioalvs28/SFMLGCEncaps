Texture2D g_texture : register(t0); // Texture d'entrée liée au registre t0
Texture2D g_pixelIdMapping : register(t1); // Deuxième texture d'entrée liée au registre t1
SamplerState g_sampler : register(s0); // Sampler lié au registre s0


// Vertex shader
struct VSOutput
{
    float4 PosH : SV_POSITION;
    float2 UV : TEXCOORD; 
};

// Vertex Shader
VSOutput VS(float3 posL : POSITION, float2 uv : TEXCOORD)
{
    VSOutput output;
    output.PosH = float4(posL, 1.0f);
    output.UV = uv; 
    return output;
}

// Pixel shader principal
float4 PS(VSOutput pin) : SV_Target
{
    float4 sampledColor = g_texture.Sample(g_sampler, pin.UV);
    float4 sampledMeshBufferId = g_pixelIdMapping.Sample(g_sampler, pin.UV);


    // Decode rgb
    float objectId = sampledMeshBufferId.r * 255.0f; 
    //objectId += sampledMeshBufferId.g * 255.0f; 
    //objectId += sampledMeshBufferId.b * 255.0f * 255.0f; 
    
    //if (objectId == 2.0f)
    //{
    //    return float4(0.5f, 0.3f, 0.6f, 1.0f);
    //}


    return sampledColor;
}