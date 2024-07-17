#include "pch.h"

GCMesh::GCMesh()
    : m_pRender(nullptr),
    m_pBufferGeometryData(nullptr),
    m_flagEnabledBits(0),
    m_geoAmount(0)
{
}

GCMesh::~GCMesh()
{
    if (m_pBufferGeometryData)
    {
        m_pBufferGeometryData->VertexBufferCPU->Release();
        m_pBufferGeometryData->IndexBufferCPU->Release();

        m_pBufferGeometryData->VertexBufferGPU->Release();
        m_pBufferGeometryData->IndexBufferGPU->Release();

        m_pBufferGeometryData->VertexByteStride = 0;
        m_pBufferGeometryData->VertexBufferByteSize = 0;
        m_pBufferGeometryData->IndexFormat = DXGI_FORMAT_R16_UINT;
        m_pBufferGeometryData->IndexBufferByteSize = 0;
        m_pBufferGeometryData->IndexCount = 0;

        DELETE(m_pBufferGeometryData);
    }
}

GC_GRAPHICS_ERROR GCMesh::Initialize(GCRenderContext* pRender, GCGeometry* pGeometry, int& flagEnabledBits)
{

    if (!CHECK_POINTERSNULL("Pointers pRender & pGeometry Valid", "Pointers pRender & pGeometry Not valid", pRender, pGeometry)) {
        return GCRENDER_ERROR_POINTER_NULL;
    }

    m_pRender = pRender;

    UploadGeometryData(pGeometry, flagEnabledBits);

    if (!CHECK_POINTERSNULL(
        "All mesh buffer data pointers are valid",
        "One or more mesh buffer data pointers are null",
        m_pBufferGeometryData->VertexBufferCPU,
        m_pBufferGeometryData->IndexBufferCPU,
        m_pBufferGeometryData->VertexBufferGPU,
        m_pBufferGeometryData->IndexBufferGPU
    )) 
    {
        return GCRENDER_ERROR_POINTER_NULL;
    };

    return GCRENDER_SUCCESS_OK;
}

void GCMesh::UploadGeometryData(GCGeometry* pGeometry, int& flagEnabledBits) {
    m_flagEnabledBits = flagEnabledBits;

    std::vector<float> vertexData;
    size_t vertexSize = 0;

    // Manage offset bits size for reserve vector
    if (HAS_FLAG(m_flagEnabledBits, VERTEX_POSITION)) vertexSize += 3;
    if (HAS_FLAG(m_flagEnabledBits, VERTEX_COLOR)) vertexSize += 4;
    if (HAS_FLAG(m_flagEnabledBits, VERTEX_UV)) vertexSize += 2;
    if (HAS_FLAG(m_flagEnabledBits, VERTEX_NORMAL)) vertexSize += 3;

    vertexData.reserve(pGeometry->pos.size() * vertexSize * 2);

    for (size_t i = 0; i < pGeometry->pos.size(); ++i) 
    {
        if (HAS_FLAG(m_flagEnabledBits, VERTEX_POSITION)) 
        {
            vertexData.push_back(pGeometry->pos[i].x);
            vertexData.push_back(pGeometry->pos[i].y);
            vertexData.push_back(pGeometry->pos[i].z);
        }
        if (HAS_FLAG(m_flagEnabledBits, VERTEX_COLOR)) 
        {
            vertexData.push_back(pGeometry->color[i].x);
            vertexData.push_back(pGeometry->color[i].y);
            vertexData.push_back(pGeometry->color[i].z);
            vertexData.push_back(pGeometry->color[i].w);
        }
        if (HAS_FLAG(m_flagEnabledBits, VERTEX_UV)) 
        {
            vertexData.push_back(pGeometry->uv[i].x);
            vertexData.push_back(pGeometry->uv[i].y);
        }
        if (HAS_FLAG(m_flagEnabledBits, VERTEX_NORMAL)) 
        {
            vertexData.push_back(pGeometry->normals[i].x);
            vertexData.push_back(pGeometry->normals[i].y);
            vertexData.push_back(pGeometry->normals[i].z);
        }
    }

    const UINT vbByteSize = static_cast<UINT>(vertexData.size() * sizeof(float));
    const UINT ibByteSize = static_cast<UINT>(pGeometry->indices.size() * sizeof(std::uint16_t));

    m_pBufferGeometryData = new GCMESHBUFFERDATA();

    D3DCreateBlob(vbByteSize, &m_pBufferGeometryData->VertexBufferCPU);
    CopyMemory(m_pBufferGeometryData->VertexBufferCPU->GetBufferPointer(), vertexData.data(), vbByteSize);
    D3DCreateBlob(ibByteSize, &m_pBufferGeometryData->IndexBufferCPU);
    CopyMemory(m_pBufferGeometryData->IndexBufferCPU->GetBufferPointer(), pGeometry->indices.data(), ibByteSize);

    auto vertexBuffer = std::make_unique<GCUploadBuffer<float>>(m_pRender->GetRenderResources()->Getmd3dDevice(), static_cast<UINT>(vertexData.size()));
    auto indexBuffer = std::make_unique<GCUploadBuffer<std::uint16_t>>(m_pRender->GetRenderResources()->Getmd3dDevice(), static_cast<UINT>(pGeometry->indices.size()));

    vertexBuffer->CopyData(0, vertexData.data(), vbByteSize);
    indexBuffer->CopyData(0, pGeometry->indices.data(), ibByteSize);

    m_pBufferGeometryData->VertexBufferGPU = vertexBuffer->Resource();
    m_pBufferGeometryData->IndexBufferGPU = indexBuffer->Resource();
    m_pBufferGeometryData->VertexBufferUploader = vertexBuffer.release()->Resource();
    m_pBufferGeometryData->IndexBufferUploader = indexBuffer.release()->Resource();

    m_pBufferGeometryData->VertexByteStride = static_cast<UINT>(vertexSize * sizeof(float));
    m_pBufferGeometryData->VertexBufferByteSize = vbByteSize;
    m_pBufferGeometryData->IndexFormat = DXGI_FORMAT_R16_UINT;
    m_pBufferGeometryData->IndexBufferByteSize = ibByteSize;

    m_pBufferGeometryData->IndexCount = static_cast<UINT>(pGeometry->indices.size());
}
