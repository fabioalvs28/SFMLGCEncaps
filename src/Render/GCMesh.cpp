#include "pch.h"

ID3D12Resource* GCMesh::CreateDefaultBuffer(
    ID3D12Device* device,
    ID3D12GraphicsCommandList* cmdList,
    const void* initData,
    UINT64 byteSize,
    ID3D12Resource** uploadBuffer) // Change the uploadBuffer to **uploadBuffer
{
    ID3D12Resource* defaultBuffer = nullptr;

    // Create the default buffer resource.
    CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_DEFAULT);
    CD3DX12_RESOURCE_DESC resDesc(CD3DX12_RESOURCE_DESC::Buffer(byteSize));
    HRESULT hr = device->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_COMMON,
        nullptr,
        IID_PPV_ARGS(&defaultBuffer)
    );

    if (FAILED(hr))
    {
        return nullptr;  // Handle failure
    }

    // Create the upload buffer resource.
    CD3DX12_HEAP_PROPERTIES heapPropUp(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC resDesc2(CD3DX12_RESOURCE_DESC::Buffer(byteSize));
    hr = device->CreateCommittedResource(
        &heapPropUp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc2,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(uploadBuffer)
    );

    if (FAILED(hr))
    {
        defaultBuffer->Release();
        return nullptr;  // Handle failure
    }

    // Describe the data we want to copy into the default buffer.
    D3D12_SUBRESOURCE_DATA subResourceData = {};
    subResourceData.pData = initData;
    subResourceData.RowPitch = byteSize;
    subResourceData.SlicePitch = subResourceData.RowPitch;

    // Transition the default buffer to the COPY_DEST state
    CD3DX12_RESOURCE_BARRIER ResBarrier(CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
    cmdList->ResourceBarrier(1, &ResBarrier);

    // Copy the data from the upload buffer to the default buffer
    UpdateSubresources(cmdList, defaultBuffer, *uploadBuffer, 0, 0, 1, &subResourceData);

    // Transition the default buffer to the GPU_READ state
    CD3DX12_RESOURCE_BARRIER ResBarrier2(CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));
    cmdList->ResourceBarrier(1, &ResBarrier2);

    // Return the default buffer
    return defaultBuffer;
}


GCMesh::GCMesh()
{
    m_pRender = nullptr;
    m_pBufferGeometryData = nullptr;
    m_flagEnabledBits = 0;

    m_pParticleBufferGPU = nullptr;
    m_pParticleBufferUploader = nullptr;
}

GCMesh::~GCMesh()
{
    if (m_pBufferGeometryData)
    {
        SAFE_RELEASE(m_pBufferGeometryData->VertexBufferCPU);
        SAFE_RELEASE(m_pBufferGeometryData->IndexBufferCPU);
        SAFE_RELEASE(m_pBufferGeometryData->VertexBufferGPU);
        SAFE_RELEASE(m_pBufferGeometryData->IndexBufferGPU);
        SAFE_RELEASE(m_pBufferGeometryData->VertexBufferUploader);
        SAFE_RELEASE(m_pBufferGeometryData->IndexBufferUploader);

        m_pBufferGeometryData->VertexByteStride = 0;
        m_pBufferGeometryData->VertexBufferByteSize = 0;
        m_pBufferGeometryData->IndexFormat = DXGI_FORMAT_R16_UINT;
        m_pBufferGeometryData->IndexBufferByteSize = 0;
        m_pBufferGeometryData->IndexCount = 0;

        SAFE_DELETE(m_pBufferGeometryData);
    }

    SAFE_RELEASE(m_pParticleBufferGPU);
    SAFE_RELEASE(m_pParticleBufferUploader);
}

bool GCMesh::Initialize(GCRender* pRender, GCGeometry* pGeometry, int& flagEnabledBits) 
{
    CHECK_POINTERSNULL("Graphics Initialized with window sucessfully", "Can't initialize Graphics, Window is empty", pRender);
    m_pRender = pRender;
    UploadGeometryData(pGeometry, flagEnabledBits);

    return true;
}

