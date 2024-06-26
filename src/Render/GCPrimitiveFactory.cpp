#include "pch.h"

GCPrimitiveFactory::GCPrimitiveFactory() 
{
	m_pRender = nullptr;
}

GCPrimitiveFactory::~GCPrimitiveFactory() 
{
}

std::vector<DirectX::XMFLOAT3> GCPrimitiveFactory::GenerateNormal(const std::vector<uint16_t>& index, const std::vector<DirectX::XMFLOAT3>& pos)
{
    std::vector <DirectX::XMFLOAT3> normals;
    for (int i = 0; i < index.size(); i+=2)
    {
        for (int i = 0; i < 3; i++)
        {
            normals.push_back(GCUtils::GetNormal(GCUtils::Xmfloat3ToGcvec3(pos[index[i]]), GCUtils::Xmfloat3ToGcvec3(pos[index[i + 1]]), GCUtils::Xmfloat3ToGcvec3(pos[index[i + 2]]), false));
        }
    }
    return normals;
}

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

void GCPrimitiveFactory::GenerateSphere(float radius, int numSegments, std::vector<DirectX::XMFLOAT3>& outVertices, std::vector<DirectX::XMFLOAT2>& outUvs, std::vector<uint16_t>& outIndices)
{
    outVertices.clear();
    outUvs.clear();
    outIndices.clear();

    // Generate sphere vertices and uvs (you can use a more refined method like icosphere subdivision)
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


void GCPrimitiveFactory::Initialize() 
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

    GenerateSphere(0.5f, 32, sphereVertices, sphereUvs, sphereIndices);

    //Put all data in map
    m_primitiveInfos = {
    // Only in 2d
    {L"plane", {
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
            { L"normals", GenerateNormal(std::vector<uint16_t>{0, 1, 2, 0, 2, 3}, std::vector<DirectX::XMFLOAT3>{
                DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f),
                DirectX::XMFLOAT3(-0.5f, 0.5f, 0.0f),
                DirectX::XMFLOAT3(0.5f, 0.5f, 0.0f),
                DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f) 
            })
            }
    }},

    {L"cube", {
        { L"index", std::vector<uint16_t>{
            0, 1, 2, 0, 2, 3,
            4, 6, 5, 4, 7, 6,
            4, 5, 1, 4, 1, 0,
            3, 2, 6, 3, 6, 7,
            1, 5, 6, 1, 6, 2,
            4, 0, 3, 4, 3, 7,
        }},
        { L"pos", std::vector<DirectX::XMFLOAT3>{
            DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f),
            DirectX::XMFLOAT3(-0.5f, +0.5f, -0.5f),
            DirectX::XMFLOAT3(+0.5f, +0.5f, -0.5f),
            DirectX::XMFLOAT3(+0.5f, -0.5f, -0.5f),
            DirectX::XMFLOAT3(-0.5f, -0.5f, +0.5f),
            DirectX::XMFLOAT3(-0.5f, +0.5f, +0.5f),
            DirectX::XMFLOAT3(+0.5f, +0.5f, +0.5f),
            DirectX::XMFLOAT3(+0.5f, -0.5f, +0.5f),
            DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f),
            DirectX::XMFLOAT3(-0.5f, +0.5f, -0.5f),
            DirectX::XMFLOAT3(-0.5f, +0.5f, +0.5f),
            DirectX::XMFLOAT3(-0.5f, -0.5f, +0.5f),
            DirectX::XMFLOAT3(+0.5f, -0.5f, -0.5f),
            DirectX::XMFLOAT3(+0.5f, +0.5f, -0.5f),
            DirectX::XMFLOAT3(+0.5f, +0.5f, +0.5f),
            DirectX::XMFLOAT3(+0.5f, -0.5f, +0.5f),
            DirectX::XMFLOAT3(-0.5f, +0.5f, -0.5f),
            DirectX::XMFLOAT3(-0.5f, +0.5f, +0.5f),
            DirectX::XMFLOAT3(+0.5f, +0.5f, +0.5f),
            DirectX::XMFLOAT3(+0.5f, +0.5f, -0.5f),
            DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f),
            DirectX::XMFLOAT3(-0.5f, -0.5f, +0.5f),
            DirectX::XMFLOAT3(+0.5f, -0.5f, +0.5f),
            DirectX::XMFLOAT3(+0.5f, -0.5f, -0.5f),
        }},
        { L"uvs", std::vector<DirectX::XMFLOAT2>{
            DirectX::XMFLOAT2(0, 1),
            DirectX::XMFLOAT2(0, 0),
            DirectX::XMFLOAT2(1, 0),
            DirectX::XMFLOAT2(1, 1),
            DirectX::XMFLOAT2(0, 1),
            DirectX::XMFLOAT2(0, 0),
            DirectX::XMFLOAT2(1, 0),
            DirectX::XMFLOAT2(1, 1),
            DirectX::XMFLOAT2(0, 1),
            DirectX::XMFLOAT2(0, 0),
            DirectX::XMFLOAT2(1, 0),
            DirectX::XMFLOAT2(1, 1),
            DirectX::XMFLOAT2(0, 1),
            DirectX::XMFLOAT2(0, 0),
            DirectX::XMFLOAT2(1, 0),
            DirectX::XMFLOAT2(1, 1),
            DirectX::XMFLOAT2(0, 1),
            DirectX::XMFLOAT2(0, 0),
            DirectX::XMFLOAT2(1, 0),
            DirectX::XMFLOAT2(1, 1),
            DirectX::XMFLOAT2(0, 1),
            DirectX::XMFLOAT2(0, 0),
            DirectX::XMFLOAT2(1, 0),
            DirectX::XMFLOAT2(1, 1),
        }},
        /*{ L"normals", GenerateNormal(std::vector<uint16_t>{
            0, 1, 2, 0, 2, 3,
            4, 6, 5, 4, 7, 6,
            4, 5, 1, 4, 1, 0,
            3, 2, 6, 3, 6, 7,
            1, 5, 6, 1, 6, 2,
            4, 0, 3, 4, 3, 7,
            }, std::vector<DirectX::XMFLOAT3>{
            DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f),
            DirectX::XMFLOAT3(-0.5f, +0.5f, -0.5f),
            DirectX::XMFLOAT3(+0.5f, +0.5f, -0.5f),
            DirectX::XMFLOAT3(+0.5f, -0.5f, -0.5f),
            DirectX::XMFLOAT3(-0.5f, -0.5f, +0.5f),
            DirectX::XMFLOAT3(-0.5f, +0.5f, +0.5f),
            DirectX::XMFLOAT3(+0.5f, +0.5f, +0.5f),
            DirectX::XMFLOAT3(+0.5f, -0.5f, +0.5f),
            DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f),
            DirectX::XMFLOAT3(-0.5f, +0.5f, -0.5f),
            DirectX::XMFLOAT3(-0.5f, +0.5f, +0.5f),
            DirectX::XMFLOAT3(-0.5f, -0.5f, +0.5f),
            DirectX::XMFLOAT3(+0.5f, -0.5f, -0.5f),
            DirectX::XMFLOAT3(+0.5f, +0.5f, -0.5f),
            DirectX::XMFLOAT3(+0.5f, +0.5f, +0.5f),
            DirectX::XMFLOAT3(+0.5f, -0.5f, +0.5f),
            DirectX::XMFLOAT3(-0.5f, +0.5f, -0.5f),
            DirectX::XMFLOAT3(-0.5f, +0.5f, +0.5f),
            DirectX::XMFLOAT3(+0.5f, +0.5f, +0.5f),
            DirectX::XMFLOAT3(+0.5f, +0.5f, -0.5f),
            DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f),
            DirectX::XMFLOAT3(-0.5f, -0.5f, +0.5f),
            DirectX::XMFLOAT3(+0.5f, -0.5f, +0.5f),
            DirectX::XMFLOAT3(+0.5f, -0.5f, -0.5f),})
        }*/
        { L"normals", std::vector<DirectX::XMFLOAT3>{
            // Face -Z
            DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f), // Normal for vertex 0
            DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f), // Normal for vertex 1
            DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f), // Normal for vertex 2
            DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f), // Normal for vertex 3
            // Face +Z
            DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), // Normal for vertex 4
            DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), // Normal for vertex 5
            DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), // Normal for vertex 6
            DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), // Normal for vertex 7
            // Face -X
            DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f), // Normal for vertex 8
            DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f), // Normal for vertex 9
            DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f), // Normal for vertex 10
            DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f), // Normal for vertex 11
            // Face +X
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), // Normal for vertex 12
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), // Normal for vertex 13
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), // Normal for vertex 14
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), // Normal for vertex 15
            // Face +Y
            DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), // Normal for vertex 16
            DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), // Normal for vertex 17
            DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), // Normal for vertex 18
            DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), // Normal for vertex 19
            // Face -Y
            DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f), // Normal for vertex 20
            DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f), // Normal for vertex 21
            DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f), // Normal for vertex 22
            DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f), // Normal for vertex 23
        }}

    }},
    // #TODO -> Doesn't work in 2d thereas, it need be 2d
    {L"circle", {
        {L"index", circleIndices},
        {L"pos", circleVertices},
        {L"uvs", circleUvs},
        //{L"normals", GenerateNormal(circleIndices, circleVertices)}
    }},
    {L"sphere", {
        {L"index", sphereIndices},
        {L"pos", sphereVertices},
        {L"uvs", sphereUvs},
        //{L"normals", GenerateNormal(sphereIndices, sphereVertices)}
    }}
    };
}


