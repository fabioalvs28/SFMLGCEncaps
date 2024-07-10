#include "pch.h"

using namespace DirectX;

GCPrimitiveFactory::GCPrimitiveFactory() 
{
	m_pRender = nullptr;
}

GCPrimitiveFactory::~GCPrimitiveFactory() 
{
}

//std::vector<DirectX::XMFLOAT3> GCPrimitiveFactory::GenerateNormal(const std::vector<uint16_t>& index, const std::vector<DirectX::XMFLOAT3>& pos)
//{
//    std::vector <DirectX::XMFLOAT3> normals;
//    for (int i = 0; i < index.size(); i+=2)
//    {
//        for (int i = 0; i < 3; i++)
//        {
//            normals.push_back(GCUtils::GetNormal(GCUtils::Xmfloat3ToGcvec3(pos[index[i]]), GCUtils::Xmfloat3ToGcvec3(pos[index[i + 1]]), GCUtils::Xmfloat3ToGcvec3(pos[index[i + 2]]), false));
//        }
//    }
//    return normals;
//}

void GCPrimitiveFactory::GenerateCircle(float radius, int numSegments, std::vector<DirectX::XMFLOAT3>& outVertices, std::vector<DirectX::XMFLOAT2>& outUvs, std::vector<uint16_t>& outIndices)
{
    outVertices.clear();
    outUvs.clear();
    outIndices.clear();

    float angleStep = DirectX::XM_2PI / numSegments;
    float angle = 0.0f;

    // Add center vertex
    outVertices.push_back(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
    outUvs.push_back(DirectX::XMFLOAT2(0.5f, 0.5f)); // Center UV

    // Add vertices around the circumference
    for (int i = 0; i < numSegments; ++i)
    {
        float x = radius * std::cos(angle);
        float y = radius * std::sin(angle);
        outVertices.push_back(DirectX::XMFLOAT3(x, 0.0f, y));

        // Map vertices to UV coordinates
        float u = (x / radius) * 0.5f + 0.5f;
        float v = (-y / radius) * 0.5f + 0.5f;
        outUvs.push_back(DirectX::XMFLOAT2(u, v));

        angle += angleStep;
    }

    // Add indices to form triangles with reversed normals
    for (int i = 1; i < numSegments; ++i)
    {
        outIndices.push_back(0);        // Center vertex
        outIndices.push_back(i);        // Current vertex
        outIndices.push_back(i + 1);    // Next vertex
    }
    // Connect last vertex to the first vertex
    outIndices.push_back(0);
    outIndices.push_back(numSegments);
    outIndices.push_back(1);
}

void GCPrimitiveFactory::GenerateSphere(float radius, int numSegments, std::vector<DirectX::XMFLOAT3>& outVertices, std::vector<DirectX::XMFLOAT2>& outUvs, std::vector<uint16_t>& outIndices, std::vector<DirectX::XMFLOAT3>& outNormals)
{
    outVertices.clear();
    outUvs.clear();
    outIndices.clear();
    outNormals.clear();

    // Generate sphere vertices, uvs, and normals
    for (int i = 0; i <= numSegments; ++i)
    {
        float phi = DirectX::XM_PI * i / numSegments;  // azimuth angle
        for (int j = 0; j <= numSegments; ++j)
        {
            float theta = DirectX::XM_2PI * j / numSegments;  // polar angle
            float x = radius * std::sin(phi) * std::cos(theta);
            float y = radius * std::cos(phi);
            float z = radius * std::sin(phi) * std::sin(theta);
            outVertices.push_back(DirectX::XMFLOAT3(x, y, z));

            float u = 1.0f - (j / (float)numSegments);  // horizontal texture coordinate
            float v = 1.0f - (i / (float)numSegments);  // vertical texture coordinate
            outUvs.push_back(DirectX::XMFLOAT2(u, v));

            // Calculate normal (normalized vertex position)
            DirectX::XMFLOAT3 normal(x, y, z);
            DirectX::XMVECTOR normalVec = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&normal));
            DirectX::XMStoreFloat3(&normal, normalVec);
            outNormals.push_back(normal);
        }
    }

    // Generate sphere indices
    for (int i = 0; i < numSegments; ++i)
    {
        for (int j = 0; j < numSegments; ++j)
        {
            int index = i * (numSegments + 1) + j;
            outIndices.push_back(index);
            outIndices.push_back(index + 1);
            outIndices.push_back(index + numSegments + 1);

            outIndices.push_back(index + 1);
            outIndices.push_back(index + numSegments + 2);
            outIndices.push_back(index + numSegments + 1);
        }
    }
}

