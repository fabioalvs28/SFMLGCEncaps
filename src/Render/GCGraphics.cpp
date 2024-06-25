#include "framework.h"

GCGraphics::GCGraphics()
{
    m_pRender = nullptr;
    m_pPrimitiveFactory = nullptr;
    m_pModelParserFactory = nullptr;

    m_vTextures.clear();
    m_vShaders.clear();
    m_vMaterials.clear();
    m_vMeshes.clear();
    m_vpCameraCB.clear();
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

    for (auto buffer : m_vpCameraCB)
    {
        SAFE_DELETE(buffer);
    }
    m_vpCameraCB.clear();

    SAFE_DELETE(m_pRender);
    SAFE_DELETE(m_pPrimitiveFactory);
    SAFE_DELETE(m_pModelParserFactory);
}

void GCGraphics::Initialize(Window* pWindow,int renderWidth,int renderHeight)
{
    CHECK_POINTERSNULL("Graphics Initialized with window sucessfully", "Can't initialize Graphics, Window is empty", pWindow);

    //Initializes Graphics for a window
    m_pRender = new GCRender();
    m_pRender->Initialize(pWindow, renderWidth, renderHeight);
    
    m_renderWidth = renderWidth;
    m_renderHeight = renderHeight;

    //Creates Primitive and parser instances
    m_pPrimitiveFactory = new GCPrimitiveFactory();
    m_pModelParserFactory = new GCModelParserObj();

    m_pPrimitiveFactory->Initialize();

    // Create One camera
    CreateCBCamera<GCVIEWPROJCB>();
}

void GCGraphics::StartFrame()
{
    for (auto& material : m_vMaterials)
    {
        for (auto& cbObject : material->GetObjectCBData())
        {
            cbObject->m_isUsed = false;
        }
    }
    m_pRender->PrepareDraw();

};
void GCGraphics::EndFrame()
{

    GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();

    m_pRender->PostDraw();

    for (int i = 0; i < m_vMaterials.size(); i++) {
        m_vMaterials[i]->ResetCBCount();
    }

    // Vérification des CB inutilisés dans les matériaux / CHECK FOR REMOVE CB BUFFER IN MATERIALS 
    for (auto& material : m_vMaterials)
    {
        for (auto& cbObject : material->GetObjectCBData())
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
};

void GCGraphics::InitializeGraphicsResourcesStart() {
    m_pRender->ResetCommandList(); // Reset Command List Before Resources Creation
}
void GCGraphics::InitializeGraphicsResourcesEnd() {
    m_pRender->CloseCommandList(); // Close and Execute after creation
    m_pRender->ExecuteCommandList();
    m_pRender->FlushCommandQueue();
}

ResourceCreationResult<GCTexture*> GCGraphics::CreateTexture(const std::string& filePath)
{
    GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();

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

        // Log creation success
        profiler.LogInfo("Texture created and inserted at index " + std::to_string(index));
    }
    else {
        // If no inactive slot was found, push the texture to the end
        index = m_vTextures.size(); // Index will be the last position

        m_vTextures.push_back(texture);
        m_vTextureActiveFlags.push_back(true);

        // Log creation success
        profiler.LogInfo("Texture created and appended at index " + std::to_string(index));
    }

    // Initialize the texture with the specified index
    texture->Initialize(filePath, this, index);

    // Return the result of the creation operation
    return ResourceCreationResult<GCTexture*>(true, texture);
}

ResourceCreationResult<GCShader*> GCGraphics::CreateShaderColor()
{
    GCShader* pShader = new GCShader();

    int flags = 0;
    SET_FLAG(flags, HAS_POSITION);
    SET_FLAG(flags, HAS_COLOR);

    pShader->Initialize(m_pRender, "../../../src/Render/Shaders/color.hlsl", "../../../src/Render/CsoCompiled/color", flags);
    pShader->Load();
    m_vShaders.push_back(pShader);

    return ResourceCreationResult<GCShader*>(true, pShader);
}

ResourceCreationResult<GCShader*> GCGraphics::CreateShaderTexture()
{
    GCShader* pShader = new GCShader();

    int flags = 0;
    SET_FLAG(flags, HAS_POSITION);
    SET_FLAG(flags, HAS_UV);

    pShader->Initialize(m_pRender, "../../../src/Render/Shaders/texture.hlsl", "../../../src/Render/CsoCompiled/texture", flags);
    pShader->Load();
    m_vShaders.push_back(pShader);

    return ResourceCreationResult<GCShader*>(true, pShader);
}

