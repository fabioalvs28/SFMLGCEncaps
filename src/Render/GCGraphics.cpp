#include "pch.h"

GCGraphics::GCGraphics()
{
    m_pRender = nullptr;
    m_pPrimitiveFactory = nullptr;
    m_pModelParserFactory = nullptr;

    m_vTextures.clear();
    m_vShaders.clear();
    m_vMaterials.clear();
    m_vMeshes.clear();
    m_pCbCameraInstances.clear();
}

GCGraphics::~GCGraphics()
{
    for (auto shader : m_vShaders)
    {
        SAFE_DELETE(shader);
    }
    m_vShaders.clear();

    for (auto material : m_vMaterials)
    {
        SAFE_DELETE(material);
    }
    m_vMaterials.clear();

    for (auto mesh : m_vMeshes)
    {
        SAFE_DELETE(mesh);
    }
    m_vMeshes.clear();

    for (auto texture : m_vTextures)
    {
        SAFE_DELETE(texture);
    }
    m_vTextures.clear();

    for (auto buffer : m_pCbCameraInstances)
    {
        SAFE_DELETE(buffer);
    }
    m_pCbCameraInstances.clear();

    SAFE_DELETE(m_pRender);
    SAFE_DELETE(m_pPrimitiveFactory);
    SAFE_DELETE(m_pModelParserFactory);
}

bool GCGraphics::Initialize(Window* pWindow,int renderWidth,int renderHeight)
{
    if (!CHECK_POINTERSNULL("Graphics Initialized with window sucessfully", "Can't initialize Graphics, Window is empty", pWindow))
        return false;

    //Creates Primitive and parser instances
    m_pPrimitiveFactory = new GCPrimitiveFactory();
    m_pModelParserFactory = new GCModelParserObj();

    m_pPrimitiveFactory->Initialize();

    //Initializes Graphics for a window
    m_pRender = new GCRender();
    m_pRender->Initialize(pWindow, renderWidth, renderHeight, this);
    
    m_renderWidth = renderWidth;
    m_renderHeight = renderHeight;



	GCShaderUploadBufferBase* pCbInstance = new GCShaderUploadBuffer<GCVIEWPROJCB>(m_pRender->Getmd3dDevice(), 1, true);
    m_pCbCameraInstances.push_back(pCbInstance);

    pCbInstance = new GCShaderUploadBuffer<GCLIGHTSPROPERTIES>(m_pRender->Getmd3dDevice(), 1, true);
    m_pCbLightPropertiesInstance = pCbInstance;

    GCLIGHTSPROPERTIES lightData = {};

    UpdateConstantBuffer(lightData, m_pCbLightPropertiesInstance);

    m_pRender->m_pCbLightPropertiesInstance = m_pCbLightPropertiesInstance;

    return true;
}

bool GCGraphics::StartFrame()
{
    for (auto& material : m_vMaterials)
    {
        for (auto& cbObject : material->GetCbObjectInstance())
        {
            cbObject->m_isUsed = false;
        }
    }
    m_pRender->PrepareDraw();

    return true;
};
bool GCGraphics::EndFrame()
{
    GCGraphicsLogger& profiler = GCGraphicsLogger::GetInstance();

    m_pRender->PostDraw();

    for (int i = 0; i < m_vMaterials.size(); i++) {
        m_vMaterials[i]->ResetCBCount();
    }

    // Vérification des CB inutilisés dans les matériaux / CHECK FOR REMOVE CB BUFFER IN MATERIALS 
    for (auto& material : m_vMaterials)
    {
        for (auto& cbObject : material->GetCbObjectInstance())
        {
            if (cbObject->m_isUsed)
                cbObject->m_framesSinceLastUse = 0;
            if (!cbObject->m_isUsed)
            {
                cbObject->m_framesSinceLastUse++;
                if (cbObject->m_framesSinceLastUse > 180)
                {
                    profiler.LogInfo("Constant buffer inutilisé trouvé dans le matériau : ");
                }
            }
        }
    }

    return true;
};

bool GCGraphics::InitializeGraphicsResourcesStart() {
    HRESULT hr = m_pRender->GetCommandList()->Reset(m_pRender->GetCommandAllocator(), nullptr);
    if (!CHECK_HRESULT(hr, "CommandList->Reset()")) {
        return false;
    };

    return true;
}
bool GCGraphics::InitializeGraphicsResourcesEnd() {
    HRESULT hr;
    hr = m_pRender->GetCommandList()->Close();
    if (!CHECK_HRESULT(hr, "CommandList()->Close()")) {
        return false;
    };
    m_pRender->ExecuteCommandList();

    if (!m_pRender->FlushCommandQueue())
        return false;

    return true;
}

