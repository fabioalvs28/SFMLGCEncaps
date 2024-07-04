Texture2D g_texture : register(t0); // Texture d'entrée liée au registre t0
SamplerState g_sampler : register(s0); // Sampler lié au registre s0

// Vertex shader
struct VSOutput
{
    float4 PosH : SV_POSITION;
    float2 UV : TEXCOORD; // Utilisation de TEXCOORD pour les coordonnées de texture (UV)
};

// Vertex Shader
VSOutput VS(float3 posL : POSITION, float2 uv : TEXCOORD)
{
    VSOutput output;
    output.PosH = float4(posL, 1.0f);
    output.UV = uv; // Assigner les coordonnées de texture (UV) à output.UV
    return output;
}

// Pixel shader
// Pixel shader
float4 PS(VSOutput pin) : SV_Target
{
    float w;
    float h;
    const int BlurRadius = 30; // Rayon du flou gaussien (nombre d'échantillons de chaque côté du pixel central)
    static const float GaussianWeights[30] =
    {
        0.009259f, 0.012945f, 0.017284f, 0.022397f, 0.028401f,
        0.035399f, 0.043466f, 0.052647f, 0.062957f, 0.074381f,
        0.086874f, 0.100356f, 0.114710f, 0.129790f, 0.145423f,
        0.161411f, 0.177535f, 0.193554f, 0.209217f, 0.224266f,
        0.238449f, 0.251523f, 0.263263f, 0.273468f, 0.281964f,
        0.288610f, 0.293304f, 0.295981f, 0.296614f, 0.295215f
    };

    g_texture.GetDimensions(w, h);
    
    // Échantillonner la texture à partir des coordonnées UV
    float4 sampledColor = g_texture.Sample(g_sampler, pin.UV);
    
    // Initialiser le accumulateur pour la couleur filtrée
    float4 filteredColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // Échantillonner les pixels environnants selon le noyau de flou gaussien
    for (int i = -BlurRadius; i <= BlurRadius; ++i)
    {
        int index = i + BlurRadius;
        if (index >= 0 && index < 30) // Check if index is within bounds
        {
            float weight = GaussianWeights[index];
        
            float2 offset = float2(i / w, 0.0f); // Adjusted to ensure correct offset
            float2 uv = pin.UV + offset;

            float4 neighborColor = g_texture.Sample(g_sampler, uv);
            filteredColor += neighborColor * weight;
        }
    }

    return filteredColor;
}