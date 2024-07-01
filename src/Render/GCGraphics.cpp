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

	GCShaderUploadBufferBase* pCbInstance = new GCShaderUploadBuffer<GCVIEWPROJCB>(m_pRender->Getmd3dDevice(), 1, true);
    m_pCbCameraInstances.push_back(pCbInstance);

    pCbInstance = new GCShaderUploadBuffer<GCLIGHTPROPERTIES>(m_pRender->Getmd3dDevice(), 1, true);
    m_pCbLightPropertiesInstance = pCbInstance;

    GCLIGHTPROPERTIES lightData = {};

    //GCLIGHT directionalLight;
    //directionalLight.position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f); // Pas de position pour une lumière directionnelle
    //directionalLight.direction = DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f); // Direction du soleil (exemple)
    //directionalLight.color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f); // Couleur de la lumière
    //directionalLight.spotAngle = 0.0f; // Pas d'angle pour une lumière directionnelle
    //directionalLight.lightIntensity = 1.3f;
    //directionalLight.lightType = 0; // Type de lumière directionnelle

    //GCLIGHT light1;
    //light1.position = DirectX::XMFLOAT3(-5.0f, 20.0f, -2.0f);
    //light1.direction = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
    //light1.color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
    //light1.spotAngle = 10.0f;
    //light1.lightIntensity = 1.2f;
    //light1.lightType = 1;

    //GCLIGHT light2;
    //light2.position = DirectX::XMFLOAT3(2.0f, 20.0f, -2.0f);
    //light2.direction = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
    //light2.color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
    //light2.spotAngle = 10.0f;
    //light2.lightIntensity = 1.2f;
    //light2.lightType = 1;

    // Lumière ponctuelle 2
    GCLIGHT light2;
    light2.position = DirectX::XMFLOAT3(0.0f, 3.0f, 0.0f); // Position en 2D (x, y, 0)
    light2.direction = DirectX::XMFLOAT3(0.0f, -0.5f, 0.0f); // Direction vers le bas en 2D
    light2.color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f); // Couleur de la lumière
    light2.spotAngle = 3.0f; // Angle du spot si applicable
    light2.lightIntensity = 1.2f;
    light2.lightType = 1; // Type de lumière ponctuelle


    //lightData.lights[0] = directionalLight;
    lightData.lights[1] = light2;
    //lightData.lights[2] = light1;

    GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();

    UpdateConstantBuffer(lightData, m_pCbLightPropertiesInstance);

    m_pRender->m_pCbLightPropertiesInstance = m_pCbLightPropertiesInstance;
}

void GCGraphics::StartFrame()
{
    for (auto& material : m_vMaterials)
    {
        for (auto& cbObject : material->GetCbObjectInstance())
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

    //for (auto& material : m_vMaterials)
    //    profiler.LogInfo(std::to_string(material->GetObjectCBData().size()));


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
    }
    else {
        // If no inactive slot was found, push the texture to the end
        index = m_vTextures.size(); // Index will be the last position

        m_vTextures.push_back(texture);
        m_vTextureActiveFlags.push_back(true);
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
        pMesh->Initialize(m_pRender, pGeometry, flagEnabledBits);

        m_vMeshes.insert(m_vMeshes.begin() + index, pMesh);

        m_vMeshActiveFlags[index] = true;

        return ResourceCreationResult<GCMesh*>(true, pMesh);
    }
    // Not inactive slot 
    else
    {
        GCMesh* pMesh = new GCMesh();
        pMesh->Initialize(m_pRender, pGeometry, flagEnabledBits);

        m_vMeshes.push_back(pMesh);
        m_vMeshActiveFlags.push_back(true);

        return ResourceCreationResult<GCMesh*>(true, pMesh);
    }
}

ResourceCreationResult<GCMesh*> GCGraphics::CreateMeshColor(GCGeometry* pGeometry)
{
    int flagsLightColor = 0;
    SET_FLAG(flagsLightColor, HAS_POSITION);
    SET_FLAG(flagsLightColor, HAS_COLOR);

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
        pMesh->Initialize(m_pRender, pGeometry, flagsLightColor);

        m_vMeshes.insert(m_vMeshes.begin() + index, pMesh);

        m_vMeshActiveFlags[index] = true;

        return ResourceCreationResult<GCMesh*>(true, pMesh);
    }
    // Not inactive slot 
    else
    {
        GCMesh* pMesh = new GCMesh();
        pMesh->Initialize(m_pRender, pGeometry, flagsLightColor);

        m_vMeshes.push_back(pMesh);
        m_vMeshActiveFlags.push_back(true);

        return ResourceCreationResult<GCMesh*>(true, pMesh);
    }
}

ResourceCreationResult<GCMesh*> GCGraphics::CreateMeshTexture(GCGeometry* pGeometry)
{
    int flagsLightTexture = 0;
    SET_FLAG(flagsLightTexture, HAS_POSITION);
    SET_FLAG(flagsLightTexture, HAS_UV);

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
        pMesh->Initialize(m_pRender, pGeometry, flagsLightTexture);

        m_vMeshes.insert(m_vMeshes.begin() + index, pMesh);

        m_vMeshActiveFlags[index] = true;

        return ResourceCreationResult<GCMesh*>(true, pMesh);
    }
    // Not inactive slot 
    else
    {
        GCMesh* pMesh = new GCMesh();
        pMesh->Initialize(m_pRender, pGeometry, flagsLightTexture);

        m_vMeshes.push_back(pMesh);
        m_vMeshActiveFlags.push_back(true);

        return ResourceCreationResult<GCMesh*>(true, pMesh);
    }
}

