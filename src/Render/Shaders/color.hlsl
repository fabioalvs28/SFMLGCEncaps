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
    float4 Color : COLOR;
    float3 Normal : NORMAL;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float4 Color : COLOR;
    float3 NormalW : NORMAL;
};

// Fonction pour calculer la couleur ambiante
float4 ComputeAmbient(float4 ambientLightColor, float4 materialAmbient, float4 vertexColor)
{
    return ambientLightColor * materialAmbient * vertexColor;
}

// Fonction pour calculer la couleur diffuse
float3 ComputeDiffuse(float3 lightDirection, float3 normal, float3 lightColor, float4 materialDiffuse)
{
    float3 normalizedNormal = normalize(normal); // Normalisation de la normale
    float diffuseFactor = max(0.0f, dot(normalizedNormal, lightDirection));
    return lightColor * materialDiffuse.rgb * diffuseFactor;
}

// Fonction pour calculer la couleur spéculaire
float3 ComputeSpecular(float3 lightDirection, float3 normal, float3 viewDirection, float3 lightColor, float4 materialSpecular, float shininess)
{
    float3 reflectDirection = reflect(-lightDirection, normal);
    float specularFactor = pow(max(dot(viewDirection, reflectDirection), 0.0f), shininess);
    return lightColor * materialSpecular.rgb * specularFactor;
}

// Vertex shader
VertexOut VS(VertexIn vin)
{
    VertexOut vout;

    // Transform position to world space
    float4 posW = mul(float4(vin.PosL, 1.0f), gWorld);

    // Transform position to homogeneous clip space using gWorld, gView, and gProj matrices.
    vout.PosH = mul(mul(posW, gView), gProj);

    // Transform normal to world space
    vout.NormalW = normalize(mul(vin.Normal, (float3x3) gWorld));

    // Pass vertex color to the pixel shader.
    vout.Color = vin.Color;

    return vout;
}

// Pixel shader
float4 PS(VertexOut pin) : SV_Target
{
    // Couleur ambiante (peut varier)
    float4 ambientLightColor = float4(0.2f, 0.2f, 0.2f, 1.0f);

    // Paramètres de la lumière directionnelle (simule le soleil)
    float3 lightColor = float3(1.0f, 1.0f, 1.0f);
    float3 lightDirection = normalize(float3(0.8f, -1.0f, 0.8f)); // Direction de la lumière du soleil

    // Paramètres du matériau
    float4 materialAmbient = float4(0.2f, 0.2f, 0.2f, 1.0f);
    float4 materialDiffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);
    float4 materialSpecular = float4(1.0f, 1.0f, 1.0f, 1.0f);
    float materialShininess = 32.0f;

    // Calculer les composantes de la lumière directionnelle
    float3 cameraPosition = -gView[3].xyz; // Récupération de la position de la caméra depuis gView

    float4 ambientColor = ComputeAmbient(ambientLightColor, materialAmbient, pin.Color);
    float3 diffuseColor = ComputeDiffuse(lightDirection, pin.NormalW, lightColor, materialDiffuse); // Utilisation de pin.NormalW normalisé
    float3 viewDirection = normalize(cameraPosition - pin.PosH.xyz); // Direction de la vue par rapport au fragment
    float3 specularColor = ComputeSpecular(lightDirection, pin.NormalW, viewDirection, lightColor, materialSpecular, materialShininess); // Utilisation de pin.NormalW normalisé

    // Combiner toutes les couleurs
    float3 finalColor = ambientColor.rgb + diffuseColor + specularColor;
    finalColor *= pin.Color.rgb;

    // Appliquer l'alpha pour la transparence
    float alpha = pin.Color.a;

    return float4(finalColor, alpha);
}