ResourceCreationResult<GCTexture*> GCGraphics::CreateTexture(const std::string& filePath)
{
    GCGraphicsLogger& profiler = GCGraphicsLogger::GetInstance();

    // Creates and initializes a texture using a path
    GCTexture* texture = new GCTexture();

    // Find the first inactive slot in m_vTextureActiveFlags
    auto it = std::find(m_vTextureActiveFlags.begin(), m_vTextureActiveFlags.end(), false);

    size_t index;

    if (it != m_vTextureActiveFlags.end()) {
        // Calculate the index of the found position
        index = std::distance(m_vTextureActiveFlags.begin(), it);

        // Insert the texture at the found index
        m_vTextures.insert(m_vTextures.begin() + index, texture);

        // Set the corresponding flag to true
        m_vTextureActiveFlags[index] = true;
    }
    else {
        // If no inactive slot was found, push the texture to the end
        index = m_vTextures.size(); // Index will be the last position

        m_vTextures.push_back(texture);
        m_vTextureActiveFlags.push_back(true);
    }

    // Initialize the texture with the specified index
    if (!texture->Initialize(filePath, this, index))
        return ResourceCreationResult<GCTexture*>(false, nullptr);


    // Return the result of the creation operation
    return ResourceCreationResult<GCTexture*>(true, texture);
}

ResourceCreationResult<GCShader*> GCGraphics::CreateShaderColor()
{
    GCShader* pShader = new GCShader();

    int flags = 0;
    SET_FLAG(flags, HAS_POSITION);
    SET_FLAG(flags, HAS_COLOR);

    if (!pShader->Initialize(m_pRender, "../../../src/Render/Shaders/color.hlsl", "../../../src/Render/CsoCompiled/color", flags))
        return ResourceCreationResult<GCShader*>(false, nullptr);
    if (!pShader->Load())
        return ResourceCreationResult<GCShader*>(false, nullptr);

    m_vShaders.push_back(pShader);

    return ResourceCreationResult<GCShader*>(true, pShader);
}

ResourceCreationResult<GCShader*> GCGraphics::CreateShaderTexture()
{
    GCShader* pShader = new GCShader();

    int flags = 0;
    SET_FLAG(flags, HAS_POSITION);
    SET_FLAG(flags, HAS_UV);

    if (!pShader->Initialize(m_pRender, "../../../src/Render/Shaders/texture.hlsl", "../../../src/Render/CsoCompiled/texture", flags));
        return ResourceCreationResult<GCShader*>(false, nullptr);
    if (!pShader->Load());
        return ResourceCreationResult<GCShader*>(false, nullptr);

    m_vShaders.push_back(pShader);

    return ResourceCreationResult<GCShader*>(true, pShader);
}

// Specify the path, with the name of the shader at the file creation , example : CsoCompiled/texture, texture is the name of the file in Cso Compiled Folder
ResourceCreationResult<GCShader*> GCGraphics::CreateShaderCustom(std::string& filePath, std::string& compiledShaderDestinationPath, int& flagEnabledBits, D3D12_CULL_MODE cullMode)
{
    GCShader* pShader = new GCShader();
    pShader->Initialize(m_pRender, filePath, compiledShaderDestinationPath, flagEnabledBits, cullMode);
    pShader->Load();

    m_vShaders.push_back(pShader);

    return ResourceCreationResult<GCShader*>(true, pShader);
}

ResourceCreationResult<GCMesh*> GCGraphics::CreateMeshCustom(GCGeometry* pGeometry, int& flagEnabledBits)
{
    if (!CHECK_POINTERSNULL("Geometry loaded successfully for mesh", "Can't create mesh, Geometry is empty", pGeometry))
    {
        return ResourceCreationResult<GCMesh*>(false, nullptr);
    }

    GCMesh* pMesh = new GCMesh();
    if (!pMesh->Initialize(m_pRender, pGeometry, flagEnabledBits))
        return ResourceCreationResult<GCMesh*>(false, nullptr);

    m_vMeshes.push_back(pMesh);
    m_vMeshActiveFlags.push_back(true);

    return ResourceCreationResult<GCMesh*>(true, pMesh);

    //// Find the first inactive slot in m_vMeshActiveFlags
    //auto it = std::find(m_vMeshActiveFlags.begin(), m_vMeshActiveFlags.end(), false);

    //// Inactive slot found
    //if (it != m_vMeshActiveFlags.end())
    //{
    //    size_t index = std::distance(m_vMeshActiveFlags.begin(), it);

    //    GCMesh* pMesh = new GCMesh();
    //    if (!pMesh->Initialize(m_pRender, pGeometry, flagEnabledBits))
    //        return ResourceCreationResult<GCMesh*>(false, nullptr);

    //    m_vMeshes.insert(m_vMeshes.begin() + index, pMesh);

    //    m_vMeshActiveFlags[index] = true;

    //    return ResourceCreationResult<GCMesh*>(true, pMesh);
    //}
    //// Not inactive slot 
    //else
    //{
    //    GCMesh* pMesh = new GCMesh();
    //    if (!pMesh->Initialize(m_pRender, pGeometry, flagEnabledBits))
    //        return ResourceCreationResult<GCMesh*>(false, nullptr);

    //    m_vMeshes.push_back(pMesh);
    //    m_vMeshActiveFlags.push_back(true);

    //    return ResourceCreationResult<GCMesh*>(true, pMesh);
    //}
}

