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
    
    // Transform to homogeneous clip space using gWorld, gView, and gProj matrices.
    vout.PosH = mul(mul(float4(vin.PosL, 1.0f), gWorld), mul(gView, gProj));
    
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
    
    return float4(texColor.rgb, alpha);
}

//float4 PS(VertexOut pin) : SV_Target
//{
//    // Échantillonner la texture à partir des coordonnées UV du vertex
//    float4 texColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
//    // Définir le rayon du flou (plus le rayon est grand, plus le flou est prononcé)
//    float blurRadius = 0.005f;
    
//    // Nombre d'échantillons pour le flou (plus il y a d'échantillons, plus le flou est doux)
//    int numSamples = 15;
    
//    // Poids de chaque échantillon (pour une distribution gaussienne)
//    float weight = 1.0f / numSamples;
    
//    // Somme des couleurs pondérées
//    for (int i = 0; i < numSamples; ++i)
//    {
//        // Offset pour l'échantillonnage autour du pixel courant
//        float2 offset = float2(cos(i * 2 * 3.14159 / numSamples), sin(i * 2 * 3.14159 / numSamples)) * blurRadius;
        
//        // Échantillonner la texture avec l'offset
//        texColor += g_texture.Sample(g_sampler, pin.UV + offset) * weight;
//    }
    
//    // Appliquer l'alpha pour la transparence
//    float alpha = texColor.a;
    
//    // Retourner la couleur texturée avec l'alpha
//    return float4(texColor.rgb, alpha);
//}