// Specify the path, with the name of the shader at the file creation , example : CsoCompiled/texture, texture is the name of the file in Cso Compiled Folder
ResourceCreationResult<GCShader*> GCGraphics::CreateShaderCustom(std::string& filePath, std::string& compiledShaderDestinationPath, int& flagEnabledBits)
{
    GCShader* pShader = new GCShader();
    pShader->Initialize(m_pRender, filePath, compiledShaderDestinationPath, flagEnabledBits);
    pShader->Load();

    m_vShaders.push_back(pShader);

    return ResourceCreationResult<GCShader*>(true, pShader);
}

ResourceCreationResult<GCMesh*> GCGraphics::CreateMesh(GCGeometry* pGeometry)
{
    GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();

    // Check if Geometry is valid
    if (!CheckPointersNull("Geometry loaded successfully for mesh", "Can't create mesh, Geometry is empty", pGeometry))
    {
        return ResourceCreationResult<GCMesh*>(false, nullptr);
    }

    // Find the first inactive slot in m_vMeshActiveFlags
    auto it = std::find(m_vMeshActiveFlags.begin(), m_vMeshActiveFlags.end(), false);

    // Inactive slot found
    if (it != m_vMeshActiveFlags.end())
    {
        size_t index = std::distance(m_vMeshActiveFlags.begin(), it);

        GCMesh* pMesh = new GCMesh();
        pMesh->Initialize(m_pRender, pGeometry);

        m_vMeshes.insert(m_vMeshes.begin() + index, pMesh);

        m_vMeshActiveFlags[index] = true;

        return ResourceCreationResult<GCMesh*>(true, pMesh);
    }
    // Not inactive slot 
    else
    {
        GCMesh* pMesh = new GCMesh();
        pMesh->Initialize(m_pRender, pGeometry);

        m_vMeshes.push_back(pMesh);
        m_vMeshActiveFlags.push_back(true);

        return ResourceCreationResult<GCMesh*>(true, pMesh);
    }
}

ResourceCreationResult<GCGeometry*> GCGraphics::CreateGeometryPrimitiveColor(const std::string& primitiveName, const DirectX::XMFLOAT4& color)
{
    int flagsColor = 0;
    SET_FLAG(flagsColor, HAS_POSITION);
    SET_FLAG(flagsColor, HAS_COLOR);

    // Call the unified BuildGeometry function
    GCGeometry* pGeometry = m_pPrimitiveFactory->BuildGeometry(primitiveName, color, flagsColor);

    CHECK_POINTERSNULL("Primitive Geometry with Color created successfully", "Failed to create Primitive Geometry with Color", pGeometry);

    return ResourceCreationResult<GCGeometry*>(true, pGeometry);
}

ResourceCreationResult<GCGeometry*> GCGraphics::CreateGeometryPrimitiveTexture(const std::string& primitiveName)
{
    int flagsTexture = 0;
    SET_FLAG(flagsTexture, HAS_POSITION);
    SET_FLAG(flagsTexture, HAS_UV);

    // Call the unified BuildGeometry function without color (nullptr)
    GCGeometry* pGeometry = m_pPrimitiveFactory->BuildGeometry(primitiveName, DirectX::XMFLOAT4(DirectX::Colors::Gray), flagsTexture);

    CHECK_POINTERSNULL("Primitive Geometry with Texture created successfully", "Failed to create Primitive Geometry with Texture", pGeometry);

    return ResourceCreationResult<GCGeometry*>(true, pGeometry);
}

ResourceCreationResult<GCGeometry*> GCGraphics::CreateGeometryPrimitiveCustom(const std::string& primitiveName, const DirectX::XMFLOAT4& color, int& flagEnabledBits)
{
    // Call the unified BuildGeometry function without color (nullptr)
    GCGeometry* pGeometry = m_pPrimitiveFactory->BuildGeometry(primitiveName, color, flagEnabledBits);

    CHECK_POINTERSNULL("Primitive Geometry with Custom parameters created successfully", "Failed to create Primitive Geometry with Custom parameters", pGeometry);

    return ResourceCreationResult<GCGeometry*>(true, pGeometry);
}

ResourceCreationResult<GCGeometry*> GCGraphics::CreateGeometryModelParserColor(const std::string& filePath, DirectX::XMFLOAT4 color, Extensions fileExtensionType)
{
    int flagsColor = 0;
    SET_FLAG(flagsColor, HAS_POSITION);
    SET_FLAG(flagsColor, HAS_COLOR);

    // Call the unified BuildGeometry function without color (nullptr)
    GCGeometry* pGeometry = m_pModelParserFactory->BuildModel(filePath, color, fileExtensionType, flagsColor);

    CHECK_POINTERSNULL("Geometry from Model Parser with Color created successfully", "Failed to create Geometry from Model Parser with Color", pGeometry);

    return ResourceCreationResult<GCGeometry*>(true, pGeometry);
}

