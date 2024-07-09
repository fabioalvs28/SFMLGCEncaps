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
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float4 Color : COLOR;
};

float4x4 TransposeMatrix(float4x4 mat)
{
    float4x4 result;

    result[0] = float4(mat[0].x, mat[1].x, mat[2].x, mat[3].x);
    result[1] = float4(mat[0].y, mat[1].y, mat[2].y, mat[3].y);
    result[2] = float4(mat[0].z, mat[1].z, mat[2].z, mat[3].z);
    result[3] = float4(mat[0].w, mat[1].w, mat[2].w, mat[3].w);

    return result;
}

// Vertex shader
VertexOut VS(VertexIn vin)
{
    float4x4 gWorldTransposed = TransposeMatrix(gWorld);
    
    VertexOut vout;

    // Transform position to homogeneous clip space using gWorld, gView, and gProj matrices.
    vout.PosH = mul(mul(mul(float4(vin.PosL, 1.0f), gWorldTransposed), gView), gProj);

    // Pass vertex color to the pixel shader.
    vout.Color = vin.Color;

    return vout;
}

// Pixel shader
float4 PS(VertexOut pin) : SV_Target
{
    // Color & Alpha
    return pin.Color;
}
