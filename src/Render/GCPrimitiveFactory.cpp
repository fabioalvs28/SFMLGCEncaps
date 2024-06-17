#include "framework.h"

GCPrimitiveFactory::GCPrimitiveFactory() 
{
	m_pRender = nullptr;
}

GCPrimitiveFactory::~GCPrimitiveFactory() 
{
}


void GCPrimitiveFactory::Initialize() 
{
    //Initializes the geometries that will then be used by both the create build geometry functions

    // Create circle vertices, uvs and indices
    std::vector<DirectX::XMFLOAT3> circleVertices;
    std::vector<DirectX::XMFLOAT2> circleUvs;
    std::vector<uint16_t> circleIndices;

    float radius = 0.5f;
    int numSegments = 32;

    circleVertices.clear();
    circleUvs.clear();
    circleIndices.clear();

    float angleStep = DirectX::XM_2PI / numSegments;
    float angle = 0.0f;

    // Add center vertex
    circleVertices.push_back(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
    circleUvs.push_back(DirectX::XMFLOAT2(0.5f, 0.5f)); // Center UV

    // Add vertices around the circumference
    for (int i = 0; i < numSegments; ++i) 
    {
        float x = radius * std::cos(angle);
        float y = radius * std::sin(angle);
        circleVertices.push_back(DirectX::XMFLOAT3(x, 0.0f, y));

        // Map vertices to UV coordinates
        float u = (x / radius) * 0.5f + 0.5f;
        float v = (-y / radius) * 0.5f + 0.5f;
        circleUvs.push_back(DirectX::XMFLOAT2(u, v));

        angle += angleStep;
    }

    // Add indices to form triangles with reversed normals
    for (int i = 1; i < numSegments; ++i) 
    {
        circleIndices.push_back(0);        // Center vertex
        circleIndices.push_back(i);        // Current vertex
        circleIndices.push_back(i + 1);    // Next vertex
    }
    // Connect last vertex to the first vertex
    circleIndices.push_back(0);
    circleIndices.push_back(numSegments);
    circleIndices.push_back(1);

    //Put all data in map
    m_primitiveInfos = {
    {L"plane", {
        { L"index", std::vector<uint16_t>{0, 2, 1, 0, 3, 2} },
        { L"pos", std::vector<DirectX::XMFLOAT3>{
            DirectX::XMFLOAT3(-0.5f, +0.0f, -0.5f),
            DirectX::XMFLOAT3(-0.5f, +0.0f, +0.5f),
            DirectX::XMFLOAT3(+0.5f, +0.0f, +0.5f),
            DirectX::XMFLOAT3(+0.5f, +0.0f, -0.5f)
        }},
        { L"uvs", std::vector<DirectX::XMFLOAT2>{
            DirectX::XMFLOAT2(0, 0),
            DirectX::XMFLOAT2(0, 1),
            DirectX::XMFLOAT2(1, 1),
            DirectX::XMFLOAT2(1, 0)
        }}
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
        }}
    }},

    {L"circle", {
        {L"index", circleIndices},
        {L"pos", circleVertices},
        {L"uvs", circleUvs}
    }},
    };
}

GCGeometry* GCPrimitiveFactory::BuildGeometryColor(std::wstring name, DirectX::XMFLOAT4 color)
{
    //Builds a color based geometry using pre-created ones
    //Needs both a geometry name and a specific color
    auto it = m_primitiveInfos.find(name);
    if (it == m_primitiveInfos.end())
    {
        OutputDebugString((L"Primitive not found: " + name + L"\n").c_str());
        return NULL;
    }

	GCGeometry* primitiveGeometry = new GCGeometry();

	primitiveGeometry->indices = std::get<std::vector<uint16_t>>(m_primitiveInfos[name][L"index"]);
	primitiveGeometry->indiceNumber = std::get<std::vector<uint16_t>>(m_primitiveInfos[name][L"index"]).size();
	

	primitiveGeometry->pos = std::get<std::vector<DirectX::XMFLOAT3>>(m_primitiveInfos[name][L"pos"]);
	primitiveGeometry->vertexNumber = std::get<std::vector<DirectX::XMFLOAT3>>(m_primitiveInfos[name][L"pos"]).size();

    for(int i = 0 ; i<primitiveGeometry->vertexNumber; i++)
	    primitiveGeometry->color.push_back(color);

	return primitiveGeometry;
}

GCGeometry* GCPrimitiveFactory::BuildGeometryTexture(std::wstring name)
{
    //Builds a texture based geometry using pre-created ones
    //Needs a geometry name
    auto it = m_primitiveInfos.find(name);
    if (it == m_primitiveInfos.end())
    {
        OutputDebugString((L"Primitive not found: " + name + L"\n").c_str());
        return NULL;
    }

	GCGeometry* primitiveGeometry = new GCGeometry();

	primitiveGeometry->indices = std::get<std::vector<uint16_t>>(m_primitiveInfos[name][L"index"]);
	primitiveGeometry->indiceNumber = std::get<std::vector<uint16_t>>(m_primitiveInfos[name][L"index"]).size();

	primitiveGeometry->pos = std::get<std::vector<DirectX::XMFLOAT3>>(m_primitiveInfos[name][L"pos"]);
	primitiveGeometry->vertexNumber = std::get<std::vector<DirectX::XMFLOAT3>>(m_primitiveInfos[name][L"pos"]).size();

	primitiveGeometry->texC = std::get<std::vector<DirectX::XMFLOAT2>>(m_primitiveInfos[name][L"uvs"]);

	return primitiveGeometry;
}