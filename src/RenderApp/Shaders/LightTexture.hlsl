#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_SPOT 1
#define LIGHT_TYPE_POINT 2

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

cbuffer cbPerMaterial : register(b2)
{
    float4 cbPerMaterial_ambientLightColor;
    float4 cbPerMaterial_ambient;
    float4 cbPerMaterial_diffuse;
    float4 cbPerMaterial_specular;
    float cbPerMaterial_shininess;
    float cbPerMaterial_padding3[3];
};

struct Light
{
    float3 cbPerLight_position;
    float3 cbPerLight_direction;
    float3 cbPerLight_color;
    float cbPerLight_spotAngle;
    int cbPerLight_lightType;
    float cbPerLight_lightIntensity;
};

cbuffer cbPerLights : register(b3)
{
    Light lights[10];
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
    float3 WorldPos : POSITION; // Position in world space
};

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
    vout.UV = vin.UV;

    // Pass world position to the pixel shader
    vout.WorldPos = posW.xyz;

    return vout;
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

float ComputePointLightIntensity(float3 lightPosition, float3 surfacePosition, float lightIntensity)
{
    float distance = length(lightPosition - surfacePosition);
    float attenuation = 1.0f / (distance * distance); // Simple inverse square law for attenuation
    return lightIntensity * attenuation;
}


// Pixel shader
float4 PS(VertexOut pin) : SV_Target
{
    float4 texColor = g_texture.Sample(g_sampler, pin.UV);
    
    float3x3 invViewMatrix = (float3x3) transpose(gView); // Transpose of view matrix
    float3 cameraPosition = -mul(invViewMatrix, gView[3].xyz); // Camera position in world space
    float3 viewDirection = normalize(cameraPosition - pin.WorldPos); // View direction

    float4 finalColor = ComputeAmbient(cbPerMaterial_ambientLightColor, cbPerMaterial_ambient, texColor);

    float3 diffuseColor = float3(0.0f, 0.0f, 0.0f);
    float3 specularColor = float3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 10; ++i)
    {
        Light currentLight = lights[i];

        if (currentLight.cbPerLight_lightType == LIGHT_TYPE_DIRECTIONAL)
        {
            // Directional light calculations
            float3 lightColorDirectional = currentLight.cbPerLight_color * currentLight.cbPerLight_lightIntensity;
            float3 lightDirectionDirectional = -normalize(currentLight.cbPerLight_direction); // Invert direction for directional light

            float diffuseIntensity = dot(pin.NormalW, lightDirectionDirectional);
            diffuseIntensity = saturate(diffuseIntensity);

            diffuseColor += ComputeDiffuse(lightDirectionDirectional, pin.NormalW, lightColorDirectional, cbPerMaterial_diffuse) * diffuseIntensity;
            specularColor += ComputeBlinnPhongSpecular(lightDirectionDirectional, pin.NormalW, viewDirection, lightColorDirectional, cbPerMaterial_specular, cbPerMaterial_shininess);
        }
        else if (currentLight.cbPerLight_lightType == LIGHT_TYPE_SPOT)
        {
            // Spot light calculations
            float3 lightColorSpot = currentLight.cbPerLight_color * currentLight.cbPerLight_lightIntensity;
            float3 lightPositionSpot = currentLight.cbPerLight_position;
            float3 lightDirectionSpot = normalize(currentLight.cbPerLight_direction);
            float spotAngle = radians(currentLight.cbPerLight_spotAngle);

            float spotIntensity = ComputeSpotIntensity(lightPositionSpot, lightDirectionSpot, pin.WorldPos, spotAngle);

            diffuseColor += ComputeDiffuse(lightDirectionSpot, pin.NormalW, lightColorSpot, cbPerMaterial_diffuse) * spotIntensity;
            specularColor += ComputeBlinnPhongSpecular(lightDirectionSpot, pin.NormalW, viewDirection, lightColorSpot, cbPerMaterial_specular, cbPerMaterial_shininess) * spotIntensity;
        }
        else if (currentLight.cbPerLight_lightType == LIGHT_TYPE_POINT)
        {
            // Point light calculations
            float3 lightColorPoint = currentLight.cbPerLight_color;
            float3 lightPositionPoint = currentLight.cbPerLight_position;

            float pointLightIntensity = ComputePointLightIntensity(lightPositionPoint, pin.WorldPos, currentLight.cbPerLight_lightIntensity);

            float3 lightDirectionPoint = normalize(lightPositionPoint - pin.WorldPos);

            diffuseColor += ComputeDiffuse(lightDirectionPoint, pin.NormalW, lightColorPoint, cbPerMaterial_diffuse) * pointLightIntensity;
            specularColor += ComputePhongSpecular(lightDirectionPoint, pin.NormalW, viewDirection, lightColorPoint, cbPerMaterial_specular, cbPerMaterial_shininess) * pointLightIntensity;
        }
    }
    
    finalColor.rgb += diffuseColor * texColor.rgb;
    finalColor.rgb += specularColor * texColor.rgb;
    finalColor.a = texColor.a;

    return finalColor;
}




