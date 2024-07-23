float4x4 TransposeMatrix(float4x4 mat)
{
    float4x4 result;

    result[0] = float4(mat[0].x, mat[1].x, mat[2].x, mat[3].x);
    result[1] = float4(mat[0].y, mat[1].y, mat[2].y, mat[3].y);
    result[2] = float4(mat[0].z, mat[1].z, mat[2].z, mat[3].z);
    result[3] = float4(mat[0].w, mat[1].w, mat[2].w, mat[3].w);

    return result;
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