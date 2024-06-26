Texture2D g_texture : register(t0); // Texture bound to t0, register space 0
SamplerState g_sampler : register(s0); // Sampler bound to s0

cbuffer cbPerObject : register(b0)
{
    float4x4 gWorld;
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
    float3 Normal : NORMAL;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float2 UV : TEXCOORD;
    float3 NormalW : NORMAL;
};

// Vertex Shader
VertexOut VS(VertexIn vin)
{
    VertexOut vout;
    
    // Transform to homogeneous clip space using gWorld, gView, and gProj matrices.
    vout.PosH = mul(mul(float4(vin.PosL, 1.0f), gWorld), mul(gView, gProj));
    
    // Pass vertex texture coordinates into the pixel shader.
    vout.UV = vin.UV;
    vout.NormalW = mul(vin.Normal, (float3x3) gWorld); // Transform normal to world space
    
    return vout;
}

// Fonction pour calculer la couleur ambiante
float4 ComputeAmbientColor(float4 ambientLightColor, float4 materialAmbient, float4 texColor)
{
    return ambientLightColor * texColor * materialAmbient;
}

// Fonction pour calculer la couleur diffuse
float3 ComputeDiffuseColor(float3 lightDirection, float3 normal, float3 lightColor, float4 materialDiffuse)
{
    float diffuseFactor = max(0.0f, dot(normal, lightDirection));
    return lightColor * materialDiffuse.rgb * diffuseFactor;
}

// Fonction pour calculer la couleur spéculaire
float3 ComputeSpecularColor(float3 lightDirection, float3 viewDirection, float3 normal, float3 lightColor, float4 materialSpecular, float materialShininess)
{
    float3 reflectDirection = reflect(-lightDirection, normal);
    float specularFactor = pow(max(dot(viewDirection, reflectDirection), 0.0f), materialShininess);
    return lightColor * materialSpecular.rgb * specularFactor;
}

// Pixel Shader
float4 PS(VertexOut pin) : SV_Target
{
    // Couleur ambiante (peut varier)
    float4 ambientLightColor = float4(0.2f, 0.2f, 0.2f, 1.0f);

    // Échantillonner la texture à partir des coordonnées UV du vertex
    float4 texColor = g_texture.Sample(g_sampler, pin.UV);

    // Paramètres de la lumière directionnelle (simule le soleil)
    float3 lightColor = float3(1.0f, 1.0f, 1.0f);
    float3 lightDirection = normalize(float3(0.8f, -1.0f, 0.8f)); // Direction de la lumière du soleil

    // Paramètres du matériau
    float4 materialAmbient = float4(0.2f, 0.2f, 0.2f, 1.0f);
    float4 materialDiffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);
    float4 materialSpecular = float4(1.0f, 1.0f, 1.0f, 1.0f);
    float materialShininess = 32.0f;

    // Calculer la couleur ambiante
    float4 ambientColor = ComputeAmbientColor(ambientLightColor, materialAmbient, texColor);

    // Calculer la couleur diffuse
    float3 normal = normalize(pin.NormalW);
    float3 diffuseColor = ComputeDiffuseColor(lightDirection, normal, lightColor, materialDiffuse);
    
    // Calculer la couleur spéculaire
    float3 viewDirection = normalize(-pin.PosH.xyz); // Vue direction
    float3 specularColor = ComputeSpecularColor(lightDirection, viewDirection, normal, lightColor, materialSpecular, materialShininess);

    // Combiner toutes les couleurs
    float3 finalColor = ambientColor.rgb + diffuseColor + specularColor;
    finalColor = texColor.rgb * finalColor;

    // Appliquer l'alpha pour la transparence
    finalColor *= texColor.a;

    return float4(finalColor, 1.0f);
}