GCGeometry* GCPrimitiveFactory::BuildGeometry(std::string name, DirectX::XMFLOAT4 color, int& flagEnabledBits)
{
    //Builds a texture based geometry using pre-created ones
    //Needs a geometry name
    std::wstring wName(name.begin(), name.end());

    auto it = m_primitiveInfos.find(wName);
    /*if (it == m_primitiveInfos.end())
    {
        std::wstring warningMsg = L"Primitive not found: " + wName + L"\n";
        OutputDebugString(warningMsg.c_str());
        profiler.LogWarning("Primitive not found: " + std::string(name.begin(), name.end()));
    }
    else
    {
        std::wstring successMsg = L"Primitive: " + wName + L" loaded successfully\n";
        OutputDebugString(successMsg.c_str());
        profiler.LogInfo("Primitive: " + std::string(name.begin(), name.end()) + " loaded successfully");
    }*/

	GCGeometry* primitiveGeometry = new GCGeometry();

	primitiveGeometry->indices = std::get<std::vector<uint16_t>>(m_primitiveInfos[wName][L"index"]);
	primitiveGeometry->indiceNumber = std::get<std::vector<uint16_t>>(m_primitiveInfos[wName][L"index"]).size();

    if (HAS_FLAG(flagEnabledBits, HAS_POSITION)) {
        primitiveGeometry->pos = std::get<std::vector<DirectX::XMFLOAT3>>(m_primitiveInfos[wName][L"pos"]);
        primitiveGeometry->vertexNumber = std::get<std::vector<DirectX::XMFLOAT3>>(m_primitiveInfos[wName][L"pos"]).size();
    }

    if (HAS_FLAG(flagEnabledBits, HAS_COLOR)) {
        for (int i = 0; i < primitiveGeometry->vertexNumber; i++)
            primitiveGeometry->color.push_back(color);
    }

    if (HAS_FLAG(flagEnabledBits, HAS_UV)) {
        primitiveGeometry->uv = std::get<std::vector<DirectX::XMFLOAT2>>(m_primitiveInfos[wName][L"uvs"]);
    }

    if (HAS_FLAG(flagEnabledBits, HAS_NORMAL)) {
        primitiveGeometry->normals = std::get<std::vector<DirectX::XMFLOAT3>>(m_primitiveInfos[wName][L"normals"]);
    }

    primitiveGeometry->m_flagEnabledBits = flagEnabledBits;


    CHECK_POINTERSNULL("Primitive Geometry built successfully", "Primitive geometry is empty", primitiveGeometry);

	return primitiveGeometry;
}