//ResourceCreationResult<GCGeometry*> GCGraphics::CreateGeometryPrimitiveColor(const GC_PRIMITIVE_ID primitiveIndex, const DirectX::XMFLOAT4& color)
//{
//    int flagsColor = 0;
//    SET_FLAG(flagsColor, HAS_POSITION);
//    SET_FLAG(flagsColor, HAS_COLOR);
//
//    // Call the unified BuildGeometry function
//    GCGeometry* pGeometry = m_pPrimitiveFactory->BuildGeometry(primitiveIndex, color, flagsColor);
//
//    CHECK_POINTERSNULL("Primitive Geometry with Color created successfully", "Failed to create Primitive Geometry with Color", pGeometry);
//
//    return ResourceCreationResult<GCGeometry*>(true, pGeometry);
//}
//
//ResourceCreationResult<GCGeometry*> GCGraphics::CreateGeometryPrimitiveTexture(const GC_PRIMITIVE_ID primitiveIndex)
//{
//    int flagsTexture = 0;
//    SET_FLAG(flagsTexture, HAS_POSITION);
//    SET_FLAG(flagsTexture, HAS_UV);
//
//    // Call the unified BuildGeometry function without color (nullptr)
//    GCGeometry* pGeometry = m_pPrimitiveFactory->BuildGeometry(primitiveIndex, DirectX::XMFLOAT4(DirectX::Colors::Gray), flagsTexture);
//
//    CHECK_POINTERSNULL("Primitive Geometry with Texture created successfully", "Failed to create Primitive Geometry with Texture", pGeometry);
//
//    return ResourceCreationResult<GCGeometry*>(true, pGeometry);
//}

ResourceCreationResult<GCGeometry*> GCGraphics::CreateGeometryPrimitive(const GC_PRIMITIVE_ID primitiveIndex, const DirectX::XMFLOAT4& color)
{
    // Call the unified BuildGeometry function without color (nullptr)
    GCGeometry* pGeometry = new GCGeometry();

    m_pPrimitiveFactory->BuildGeometry(primitiveIndex, color, pGeometry);

    CHECK_POINTERSNULL("Primitive Geometry with Custom parameters created successfully", "Failed to create Primitive Geometry with Custom parameters", pGeometry);

    return ResourceCreationResult<GCGeometry*>(true, pGeometry);
}

//ResourceCreationResult<GCGeometry*> GCGraphics::CreateGeometryModelParserColor(const std::string& filePath, DirectX::XMFLOAT4 color, Extensions fileExtensionType)
//{
//    int flagsColor = 0;
//    SET_FLAG(flagsColor, HAS_POSITION);
//    SET_FLAG(flagsColor, HAS_COLOR);
//
//    // Call the unified BuildGeometry function without color (nullptr)
//    GCGeometry* pGeometry = m_pModelParserFactory->BuildModel(filePath, color, fileExtensionType, flagsColor);
//
//    CHECK_POINTERSNULL("Geometry from Model Parser with Color created successfully", "Failed to create Geometry from Model Parser with Color", pGeometry);
//
//    return ResourceCreationResult<GCGeometry*>(true, pGeometry);
//}
//
//ResourceCreationResult<GCGeometry*> GCGraphics::CreateGeometryModelParserCustom(const std::string& filePath, DirectX::XMFLOAT4 color, Extensions fileExtensionType, int& flagEnabledBits)
//{
//    // Call the unified BuildGeometry function without color (nullptr)
//    GCGeometry* pGeometry = m_pModelParserFactory->BuildModel(filePath, color, fileExtensionType, flagEnabledBits);
//
//    CHECK_POINTERSNULL("Geometry from Model Parser with Custom parameters created successfully", "Failed to create Geometry from Model Parser with Custom parameters", pGeometry);
//
//    return ResourceCreationResult<GCGeometry*>(true, pGeometry);
//}

ResourceCreationResult<GCGeometry*> GCGraphics::CreateGeometryModelParser(const std::string& filePath, DirectX::XMFLOAT4 color, Extensions fileExtensionType)
{
    int flagsTexture = 0;
    SET_FLAG(flagsTexture, HAS_POSITION);
    SET_FLAG(flagsTexture, HAS_UV);

    // Call the unified BuildGeometry function without color (nullptr)
    GCGeometry* pGeometry = new GCGeometry;

    m_pModelParserFactory->BuildModel(filePath, DirectX::XMFLOAT4(DirectX::Colors::Gray), fileExtensionType, pGeometry);

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

// Update per camera constant buffer
void GCGraphics::UpdateViewProjConstantBuffer(DirectX::XMFLOAT4X4 projectionMatrix, DirectX::XMFLOAT4X4 viewMatrix) {
    GCVIEWPROJCB cameraData;
    cameraData.view = viewMatrix;
    cameraData.proj = projectionMatrix;

    UpdateConstantBuffer(cameraData, m_pCbCameraInstances[0]);

    m_pRender->m_pCbCurrentViewProjInstance = m_pCbCameraInstances[0];
}

// Update per object constant buffer
void GCGraphics::UpdateWorldConstantBuffer(GCMaterial* pMaterial, DirectX::XMFLOAT4X4 worldMatrix) {

    if (pMaterial->GetCount() >= pMaterial->GetCbObjectInstance().size()) {
        pMaterial->AddCbPerObject<GCWORLDCB>();
    }
    GCWORLDCB worldData;
    worldData.world = worldMatrix;
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