void GCPrimitiveFactory::GenerateCube(std::vector<DirectX::XMFLOAT3>& vertices,
    std::vector<uint16_t>& indices,
    std::vector<DirectX::XMFLOAT2>& uvs,
    std::vector<DirectX::XMFLOAT3>& normals)
{
    // Define 24 vertices of the cube (each vertex is unique)
    DirectX::XMFLOAT3 positions[24] = {
        // Front face
        { -0.5f, -0.5f, -0.5f }, // 0
        {  0.5f, -0.5f, -0.5f }, // 1
        {  0.5f,  0.5f, -0.5f }, // 2
        { -0.5f,  0.5f, -0.5f }, // 3

        // Right face
        {  0.5f, -0.5f, -0.5f }, // 4
        {  0.5f, -0.5f,  0.5f }, // 5
        {  0.5f,  0.5f,  0.5f }, // 6
        {  0.5f,  0.5f, -0.5f }, // 7

        // Back face
        {  0.5f, -0.5f,  0.5f }, // 8
        { -0.5f, -0.5f,  0.5f }, // 9
        { -0.5f,  0.5f,  0.5f }, // 10
        {  0.5f,  0.5f,  0.5f }, // 11

        // Left face
        { -0.5f, -0.5f,  0.5f }, // 12
        { -0.5f, -0.5f, -0.5f }, // 13
        { -0.5f,  0.5f, -0.5f }, // 14
        { -0.5f,  0.5f,  0.5f }, // 15

        // Top face
        { -0.5f,  0.5f, -0.5f }, // 16
        {  0.5f,  0.5f, -0.5f }, // 17
        {  0.5f,  0.5f,  0.5f }, // 18
        { -0.5f,  0.5f,  0.5f }, // 19

        // Bottom face
        { -0.5f, -0.5f,  0.5f }, // 20
        {  0.5f, -0.5f,  0.5f }, // 21
        {  0.5f, -0.5f, -0.5f }, // 22
        { -0.5f, -0.5f, -0.5f }  // 23
    };

    // Define indices (triangles) for each face with back face culling
    uint16_t cubeIndices[36] = {
        0,  1,  2,  0,  2,  3, // Front face
        4,  5,  6,  4,  6,  7, // Right face
        8,  9, 10,  8, 10, 11, // Back face
       12, 13, 14, 12, 14, 15, // Left face
       16, 17, 18, 16, 18, 19, // Top face
       20, 21, 22, 20, 22, 23  // Bottom face
    };

    // Define UVs for each vertex (24 vertices)
    DirectX::XMFLOAT2 cubeUVs[24] = {
        // Front face
        { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f },
        // Right face
        { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f },
        // Back face
        { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f },
        // Left face
        { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f },
        // Top face
        { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f },
        // Bottom face
        { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f }
    };

    // Define normals for each vertex (24 vertices)
    DirectX::XMFLOAT3 cubeNormals[24] = {
        // Front face
        { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f },
        // Right face
        { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f },
        // Back face
        { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f },
        // Left face
        { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f },
        // Top face
        { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
        // Bottom face
        { 0.0f, -1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }
    };

    // Copy vertices, indices, UVs, and normals into the provided vectors
    vertices.assign(positions, positions + 24);
    indices.assign(cubeIndices, cubeIndices + 36);
    uvs.assign(cubeUVs, cubeUVs + 24); // Assign 24 UVs
    normals.assign(cubeNormals, cubeNormals + 24); // Assign 24 normals

    // Adjust face winding for back face culling (clockwise)
    for (size_t i = 0; i < indices.size(); i += 3) {
        std::swap(indices[i], indices[i + 2]);
    }
}

void GCPrimitiveFactory::GenerateCubeSkybox(std::vector<DirectX::XMFLOAT3>& vertices,
    std::vector<uint16_t>& indices,
    std::vector<DirectX::XMFLOAT2>& uvs,
    std::vector<DirectX::XMFLOAT3>& normals)
{
    DirectX::XMFLOAT3 positions[24] = {
        // Front face
        { -0.5f, -0.5f, -0.5f }, // 0
        {  0.5f, -0.5f, -0.5f }, // 1
        {  0.5f,  0.5f, -0.5f }, // 2
        { -0.5f,  0.5f, -0.5f }, // 3

        // Right face
        {  0.5f, -0.5f, -0.5f }, // 4
        {  0.5f, -0.5f,  0.5f }, // 5
        {  0.5f,  0.5f,  0.5f }, // 6
        {  0.5f,  0.5f, -0.5f }, // 7

        // Back face
        {  0.5f, -0.5f,  0.5f }, // 8
        { -0.5f, -0.5f,  0.5f }, // 9
        { -0.5f,  0.5f,  0.5f }, // 10
        {  0.5f,  0.5f,  0.5f }, // 11

        // Left face
        { -0.5f, -0.5f,  0.5f }, // 12
        { -0.5f, -0.5f, -0.5f }, // 13
        { -0.5f,  0.5f, -0.5f }, // 14
        { -0.5f,  0.5f,  0.5f }, // 15

        // Top face
        { -0.5f,  0.5f, -0.5f }, // 16
        {  0.5f,  0.5f, -0.5f }, // 17
        {  0.5f,  0.5f,  0.5f }, // 18
        { -0.5f,  0.5f,  0.5f }, // 19

        // Bottom face
        { -0.5f, -0.5f,  0.5f }, // 20
        {  0.5f, -0.5f,  0.5f }, // 21
        {  0.5f, -0.5f, -0.5f }, // 22
        { -0.5f, -0.5f, -0.5f }  // 23
    };

    // Define indices (triangles) for each face with back face culling (clockwise)
    uint16_t cubeIndices[36] = {
        0,  2,  1,  0,  3,  2, // Front face
        4,  6,  5,  4,  7,  6, // Right face
        8, 10,  9,  8, 11, 10, // Back face
       12, 14, 13, 12, 15, 14, // Left face
       16, 18, 17, 16, 19, 18, // Top face
       20, 22, 21, 20, 23, 22  // Bottom face
    };

    // Define UVs for each vertex (24 vertices)
    DirectX::XMFLOAT2 cubeUVs[24] = {
        // Front face
        { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f },
        // Right face
        { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f },
        // Back face
        { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f },
        // Left face
        { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f },
        // Top face
        { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f },
        // Bottom face
        { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f }
    };

    // Define normals for each vertex (24 vertices)
    DirectX::XMFLOAT3 cubeNormals[24] = {
        // Front face
        { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f },
        // Right face
        { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f },
        // Back face
        { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f },
        // Left face
        { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f },
        // Top face
        { 0.0f, -1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f },
        // Bottom face
        { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }
    };

    // Copy vertices, indices, UVs, and normals into the provided vectors
    vertices.assign(positions, positions + 24);
    indices.assign(cubeIndices, cubeIndices + 36);
    uvs.assign(cubeUVs, cubeUVs + 24); // Assign 24 UVs
    normals.assign(cubeNormals, cubeNormals + 24); // Assign 24 normals
}

bool GCPrimitiveFactory::Initialize() 
{
    // Create circle vertices, uvs and indices
    std::vector<DirectX::XMFLOAT3> circleVertices;
    std::vector<DirectX::XMFLOAT2> circleUvs;
    std::vector<uint16_t> circleIndices;

    GenerateCircle(0.5f, 32, circleVertices, circleUvs, circleIndices);

    // Create sphere vertices, uvs and indices
    std::vector<DirectX::XMFLOAT3> sphereVertices;
    std::vector<DirectX::XMFLOAT2> sphereUvs;
    std::vector<uint16_t> sphereIndices;
    std::vector<DirectX::XMFLOAT3> sphereNormals;

    GenerateSphere(0.5f, 32, sphereVertices, sphereUvs, sphereIndices, sphereNormals);

    std::vector<DirectX::XMFLOAT3> cubeVertices;
    std::vector<DirectX::XMFLOAT2> cubeUvs;
    std::vector<uint16_t> cubeIndices;
    std::vector<DirectX::XMFLOAT3> cubeNormals;

    GenerateCube(cubeVertices, cubeIndices, cubeUvs, cubeNormals);

    std::vector<DirectX::XMFLOAT3> cubeSkyboxVertices;
    std::vector<DirectX::XMFLOAT2> cubeSkyboxUvs;
    std::vector<uint16_t> cubeSkyboxIndices;
    std::vector<DirectX::XMFLOAT3> cubeSkyboxNormals;

    GenerateCubeSkybox(cubeSkyboxVertices, cubeSkyboxIndices, cubeSkyboxUvs, cubeSkyboxNormals);

    //Put all data in map
    m_primitiveInfos = 
    {
        // Only in 2d
        { //plane
            { L"index", std::vector<uint16_t>{0, 1, 2, 0, 2, 3} },
            { L"pos", std::vector<DirectX::XMFLOAT3>{
                DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f),
                DirectX::XMFLOAT3(-0.5f, 0.5f, 0.0f),
                DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f),
                DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f)
            }},
            { L"uvs", std::vector<DirectX::XMFLOAT2>{
                DirectX::XMFLOAT2(0.0f, 1.0f),
                DirectX::XMFLOAT2(0.0f, 0.0f),
                DirectX::XMFLOAT2(1.0f, 0.0f),
                DirectX::XMFLOAT2(1.0f, 1.0f)
            }},
            { L"normals", std::vector<DirectX::XMFLOAT3>{
                DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), // Normal for Bottom-left
                DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), // Normal for Bottom-right
                DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), // Normal for Top-right
                DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), // Normal for Top-left
            }},
        },

        { //cube
            {L"index", cubeIndices},
            {L"pos", cubeVertices},
            {L"uvs", cubeUvs},
            {L"normals", cubeNormals},
        },

        { //cube skybox
            {L"index", cubeSkyboxIndices},
            {L"pos", cubeSkyboxVertices},
            {L"uvs", cubeSkyboxUvs},
            {L"normals", cubeSkyboxNormals},
        },
        // #TODO -> Doesn't work in 2d thereas, it need be 2d
        { //circle
            {L"index", circleIndices},
            {L"pos", circleVertices},
            {L"uvs", circleUvs},
        },
        { //Sphere
            {L"index", sphereIndices},
            {L"pos", sphereVertices},
            {L"uvs", sphereUvs},
            {L"normals", sphereNormals},
        },
        { // Quad
            {L"index", std::vector<uint16_t>{0, 3, 1, 2, 3, 0}}, // Indices pour former deux triangles
            {L"pos", std::vector<DirectX::XMFLOAT3>{
                DirectX::XMFLOAT3(-1.0f,  1.0f, 0.0f),  // Top-left
                DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f),  // Bottom-left
                DirectX::XMFLOAT3(1.0f,  1.0f, 0.0f),   // Top-right
                DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f)    // Bottom-right
            }},
            {L"uvs", std::vector<DirectX::XMFLOAT2>{
                DirectX::XMFLOAT2(0.0f, 0.0f),  // Top-left
                DirectX::XMFLOAT2(0.0f, 1.0f),  // Bottom-left
                DirectX::XMFLOAT2(1.0f, 0.0f),   // Top-right
                DirectX::XMFLOAT2(1.0f, 1.0f),  // Bottom-right
            }},
            {L"normals", std::vector<DirectX::XMFLOAT3>{
                DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), // Normal for Top-left
                DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), // Normal for Bottom-left
                DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), // Normal for Bottom-right
                DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)  // Normal for Top-right
            }},
        },
    };

    return true;
}