void GCMesh::UploadGeometryData(GCGeometry* pGeometry, int& flagEnabledBits) {
    m_flagEnabledBits = flagEnabledBits;

    std::vector<float> vertexData;
    size_t vertexSize = 0;

    // Manage offset bits size for reserve vector
    if (HAS_FLAG(m_flagEnabledBits, HAS_POSITION)) vertexSize += 3; 
    if (HAS_FLAG(m_flagEnabledBits, HAS_COLOR)) vertexSize += 4;
    if (HAS_FLAG(m_flagEnabledBits, HAS_UV)) vertexSize += 2; 
    if (HAS_FLAG(m_flagEnabledBits, HAS_NORMAL)) vertexSize += 3; 

    vertexData.reserve(pGeometry->pos.size() * vertexSize * 2);

    for (size_t i = 0; i < pGeometry->pos.size(); ++i)
    {
        if (HAS_FLAG(m_flagEnabledBits, HAS_POSITION)) 
        {
            vertexData.push_back(pGeometry->pos[i].x);
            vertexData.push_back(pGeometry->pos[i].y);
            vertexData.push_back(pGeometry->pos[i].z);
        }
        if (HAS_FLAG(m_flagEnabledBits, HAS_COLOR))
        {
            vertexData.push_back(pGeometry->color[i].x);
            vertexData.push_back(pGeometry->color[i].y);
            vertexData.push_back(pGeometry->color[i].z);
            vertexData.push_back(pGeometry->color[i].w);
        }
        if (HAS_FLAG(m_flagEnabledBits, HAS_UV)) 
        {
            vertexData.push_back(pGeometry->uv[i].x);
            vertexData.push_back(pGeometry->uv[i].y);
        }
        if (HAS_FLAG(m_flagEnabledBits, HAS_NORMAL)) 
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
    
    m_pBufferGeometryData->VertexBufferGPU = CreateDefaultBuffer(m_pRender->Getmd3dDevice(), m_pRender->GetCommandList(), vertexData.data(), vbByteSize, &m_pBufferGeometryData->VertexBufferUploader);
    m_pBufferGeometryData->IndexBufferGPU = CreateDefaultBuffer(m_pRender->Getmd3dDevice(), m_pRender->GetCommandList(), pGeometry->indices.data(), ibByteSize, &m_pBufferGeometryData->IndexBufferUploader);

    m_pBufferGeometryData->VertexByteStride = static_cast<UINT>(vertexSize * sizeof(float));
    m_pBufferGeometryData->VertexBufferByteSize = vbByteSize;
    m_pBufferGeometryData->IndexFormat = DXGI_FORMAT_R16_UINT;
    m_pBufferGeometryData->IndexBufferByteSize = ibByteSize;

    m_pBufferGeometryData->IndexCount = static_cast<UINT>(pGeometry->indices.size());
}

//bool GCMesh::AddParticle(GCGeometry* pGeometry, DirectX::XMFLOAT3 pos)
//{
//    GCPARTICLE particle;
//    particle.pParticleGeometry = pGeometry;
//    particle.initPos = pos;
//
//    m_Particles.push_back(particle);
//
//    return true;
//}

//bool GCMesh::DeleteParticleAt(int index)
//{
//    m_Particles.erase(m_Particles.begin() + index);
//
//    return true;
//}

void GCMesh::InitializeParticleSystem(size_t maxParticles)
{
    m_Particles.resize(maxParticles);

    // Initialize particles with default values
    for (auto& particle : m_Particles)
    {
        particle.initPos = { static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f,
                              static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f,
                              static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f };

        // Give each particle a random position and velocity for better visualization
        particle.position = particle.initPos;

        particle.velocity = { static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f,
                              static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f,
                              static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f };
        particle.lifetime = 2.0f;
        particle.age = 0.0f;
    }

    const UINT particleByteSize = static_cast<UINT>(m_Particles.size() * sizeof(GCPARTICLE));

    // Check the return value of CreateDefaultBuffer to ensure buffer creation was successful
    m_pParticleBufferGPU = CreateDefaultBuffer(m_pRender->Getmd3dDevice(), m_pRender->GetCommandList(), m_Particles.data(), particleByteSize, &m_pParticleBufferUploader);
}

void GCMesh::UpdateParticles(float deltaTime)
{
    for (auto& particle : m_Particles)
    {
        if (particle.age < particle.lifetime)
        {
            // Update particle position based on velocity
            particle.position.x += particle.velocity.x * deltaTime;
            particle.position.y += particle.velocity.y * deltaTime;
            particle.position.z += particle.velocity.z * deltaTime;

            // Update age of the particle
            particle.age += deltaTime;
        }
        else
        {
            particle.age = 0;
            particle.position = particle.initPos;
        }
    }

    // Update GPU buffer with the new particle data
    D3D12_SUBRESOURCE_DATA subResourceData = {};
    subResourceData.pData = m_Particles.data();
    subResourceData.RowPitch = static_cast<UINT>(m_Particles.size() * sizeof(GCPARTICLE));
    subResourceData.SlicePitch = subResourceData.RowPitch;

    // Use the command list to copy the updated data to the GPU buffer
    CD3DX12_RESOURCE_BARRIER ResBarrier(CD3DX12_RESOURCE_BARRIER::Transition(m_pParticleBufferGPU, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
    m_pRender->GetCommandList()->ResourceBarrier(1, &ResBarrier);

    UpdateSubresources<1>(m_pRender->GetCommandList(), m_pParticleBufferGPU, m_pParticleBufferUploader, 0, 0, 1, &subResourceData);

    CD3DX12_RESOURCE_BARRIER ResBarrier2(CD3DX12_RESOURCE_BARRIER::Transition(m_pParticleBufferGPU, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));
    m_pRender->GetCommandList()->ResourceBarrier(1, &ResBarrier2);
}

void GCMesh::RenderParticles(GCGraphics* graphics, GCMaterial* material, DirectX::XMMATRIX viewProjMatrix)
{
    for (const auto& particle : m_Particles) {
        if (particle.age < particle.lifetime) {
            DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixTranslation(particle.position.x, particle.position.y, particle.position.z);
            DirectX::XMMATRIX worldViewProjMatrix = worldMatrix * viewProjMatrix;
            DirectX::XMFLOAT4X4 worldViewProj;
            DirectX::XMStoreFloat4x4(&worldViewProj, XMMatrixTranspose(worldViewProjMatrix));
            graphics->UpdateWorldConstantBuffer(material, worldViewProj);
            graphics->GetRender()->DrawObject(this, material);
        }
    }
}

//bool GCMesh::UpdateGeometryData(GCGeometry* pNewGeometry, int& newFlagEnabledBits)
//{
//    if (!pNewGeometry || !m_pBufferGeometryData) {
//        // Log or handle the error
//        return false;
//    }
//
//    m_flagEnabledBits = newFlagEnabledBits;
//
//    // Calculate new vertex size based on updated flags
//    size_t newVertexSize = 0;
//    if (HAS_FLAG(m_flagEnabledBits, HAS_POSITION)) newVertexSize += 3;
//    if (HAS_FLAG(m_flagEnabledBits, HAS_COLOR)) newVertexSize += 4;
//    if (HAS_FLAG(m_flagEnabledBits, HAS_UV)) newVertexSize += 2;
//    if (HAS_FLAG(m_flagEnabledBits, HAS_NORMAL)) newVertexSize += 3;
//
//    std::vector<float> newVertexData;
//    newVertexData.reserve(pNewGeometry->pos.size() * newVertexSize * 2);
//
//    for (size_t i = 0; i < pNewGeometry->pos.size(); ++i)
//    {
//        if (HAS_FLAG(m_flagEnabledBits, HAS_POSITION))
//        {
//            newVertexData.push_back(pNewGeometry->pos[i].x);
//            newVertexData.push_back(pNewGeometry->pos[i].y);
//            newVertexData.push_back(pNewGeometry->pos[i].z);
//        }
//        if (HAS_FLAG(m_flagEnabledBits, HAS_COLOR))
//        {
//            newVertexData.push_back(pNewGeometry->color[i].x);
//            newVertexData.push_back(pNewGeometry->color[i].y);
//            newVertexData.push_back(pNewGeometry->color[i].z);
//            newVertexData.push_back(pNewGeometry->color[i].w);
//        }
//        if (HAS_FLAG(m_flagEnabledBits, HAS_UV)) 
//        {
//            newVertexData.push_back(pNewGeometry->uv[i].x);
//            newVertexData.push_back(pNewGeometry->uv[i].y);
//        }
//        if (HAS_FLAG(m_flagEnabledBits, HAS_NORMAL))
//        {
//            newVertexData.push_back(pNewGeometry->normals[i].x);
//            newVertexData.push_back(pNewGeometry->normals[i].y);
//            newVertexData.push_back(pNewGeometry->normals[i].z);
//        }
//    }
//
//    const UINT newVbByteSize = static_cast<UINT>(newVertexData.size() * sizeof(float));
//    const UINT newIbByteSize = static_cast<UINT>(pNewGeometry->indices.size() * sizeof(uint16_t));
//
//    // Map the upload buffer and copy the vertex data
//    void* pVertexDataBegin = nullptr;
//    HRESULT hr = m_pBufferGeometryData->VertexBufferUploader->Map(0, nullptr, &pVertexDataBegin);
//    if (FAILED(hr)) {
//        // Log or handle the error
//        return false;
//    }
//    memcpy(pVertexDataBegin, newVertexData.data(), newVbByteSize);
//    m_pBufferGeometryData->VertexBufferUploader->Unmap(0, nullptr);
//
//    // Map the upload buffer and copy the index data
//    void* pIndexDataBegin = nullptr;
//    hr = m_pBufferGeometryData->IndexBufferUploader->Map(0, nullptr, &pIndexDataBegin);
//    if (FAILED(hr)) {
//        // Log or handle the error
//        return false;
//    }
//    memcpy(pIndexDataBegin, pNewGeometry->indices.data(), newIbByteSize);
//    m_pBufferGeometryData->IndexBufferUploader->Unmap(0, nullptr);
//
//    // Update the buffer size and data
//    m_pBufferGeometryData->VertexByteStride = static_cast<UINT>(newVertexSize * sizeof(float));
//    m_pBufferGeometryData->VertexBufferByteSize = newVbByteSize;
//    m_pBufferGeometryData->IndexFormat = DXGI_FORMAT_R16_UINT;
//    m_pBufferGeometryData->IndexBufferByteSize = newIbByteSize;
//    m_pBufferGeometryData->IndexCount = static_cast<UINT>(pNewGeometry->indices.size());
//
//    return true;
//}