ResourceCreationResult<GCGeometry*> GCGraphics::CreateGeometryModelParserCustom(const std::string& filePath, DirectX::XMFLOAT4 color, Extensions fileExtensionType, int& flagEnabledBits)
{
    // Call the unified BuildGeometry function without color (nullptr)
    GCGeometry* pGeometry = m_pModelParserFactory->BuildModel(filePath, color, fileExtensionType, flagEnabledBits);

    CHECK_POINTERSNULL("Geometry from Model Parser with Custom parameters created successfully", "Failed to create Geometry from Model Parser with Custom parameters", pGeometry);

    return ResourceCreationResult<GCGeometry*>(true, pGeometry);
}

ResourceCreationResult<GCGeometry*> GCGraphics::CreateGeometryModelParserTexture(const std::string& filePath, Extensions fileExtensionType)
{
    int flagsTexture = 0;
    SET_FLAG(flagsTexture, HAS_POSITION);
    SET_FLAG(flagsTexture, HAS_UV);

    // Call the unified BuildGeometry function without color (nullptr)
    GCGeometry* pGeometry = m_pModelParserFactory->BuildModel(filePath, DirectX::XMFLOAT4(DirectX::Colors::Gray), fileExtensionType, flagsTexture);

    CHECK_POINTERSNULL("Geometry from Model Parser with Texture created successfully", "Failed to create Geometry from Model Parser with Texture", pGeometry);

    return ResourceCreationResult<GCGeometry*>(true, pGeometry);
}

ResourceCreationResult<GCMaterial*> GCGraphics::CreateMaterial(GCShader* pShader)
{
    CHECK_POINTERSNULL("Shader loaded successfully for material", "Can't create material, Shader is empty", pShader);

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
    CHECK_POINTERSNULL("Ptr for RemoveShader is not null", "Can't remove shader, pShader is null", pShader);

    // Removes Shader both from vector and the shader itself
    auto it = std::find(m_vShaders.begin(), m_vShaders.end(), pShader);

    if (LogRemoveResource(it, "Shader", m_vShaders))
    {
        m_vShaders.erase(it);
        delete pShader;
        return true;
    }
    return false;
}

bool GCGraphics::RemoveMaterial(GCMaterial* pMaterial)
{
    CHECK_POINTERSNULL("Ptr for RemoveMaterial is not null", "Can't remove material, pMaterial is null", pMaterial);

    auto it = std::find(m_vMaterials.begin(), m_vMaterials.end(), pMaterial);

    if (LogRemoveResource(it, "Material", m_vMaterials))
    {
        m_vMaterials.erase(it);
        delete pMaterial;
        return true;
    }
    return false;
}

bool GCGraphics::RemoveMesh(GCMesh* pMesh)
{
    CHECK_POINTERSNULL("Ptr for RemoveMesh is not null", "Can't remove mesh, pMesh is null", pMesh);

    // Removes Mesh
    auto it = std::find(m_vMeshes.begin(), m_vMeshes.end(), pMesh);

    if (it != m_vMeshes.end())
    {
        if (LogRemoveResource(it, "Mesh", m_vMeshes))
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
    CHECK_POINTERSNULL("Ptr for RemoveTexture is not null", "Can't remove texture, pTexture is null", pTexture);

    // Removes Texture
    auto it = std::find(m_vTextures.begin(), m_vTextures.end(), pTexture);

    if (LogRemoveResource(it, "Texture", m_vTextures))
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

void GCGraphics::UpdateViewProjConstantBuffer(DirectX::XMFLOAT4X4 projectionMatrix, DirectX::XMFLOAT4X4 viewMatrix) {
    GCVIEWPROJCB cameraData;
    cameraData.view = viewMatrix;
    cameraData.proj = projectionMatrix;

    UpdateConstantBuffer(cameraData, m_vpCameraCB[0]);

    m_pRender->m_pCurrentViewProj = m_vpCameraCB[0];
}

//Updates a cb data of a given material using the three matrices world/view/proj
//using a count for now that'll need to be reset after each draw,might be subject to changes in the near future
void GCGraphics::UpdateWorldConstantBuffer(GCMaterial* pMaterial, DirectX::XMFLOAT4X4 worldMatrix) {

    if (pMaterial->GetCount() >= pMaterial->GetObjectCBData().size())
        pMaterial->CreateCBObject<GCWORLDCB>();

    GCWORLDCB worldData;
    worldData.world = worldMatrix;
    // Update 
    pMaterial->UpdateConstantBuffer(worldData, pMaterial->GetObjectCBData()[pMaterial->GetCount()]);

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