Texture2D g_texture : register(t0); // Texture d'entr�e li�e au registre t0
SamplerState g_sampler : register(s0); // Sampler li� au registre s0

struct VertexIn
{
    float3 PosL : POSITION; // Position du vertex en local
    float2 UV : TEXCOORD; // Coordonn�es de texture
};

struct VertexOut
{
    float4 PosH : SV_POSITION; // Position du vertex en espace homog�ne
    float2 UV : TEXCOORD; // Coordonn�es de texture pass�es au pixel shader
};

// Vertex Shader
VertexOut VS(VertexIn vin)
{
    VertexOut vout;
    
    // Aucune transformation n�cessaire pour le post-processing
    vout.PosH = float4(vin.PosL, 1.0f); // Utilise directement la position du vertex
    vout.UV = vin.UV; // Passe les coordonn�es de texture au pixel shader
    
    return vout;
}

// Pixel Shader
float4 PS(VertexOut pin) : SV_Target
{
    // �chantillonner la texture � partir des coordonn�es UV du pixel
    float4 texColor = g_texture.Sample(g_sampler, pin.UV);
    
    // Ici, appliquez votre effet de post-processing
    // Par exemple, appliquer une teinte ou tout autre effet souhait�
    
    // Pour cet exemple simple, on ne fait que retourner la couleur �chantillonn�e sans modification
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