ResourceCreationResult<GCMesh*> GCGraphics::CreateMeshColor(GCGeometry* pGeometry)
{
    int flagsLightColor = 0;
    SET_FLAG(flagsLightColor, HAS_POSITION);
    SET_FLAG(flagsLightColor, HAS_COLOR);

    GCGraphicsLogger& profiler = GCGraphicsLogger::GetInstance();

    // Check if Geometry is valid
    if (!CHECK_POINTERSNULL("Geometry loaded successfully for mesh", "Can't create mesh, Geometry is empty", pGeometry))
        return ResourceCreationResult<GCMesh*>(false, nullptr);

    GCMesh* pMesh = new GCMesh();
    if (!pMesh->Initialize(m_pRender, pGeometry, flagsLightColor))
        return ResourceCreationResult<GCMesh*>(false, nullptr);

    m_vMeshes.push_back(pMesh);
    m_vMeshActiveFlags.push_back(true);

    return ResourceCreationResult<GCMesh*>(true, pMesh);

    //// Find the first inactive slot in m_vMeshActiveFlags
    //auto it = std::find(m_vMeshActiveFlags.begin(), m_vMeshActiveFlags.end(), false);

    //// Inactive slot found
    //if (it != m_vMeshActiveFlags.end())
    //{
    //    size_t index = std::distance(m_vMeshActiveFlags.begin(), it);

    //    GCMesh* pMesh = new GCMesh();
    //    if (!pMesh->Initialize(m_pRender, pGeometry, flagsLightColor))
    //        return ResourceCreationResult<GCMesh*>(false, nullptr);

    //    m_vMeshes.insert(m_vMeshes.begin() + index, pMesh);

    //    m_vMeshActiveFlags[index] = true;

    //    return ResourceCreationResult<GCMesh*>(true, pMesh);
    //}
    //// Not inactive slot 
    //else
    //{
    //    GCMesh* pMesh = new GCMesh();
    //    if (!pMesh->Initialize(m_pRender, pGeometry, flagsLightColor))
    //        return ResourceCreationResult<GCMesh*>(false, nullptr);

    //    m_vMeshes.push_back(pMesh);
    //    m_vMeshActiveFlags.push_back(true);

    //    return ResourceCreationResult<GCMesh*>(true, pMesh);
    //}
}

ResourceCreationResult<GCMesh*> GCGraphics::CreateMeshTexture(GCGeometry* pGeometry)
{
    int flagsLightTexture = 0;
    SET_FLAG(flagsLightTexture, HAS_POSITION);
    SET_FLAG(flagsLightTexture, HAS_UV);

    GCGraphicsLogger& profiler = GCGraphicsLogger::GetInstance();

    // Check if Geometry is valid
    if (!CHECK_POINTERSNULL("Geometry loaded successfully for mesh", "Can't create mesh, Geometry is empty", pGeometry))
    {
        return ResourceCreationResult<GCMesh*>(false, nullptr);
    }

    GCMesh* pMesh = new GCMesh();
    pMesh->Initialize(m_pRender, pGeometry, flagsLightTexture);

    m_vMeshes.push_back(pMesh);
    m_vMeshActiveFlags.push_back(true);

    return ResourceCreationResult<GCMesh*>(true, pMesh);

    //// Find the first inactive slot in m_vMeshActiveFlags
    //auto it = std::find(m_vMeshActiveFlags.begin(), m_vMeshActiveFlags.end(), false);

    //// Inactive slot found
    //if (it != m_vMeshActiveFlags.end())
    //{
    //    size_t index = std::distance(m_vMeshActiveFlags.begin(), it);

    //    GCMesh* pMesh = new GCMesh();
    //    pMesh->Initialize(m_pRender, pGeometry, flagsLightTexture);

    //    m_vMeshes.insert(m_vMeshes.begin() + index, pMesh);

    //    m_vMeshActiveFlags[index] = true;

    //    return ResourceCreationResult<GCMesh*>(true, pMesh);
    //}
    //// Not inactive slot 
    //else
    //{
    //    GCMesh* pMesh = new GCMesh();
    //    pMesh->Initialize(m_pRender, pGeometry, flagsLightTexture);

    //    m_vMeshes.push_back(pMesh);
    //    m_vMeshActiveFlags.push_back(true);

    //    return ResourceCreationResult<GCMesh*>(true, pMesh);
    //}
}

