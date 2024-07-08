Texture2D g_texture : register(t0); // Texture d'entr�e li�e au registre t0
Texture2D g_meshBufferIdTexture : register(t1); // Deuxi�me texture d'entr�e li�e au registre t1
SamplerState g_sampler : register(s0); // Sampler li� au registre s0


// Vertex shader
struct VSOutput
{
    float4 PosH : SV_POSITION;
    float2 UV : TEXCOORD; // Utilisation de TEXCOORD pour les coordonn�es de texture (UV)
};

// Vertex Shader
VSOutput VS(float3 posL : POSITION, float2 uv : TEXCOORD)
{
    VSOutput output;
    output.PosH = float4(posL, 1.0f);
    output.UV = uv; // Assigner les coordonn�es de texture (UV) � output.UV
    return output;
}

// Pixel shader
//float4 PS(VSOutput pin) : SV_Target
//{
//    float w;
//    float h;
//    const int BlurRadius = 30; // Rayon du flou gaussien (nombre d'�chantillons de chaque c�t� du pixel central)
//    static const float GaussianWeights[30] =
//    {
//        0.009259f, 0.012945f, 0.017284f, 0.022397f, 0.028401f,
//        0.035399f, 0.043466f, 0.052647f, 0.062957f, 0.074381f,
//        0.086874f, 0.100356f, 0.114710f, 0.129790f, 0.145423f,
//        0.161411f, 0.177535f, 0.193554f, 0.209217f, 0.224266f,
//        0.238449f, 0.251523f, 0.263263f, 0.273468f, 0.281964f,
//        0.288610f, 0.293304f, 0.295981f, 0.296614f, 0.295215f
//    };

//    g_texture.GetDimensions(w, h);
    
//    // �chantillonner la texture � partir des coordonn�es UV
//    float4 sampledColor = g_texture.Sample(g_sampler, pin.UV);
    
//    // Initialiser le accumulateur pour la couleur filtr�e
//    float4 filteredColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

//    // �chantillonner les pixels environnants selon le noyau de flou gaussien
//    for (int i = -BlurRadius; i <= BlurRadius; ++i)
//    {
//        int index = i + BlurRadius;
//        if (index >= 0 && index < 30) // Check if index is within bounds
//        {
//            float weight = GaussianWeights[index];
        
//            float2 offset = float2(i / w, 0.0f); // Adjusted to ensure correct offset
//            float2 uv = pin.UV + offset;

//            float4 neighborColor = g_texture.Sample(g_sampler, uv);
//            filteredColor += neighborColor * weight;
//        }
//    }


//    return filteredColor;
//}

// Fonction pour calculer le bloom autour de l'objet

// Fonction pour calculer l'intensit� du bloom en fonction des coordonn�es UV et de l'identifiant d'objet
float CalculateBloomIntensity(float2 uv, float objectId)
{
    const int BlurRadius = 5; // Rayon du flou gaussien autour de l'objet
    static const float GaussianWeights[5] = { 0.227027027f, 0.194594595f, 0.121621622f, 0.054054054f, 0.0162162162f };

    float bloomIntensity = 0.0f;

    // �chantillonner les pixels environnants selon le noyau de flou gaussien
    for (int i = -BlurRadius; i <= BlurRadius; ++i)
    {
        float2 offset = float2(i, 0.0f); // Ajuster l'offset pour le flou autour de l'objet
        float2 uvOffset = uv + offset;

        float4 neighborMeshBufferId = g_meshBufferIdTexture.Sample(g_sampler, uvOffset);
        float neighborObjectId = neighborMeshBufferId.r;

        // Appliquer le bloom si l'objet voisin a un identifiant similaire
        if (neighborObjectId >= 99 && neighborObjectId <= 102)
        {
            float weight = GaussianWeights[abs(i)];
            bloomIntensity += weight;
        }
    }

    return bloomIntensity;
}
// Pixel shader principal
float4 PS(VSOutput pin) : SV_Target
{
    float4 sampledColor = g_texture.Sample(g_sampler, pin.UV);

    // �chantillonnez l'identifiant d'objet de la texture
    float4 sampledMeshBufferId = g_meshBufferIdTexture.Sample(g_sampler, pin.UV);
    float objectId = sampledMeshBufferId.r;

    // Condition pour appliquer le bloom
    if (objectId >= 99 && objectId <= 102)
    {
        // Calcul du bloom pour l'objet (et potentiellement autour de l'objet)
        float bloomIntensity = CalculateBloomIntensity(pin.UV, objectId);

        // Appliquer le bloom � l'�chantillon de couleur
        sampledColor.rgb += bloomIntensity;
    }
    else
    {
        // Ne pas appliquer le bloom aux pixels qui ne satisfont pas la condition
        // Vous pouvez laisser cette partie vide ou retourner simplement l'�chantillon de couleur sans modification
    }

    return sampledColor;
}

