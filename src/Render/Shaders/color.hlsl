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
    //float3 Normal : NORMAL;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float4 Color : COLOR;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
    
    // Transform position to homogeneous clip space using gWorld, gView, and gProj matrices.
    vout.PosH = mul(mul(float4(vin.PosL, 1.0f), gWorld), mul(gView, gProj));
    
    // Pass vertex color to the pixel shader.
    vout.Color = vin.Color;
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    /*
    float4 diffuseAlbedo = float4(1.0f, 0.0f, 1.0f, 1.0f); // Diffuse albedo
    float3 fresnelR0 = float3(0.1f, 1.0f, 0.1f); // Fresnel reflectance at normal incidence
    float roughness = 0.0f; // Roughness
    // Light properties
    float3 lightDirection = normalize(float3(1.0f, 1.0f, 1.0f)); // Example light direction
    float3 lightColor = float3(1.0f, 1.0f, 1.0f); // Example light color (white)

    // Use material properties for lighting and shading calculations
    float3 viewDirection = normalize(float3(0.0f, 0.0f, 1.0f)); // Example view direction (towards the camera)
    
    // Diffuse calculation
    float diffuseFactor = max(0.0f, dot(normalize(diffuseAlbedo.rgb), lightDirection));
    float3 diffuseColor = diffuseAlbedo.rgb * diffuseFactor * lightColor;
    
    // Specular calculation using roughness
    float3 halfVector = normalize(lightDirection + viewDirection);
    float specularFactor = pow(max(0.0f, dot(normalize(pin.Color.rgb), halfVector)), 1.0f / max(roughness, 0.0001f)); // Clamp roughness to avoid division by zero
    float3 specularColor = fresnelR0 * specularFactor * lightColor;
    
    // Combine diffuse and specular components
    float3 finalColor = diffuseColor + specularColor;

    return float4(finalColor, diffuseAlbedo.a); // Use alpha from diffuse albedo
    */
    return pin.Color;


    /*//bool isMatching = (gWorld[3][0] == 3.0f);

    //if (isMatching)
    //{
    //    return float4(1.0f, 0.0f, 0.0f, 1.0f);
    //}
    //else
    //{
    //    return pin.Color;
    //}*/
}