GC_GRAPHICS_ERROR GCPrimitiveFactory::BuildGeometry(GC_PRIMITIVE_ID index, DirectX::XMFLOAT4 color, GCGeometry* pGeometry)
{
    //Builds a texture based geometry using pre-created ones
    //Needs a geometry name
    if (!CHECK_POINTERSNULL("Primitive Geometry built successfully", "Primitive geometry is empty", pGeometry))
        return GCRENDER_ERROR_POINTER_NULL;

	pGeometry->indices = std::get<std::vector<uint16_t>>(m_primitiveInfos[index][L"index"]);
	pGeometry->indiceNumber = std::get<std::vector<uint16_t>>(m_primitiveInfos[index][L"index"]).size();

    pGeometry->pos = std::get<std::vector<DirectX::XMFLOAT3>>(m_primitiveInfos[index][L"pos"]);
    pGeometry->vertexNumber = std::get<std::vector<DirectX::XMFLOAT3>>(m_primitiveInfos[index][L"pos"]).size();

    for (int i = 0; i < pGeometry->vertexNumber; i++)
    pGeometry->color.push_back(color);

    pGeometry->uv = std::get<std::vector<DirectX::XMFLOAT2>>(m_primitiveInfos[index][L"uvs"]);

    pGeometry->normals = std::get<std::vector<DirectX::XMFLOAT3>>(m_primitiveInfos[index][L"normals"]);

	return GCRENDER_SUCCESS_OK;
}