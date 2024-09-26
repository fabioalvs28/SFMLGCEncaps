float4x4 TransposeMatrix(float4x4 mat)
{
    float4x4 result;

    result[0] = float4(mat[0].x, mat[1].x, mat[2].x, mat[3].x);
    result[1] = float4(mat[0].y, mat[1].y, mat[2].y, mat[3].y);
    result[2] = float4(mat[0].z, mat[1].z, mat[2].z, mat[3].z);
    result[3] = float4(mat[0].w, mat[1].w, mat[2].w, mat[3].w);

    return result;
}

float4x4 InverseMatrix(float4x4 m)
{
    float n11 = m[0][0], n12 = m[1][0], n13 = m[2][0], n14 = m[3][0];
    float n21 = m[0][1], n22 = m[1][1], n23 = m[2][1], n24 = m[3][1];
    float n31 = m[0][2], n32 = m[1][2], n33 = m[2][2], n34 = m[3][2];
    float n41 = m[0][3], n42 = m[1][3], n43 = m[2][3], n44 = m[3][3];

    float t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
    float t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
    float t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
    float t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

    float det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;
    float idet = 1.0f / det;

    float4x4 ret;

    ret[0][0] = t11 * idet;
    ret[0][1] = (n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 + n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44) * idet;
    ret[0][2] = (n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 - n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44) * idet;
    ret[0][3] = (n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 + n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43) * idet;

    ret[1][0] = t12 * idet;
    ret[1][1] = (n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 - n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44) * idet;
    ret[1][2] = (n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 + n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44) * idet;
    ret[1][3] = (n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 - n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43) * idet;

    ret[2][0] = t13 * idet;
    ret[2][1] = (n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 + n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44) * idet;
    ret[2][2] = (n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 - n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44) * idet;
    ret[2][3] = (n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 + n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43) * idet;

    ret[3][0] = t14 * idet;
    ret[3][1] = (n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 - n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34) * idet;
    ret[3][2] = (n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 + n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34) * idet;
    ret[3][3] = (n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 - n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33) * idet;

    return ret;
}

// Function to calculate ambient color
float4 ComputeAmbient(float4 ambientLightColor, float4 materialAmbient, float4 vertexColor)
{
    return ambientLightColor * materialAmbient * vertexColor;
}

// Function to calculate diffuse color
float3 ComputeDiffuse(float3 lightDirection, float3 normal, float3 lightColor, float4 materialDiffuse)
{
    float3 normalizedNormal = normalize(normal); // Normalize normal
    float diffuseFactor = max(0.0f, dot(normalizedNormal, lightDirection));
    return lightColor * materialDiffuse.rgb * diffuseFactor;
}

// Function to calculate specular color using Phong model
float3 ComputePhongSpecular(float3 lightDirection, float3 normal, float3 viewDirection, float3 lightColor, float4 materialSpecular, float shininess)
{
    float3 reflectDirection = reflect(-lightDirection, normal); // Calculate reflection vector
    float specularFactor = pow(max(dot(viewDirection, reflectDirection), 0.0f), shininess); // Calculate specular factor
    return lightColor * materialSpecular.rgb * specularFactor; // Final specular color
}

// Function to calculate specular color using Blinn-Phong model
float3 ComputeBlinnPhongSpecular(float3 lightDirection, float3 normal, float3 viewDirection, float3 lightColor, float4 materialSpecular, float shininess)
{
    float3 halfwayDir = normalize(lightDirection + viewDirection); // Calculate "halfway" vector
    float specularFactor = pow(max(dot(normal, halfwayDir), 0.0f), shininess); // Calculate specular factor
    return lightColor * materialSpecular.rgb * specularFactor; // Final specular color
}

// Function to calculate spotlight intensity based on cone angle
float ComputeSpotIntensity(float3 lightPosition, float3 lightDirection, float3 surfacePosition, float spotAngle)
{
    float3 toLight = normalize(lightPosition - surfacePosition);
    float cosAngle = dot(-toLight, lightDirection);
    float minCos = cos(spotAngle);
    float maxCos = (minCos + 1.0f) / 2.0f;
    return saturate((cosAngle - minCos) / (maxCos - minCos));
}

// Function to calculate point light intensity based on distance attenuation
float ComputePointLightIntensity(float3 lightPosition, float3 surfacePosition, float lightIntensity)
{
    float distance = length(lightPosition - surfacePosition);
    float attenuation = 1.0f / (distance * distance); // Simple inverse square law for attenuation
    return lightIntensity * attenuation;
}

//float2 ReconstructWorldPosition(float2 screenPos, float4x4 projectionMatrix)
//{
//    // Convertir les coordonnées d'écran (0 à 1) en coordonnées de clip (-1 à 1)
//    float4 clipPos = float4(screenPos * 2.0f - 1.0f, 1.0f, 1.0f); // Utiliser profondeur 1.0 comme valeur par défaut

//    // Passer des coordonnées de clip à l'espace de projection
//    float4x4 invProj = InverseMatrix(projectionMatrix);
//    float4 projPos = mul(invProj, clipPos);
    
//    // Normaliser les coordonnées pour obtenir la position en espace de l'écran
//    float2 screenPos2D = projPos.xy / projPos.w;

//    // Retourner les coordonnées (x, y) en espace de l'écran
//    return screenPos2D;
//}

float3 ReconstructWorldPosition(float2 screenPos, float depth, float4x4 viewMatrix, float4x4 projectionMatrix)
{
    float4 clipPos = float4(screenPos * 2.0f - 1.0f, depth, 1.0f);

    // to view space
    float4 viewPos = mul(InverseMatrix(projectionMatrix), clipPos);
    viewPos /= viewPos.w;

    // to world space
    float4 worldPos = mul(InverseMatrix(viewMatrix), viewPos);

    return worldPos.xyz;
}