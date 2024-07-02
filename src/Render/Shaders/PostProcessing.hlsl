Texture2D g_texture : register(t0); // Texture d'entrée liée au registre t0
SamplerState g_sampler : register(s0); // Sampler lié au registre s0

struct VertexIn
{
    float3 PosL : POSITION; // Position du vertex en local
    float2 UV : TEXCOORD; // Coordonnées de texture
};

struct VertexOut
{
    float4 PosH : SV_POSITION; // Position du vertex en espace homogène
    float2 UV : TEXCOORD; // Coordonnées de texture passées au pixel shader
};

// Vertex Shader
VertexOut VS(VertexIn vin)
{
    VertexOut vout;
    
    // Aucune transformation nécessaire pour le post-processing
    vout.PosH = float4(vin.PosL, 1.0f); // Utilise directement la position du vertex
    vout.UV = vin.UV; // Passe les coordonnées de texture au pixel shader
    
    return vout;
}

// Pixel Shader
float4 PS(VertexOut pin) : SV_Target
{
    // Échantillonner la texture à partir des coordonnées UV du pixel
    float4 texColor = g_texture.Sample(g_sampler, pin.UV);
    
    // Ici, appliquez votre effet de post-processing
    // Par exemple, appliquer une teinte ou tout autre effet souhaité
    
    // Pour cet exemple simple, on ne fait que retourner la couleur échantillonnée sans modification
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