ResourceCreationResult<GCGeometry*> GCGraphics::CreateGeometryPrimitive(const GC_PRIMITIVE_ID primitiveIndex, const DirectX::XMFLOAT4& color)
{
    GCGeometry* pGeometry = new GCGeometry();

    if (!m_pPrimitiveFactory->BuildGeometry(primitiveIndex, color, pGeometry))
        return ResourceCreationResult<GCGeometry*>(false, nullptr);

    if (!CHECK_POINTERSNULL("Primitive Geometry with Custom parameters created successfully", "Failed to create Primitive Geometry with Custom parameters", pGeometry))
        return ResourceCreationResult<GCGeometry*>(false, nullptr);

    return ResourceCreationResult<GCGeometry*>(true, pGeometry);
}

ResourceCreationResult<GCGeometry*> GCGraphics::CreateGeometryModelParser(const std::string& filePath, DirectX::XMFLOAT4 color, Extensions fileExtensionType)
{
    GCGeometry* pGeometry = new GCGeometry;

    if(!m_pModelParserFactory->BuildModel(filePath, DirectX::XMFLOAT4(DirectX::Colors::Gray), fileExtensionType, pGeometry))
        return ResourceCreationResult<GCGeometry*>(false, nullptr);

    if(!CHECK_POINTERSNULL("Geometry from Model Parser with Texture created successfully", "Failed to create Geometry from Model Parser with Texture", pGeometry))
        return ResourceCreationResult<GCGeometry*>(false, nullptr);

    return ResourceCreationResult<GCGeometry*>(true, pGeometry);
}

ResourceCreationResult<GCMaterial*> GCGraphics::CreateMaterial(GCShader* pShader)
{
    if (!CHECK_POINTERSNULL("Shader loaded successfully for material", "Can't create material, Shader is empty", pShader))
        return ResourceCreationResult<GCMaterial*>(false, nullptr);

    GCMaterial* material = new GCMaterial();
    material->Initialize(pShader);
    m_vMaterials.push_back(material);

    return ResourceCreationResult<GCMaterial*>(true, material);
}

std::vector<GCShader*> GCGraphics::GetShaders() 
{
    return m_vShaders;
}

std::vector<GCMaterial*> GCGraphics::GetMaterials() 
{
    return m_vMaterials;
}

std::vector<GCMesh*> GCGraphics::GetMeshes() 
{
    return m_vMeshes;
}

std::vector<GCTexture*> GCGraphics::GetTextures() 
{
    return m_vTextures;
}

bool GCGraphics::RemoveShader(GCShader* pShader)
{
    if (!CHECK_POINTERSNULL("Ptr for RemoveShader is not null", "Can't remove shader, pShader is null", pShader))
        return false;

    // Removes Shader both from vector and the shader itself
    auto it = std::find(m_vShaders.begin(), m_vShaders.end(), pShader);

    if (LOG_REMOVE_RESOURCE(it, "Shader", m_vShaders))
    {
        m_vShaders.erase(it);
        delete pShader;
        return true;
    }
    return false;
}

bool GCGraphics::RemoveMaterial(GCMaterial* pMaterial)
{
    if (!CHECK_POINTERSNULL("Ptr for RemoveMaterial is not null", "Can't remove material, pMaterial is null", pMaterial))
        return false;

    auto it = std::find(m_vMaterials.begin(), m_vMaterials.end(), pMaterial);

    if (LOG_REMOVE_RESOURCE(it, "Material", m_vMaterials))
    {
        m_vMaterials.erase(it);
        delete pMaterial;
        return true;
    }
    return false;
}

bool GCGraphics::RemoveMesh(GCMesh* pMesh)
{
    if(!CHECK_POINTERSNULL("Ptr for RemoveMesh is not null", "Can't remove mesh, pMesh is null", pMesh))
        return false;

    // Removes Mesh
    auto it = std::find(m_vMeshes.begin(), m_vMeshes.end(), pMesh);

    if (it != m_vMeshes.end())
    {
        if (LOG_REMOVE_RESOURCE(it, "Mesh", m_vMeshes))
        {
            size_t index = std::distance(m_vMeshes.begin(), it);

            if (index < m_vMeshActiveFlags.size())
            {
                m_vMeshActiveFlags[index] = false;
            }

            m_vMeshes.erase(it);
            delete pMesh;

            return true;
        }
    }
    return false;
}

