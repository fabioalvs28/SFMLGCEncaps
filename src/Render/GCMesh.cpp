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
    m_Particles.resize(2);
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

    std::vector<uint16_t> indices;
    // Manage offset bits size for reserve vector
    if (HAS_FLAG(m_flagEnabledBits, HAS_POSITION)) vertexSize += 3; 
    if (HAS_FLAG(m_flagEnabledBits, HAS_COLOR)) vertexSize += 4;
    if (HAS_FLAG(m_flagEnabledBits, HAS_UV)) vertexSize += 2; 
    if (HAS_FLAG(m_flagEnabledBits, HAS_NORMAL)) vertexSize += 3; 

    vertexData.reserve(pGeometry->pos.size() * vertexSize * 2);

    for (int i = 0; i < m_Particles.size(); i++)
    {
        for (size_t j = 0; j < pGeometry->pos.size(); ++j)
        {
            if (HAS_FLAG(m_flagEnabledBits, HAS_POSITION)) 
            {
                vertexData.push_back(pGeometry->pos[j].x + i * 1.0f);
                vertexData.push_back(pGeometry->pos[j].y);
                vertexData.push_back(pGeometry->pos[j].z);
            }
            if (HAS_FLAG(m_flagEnabledBits, HAS_COLOR))
            {
                vertexData.push_back(pGeometry->color[j].x);
                vertexData.push_back(pGeometry->color[j].y);
                vertexData.push_back(pGeometry->color[j].z);
                vertexData.push_back(pGeometry->color[j].w);
            }
            if (HAS_FLAG(m_flagEnabledBits, HAS_UV)) {
                vertexData.push_back(pGeometry->uv[j].x);
                vertexData.push_back(pGeometry->uv[j].y);
            }
            if (HAS_FLAG(m_flagEnabledBits, HAS_NORMAL)) 
            {
                vertexData.push_back(pGeometry->normals[j].x);
                vertexData.push_back(pGeometry->normals[j].y);
                vertexData.push_back(pGeometry->normals[j].z);
            }
        }
        indices.insert(indices.begin(), pGeometry->indices.begin(), pGeometry->indices.end());
    }

    const UINT vbByteSize = static_cast<UINT>(vertexData.size() * sizeof(float));
    const UINT ibByteSize = static_cast<UINT>(indices.size() * sizeof(std::uint16_t));

    m_pBufferGeometryData = new GCMESHBUFFERDATA();

    D3DCreateBlob(vbByteSize, &m_pBufferGeometryData->VertexBufferCPU);
    CopyMemory(m_pBufferGeometryData->VertexBufferCPU->GetBufferPointer(), vertexData.data(), vbByteSize);

    D3DCreateBlob(ibByteSize, &m_pBufferGeometryData->IndexBufferCPU);
    CopyMemory(m_pBufferGeometryData->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

    m_pBufferGeometryData->VertexBufferGPU = CreateDefaultBuffer(m_pRender->Getmd3dDevice(), m_pRender->GetCommandList(), vertexData.data(), vbByteSize, &m_pBufferGeometryData->VertexBufferUploader);
    m_pBufferGeometryData->IndexBufferGPU = CreateDefaultBuffer(m_pRender->Getmd3dDevice(), m_pRender->GetCommandList(), indices.data(), ibByteSize, &m_pBufferGeometryData->IndexBufferUploader);

    m_pBufferGeometryData->VertexByteStride = static_cast<UINT>(vertexSize * sizeof(float));
    m_pBufferGeometryData->VertexBufferByteSize = vbByteSize;
    m_pBufferGeometryData->IndexFormat = DXGI_FORMAT_R16_UINT;
    m_pBufferGeometryData->IndexBufferByteSize = ibByteSize;

    m_pBufferGeometryData->IndexCount = static_cast<UINT>(indices.size());
}

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
        particle.Position = particle.initPos;

        particle.Velocity = { static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f,
                              static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f,
                              static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f };
        particle.Lifetime = 2.0f;
        particle.Age = 0.0f;
    }

    const UINT particleByteSize = static_cast<UINT>(m_Particles.size() * sizeof(GCPARTICLE));

    // Check the return value of CreateDefaultBuffer to ensure buffer creation was successful
    m_pParticleBufferGPU = CreateDefaultBuffer(m_pRender->Getmd3dDevice(), m_pRender->GetCommandList(), m_Particles.data(), particleByteSize, &m_pParticleBufferUploader);
    
}

void GCMesh::UpdateParticles(float deltaTime)
{
    for (auto& particle : m_Particles)
    {
        if (particle.Age < particle.Lifetime)
        {
            // Update particle position based on velocity
            particle.Position.x += particle.Velocity.x * deltaTime;
            particle.Position.y += particle.Velocity.y * deltaTime;
            particle.Position.z += particle.Velocity.z * deltaTime;

            // Update age of the particle
            particle.Age += deltaTime;
        }
        else
        {
            particle.Age = 0;
            particle.Position = particle.initPos;
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
        if (particle.Age < particle.Lifetime) {
            DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixTranslation(particle.Position.x, particle.Position.y, particle.Position.z);
            DirectX::XMMATRIX worldViewProjMatrix = worldMatrix * viewProjMatrix;
            DirectX::XMFLOAT4X4 worldViewProj;
            DirectX::XMStoreFloat4x4(&worldViewProj, XMMatrixTranspose(worldViewProjMatrix));
            graphics->UpdateWorldConstantBuffer(material, worldViewProj);
            graphics->GetRender()->DrawObject(this, material);
        }
    }
}