bool GCGraphics::RemoveTexture(GCTexture* pTexture)
{
    if(!CHECK_POINTERSNULL("Ptr for RemoveTexture is not null", "Can't remove texture, pTexture is null", pTexture))
        return false;

    // Removes Texture
    auto it = std::find(m_vTextures.begin(), m_vTextures.end(), pTexture);

    if (LOG_REMOVE_RESOURCE(it, "Texture", m_vTextures))
    {
        size_t index = std::distance(m_vTextures.begin(), it);

        if (index < m_vTextureActiveFlags.size())
        {
            m_vTextureActiveFlags[index] = false;
        }

        delete pTexture;
        m_vTextures.erase(it);

        return true;
    }
    return false;
}

// Update per camera constant buffer
bool GCGraphics::UpdateViewProjConstantBuffer(DirectX::XMFLOAT4X4 projectionMatrix, DirectX::XMFLOAT4X4 viewMatrix) 
{
    GCVIEWPROJCB cameraData;
    cameraData.view = viewMatrix;
    cameraData.proj = projectionMatrix;

    UpdateConstantBuffer(cameraData, m_pCbCameraInstances[0]);

    m_pRender->m_pCbCurrentViewProjInstance = m_pCbCameraInstances[0];

    return true;
}

// Update per object constant buffer
bool GCGraphics::UpdateWorldConstantBuffer(GCMaterial* pMaterial, DirectX::XMFLOAT4X4 worldMatrix) 
{
    if (!CHECK_POINTERSNULL("Ptr for Update World Constant Buffer is not null", "Ptr for UpdateMaterialProperties is null", pMaterial))
        return false;

    if (pMaterial->GetCount() >= pMaterial->GetCbObjectInstance().size()) {
        pMaterial->AddCbPerObject<GCWORLDCB>();
    }
    GCWORLDCB worldData;
    worldData.world = worldMatrix;
    worldData.objectId = 100.2f;
    // Update 
    pMaterial->UpdateConstantBuffer(worldData, pMaterial->GetCbObjectInstance()[pMaterial->GetCount()]);

}

//Update Camera and Object Constant Buffer / But They can also send their own structure
void GCGraphics::UpdateConstantBuffer(const GCSHADERCB& objectData, GCShaderUploadBufferBase* uploadBufferInstance)
{
    uploadBufferInstance->CopyData(0, objectData);
}

DirectX::XMFLOAT4X4 GCGraphics::ToPixel(int pixelX, int pixelY, DirectX::XMFLOAT4X4 proj, DirectX::XMFLOAT4X4 view) {
    DirectX::XMFLOAT3 worldPos = GCUtils::PixelToWorld(pixelX, pixelY, m_renderWidth, m_renderHeight, proj, view);

    DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslation(worldPos.x, worldPos.y, worldPos.z);

    DirectX::XMFLOAT4X4 worldMatrix;
    DirectX::XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixTranspose(translationMatrix));

    return worldMatrix;
}

bool GCGraphics::UpdateMaterialProperties(GCMaterial* pMaterial, GCMATERIALPROPERTIES objectData)
{
    if (!CHECK_POINTERSNULL("Ptr for Update Material Properties is not null", "Ptr for UpdateMaterialProperties is null", pMaterial))
        return false;

    UpdateConstantBuffer(objectData, pMaterial->GetCbMaterialPropertiesInstance());
    return true;
}

bool GCGraphics::UpdateMaterialProperties(GCMaterial* pMaterial, DirectX::XMFLOAT4 ambientLightColor, DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 diffuse, DirectX::XMFLOAT4 specular, float shininess)
{
    if (!CHECK_POINTERSNULL("Ptr for Update Material Properties is not null", "Ptr for UpdateMaterialProperties is null", pMaterial))
        return false;

    GCMATERIALPROPERTIES materialData;
    materialData.ambientLightColor = ambientLightColor;
    materialData.ambient = ambient;
    materialData.diffuse = diffuse;
    materialData.specular = specular;
    materialData.shininess = shininess;

    UpdateConstantBuffer(materialData, pMaterial->GetCbMaterialPropertiesInstance());
    return true;
}

bool GCGraphics::UpdateLights(GCLIGHTSPROPERTIES objectData) {
    UpdateConstantBuffer(objectData, m_pCbLightPropertiesInstance);

    return true;
}
