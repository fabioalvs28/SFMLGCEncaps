#include "framework.h"

GCGraphics::GCGraphics()
{
    m_pRender = nullptr;
    m_pPrimitiveFactory = nullptr;
    m_pModelParserFactory = nullptr;

    m_meshId = 0;
    m_shaderId = 0;
    m_materialId = 0;
    m_textureId = 0;

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
    GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();

    CHECK_POINTERSNULL(profiler, "Graphics Initialized with window sucessfully", "Can't initialize Graphics, Window is empty", pWindow);

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

GCTexture* GCGraphics::CreateTexture(const std::string& filePath) 
{
    //Creates and initializes a texture using a path
    GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();

	GCTexture* texture = new GCTexture();
	texture->Initialize(filePath, this);
    m_vTextures.push_back(texture);
	return texture;
}

GCShader* GCGraphics::CreateShaderColor() 
{
    GCShader* pShader = new GCShader();

    int flags = 0;
    SET_FLAG(flags, HAS_POSITION);
    SET_FLAG(flags, HAS_COLOR);

    pShader->Initialize(m_pRender, "../../../src/Render/Shaders/color.hlsl", "../../../src/Render/CsoCompiled/color", flags);
    pShader->Load();
    m_vShaders.push_back(pShader);
    m_shaderId++;

    return pShader;   
}

GCShader* GCGraphics::CreateShaderTexture() 
{
    GCShader* pShader = new GCShader();

    int flags = 0;
    SET_FLAG(flags, HAS_POSITION);
    SET_FLAG(flags, HAS_UV);

    pShader->Initialize(m_pRender, "../../../src/Render/Shaders/texture.hlsl", "../../../src/Render/CsoCompiled/texture", flags);
    pShader->Load();
    m_vShaders.push_back(pShader);
    m_shaderId++;

    return pShader;
}


// Specify the path, with the name of the shader at the file creation , example : CsoCompiled/texture, texture is the name of the file in Cso Compiled Folder
GCShader* GCGraphics::CreateShaderCustom(std::string& filePath, std::string& compiledShaderDestinationPath, int& flagEnabledBits) {
    GCShader* pShader = new GCShader();
    pShader->Initialize(m_pRender, filePath, compiledShaderDestinationPath, flagEnabledBits);
    pShader->Load();

    m_vShaders.push_back(pShader);
    m_shaderId++;

    return pShader;
}

GCMesh* GCGraphics::CreateMesh(GCGeometry* pGeometry)
{
    GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();

    CHECK_POINTERSNULL(profiler,"Geometry loaded successfully for mesh","Can't create mesh, Geometry is empty",pGeometry);

    GCMesh* pMesh = new GCMesh();
    pMesh->Initialize(m_pRender, pGeometry);

    m_vMeshes.push_back(pMesh);
    return pMesh;
}

GCGeometry* GCGraphics::CreateGeometryPrimitiveColor(const std::string& primitiveName, const DirectX::XMFLOAT4& color)
{
    int flagsColor = 0;
    SET_FLAG(flagsColor, HAS_POSITION);
    SET_FLAG(flagsColor, HAS_COLOR);

    GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();

    // Call the unified BuildGeometry function
    GCGeometry* pGeometry = m_pPrimitiveFactory->BuildGeometry(primitiveName, color, flagsColor);

    CHECK_POINTERSNULL(profiler, "Primitive Geometry with Color created successfully", "Failed to create Primitive Geometry with Color", pGeometry);

    return pGeometry;
}

GCGeometry* GCGraphics::CreateGeometryPrimitiveTexture(const std::string& primitiveName)
{
    int flagsTexture = 0;
    SET_FLAG(flagsTexture, HAS_POSITION);
    SET_FLAG(flagsTexture, HAS_UV);

    GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();


    // Call the unified BuildGeometry function without color (nullptr)
    GCGeometry* pGeometry = m_pPrimitiveFactory->BuildGeometry(primitiveName, DirectX::XMFLOAT4(DirectX::Colors::Gray), flagsTexture);

    CHECK_POINTERSNULL(profiler, "Primitive Geometry with Texture created successfully", "Failed to create Primitive Geometry with Texture", pGeometry);

    return pGeometry;
}

GCGeometry* GCGraphics::CreateGeometryModelParserColor(const std::string& filePath, DirectX::XMFLOAT4 color, Extensions fileExtensionType)
{
    int flagsColor = 0;
    SET_FLAG(flagsColor, HAS_POSITION);
    SET_FLAG(flagsColor, HAS_COLOR);

    GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();

    // Call the unified BuildGeometry function without color (nullptr)
    GCGeometry* pGeometry = m_pModelParserFactory->BuildModel(filePath, color, fileExtensionType, flagsColor);

    CHECK_POINTERSNULL(profiler, "Primitive Geometry with Texture created successfully", "Failed to create Primitive Geometry with Texture", pGeometry);

    return pGeometry;
}

GCGeometry* GCGraphics::CreateGeometryModelParserTexture(const std::string& filePath, Extensions fileExtensionType)
{
    int flagsTexture = 0;
    SET_FLAG(flagsTexture, HAS_POSITION);
    SET_FLAG(flagsTexture, HAS_UV);

    GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();

    // Call the unified BuildGeometry function without color (nullptr)
    GCGeometry* pGeometry = m_pModelParserFactory->BuildModel(filePath, DirectX::XMFLOAT4(DirectX::Colors::Gray), fileExtensionType, flagsTexture);

    CHECK_POINTERSNULL(profiler, "Primitive Geometry with Texture created successfully", "Failed to create Primitive Geometry with Texture", pGeometry);

    return pGeometry;
}


GCMaterial* GCGraphics::CreateMaterial(GCShader* pShader) {

    GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();


    CHECK_POINTERSNULL(profiler,"Shader loaded successfully for material","Can't create material, Shader is empty", pShader);

    GCMaterial* material = new GCMaterial();
    material->Initialize(pShader);
    m_vMaterials.push_back(material);
    m_materialId++;
    return material;
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

void GCGraphics::RemoveShader(GCShader* pShader) 
{
    //Removes Shader both from vector and the shader itself
    auto it = std::find(m_vShaders.begin(), m_vShaders.end(), pShader);

    GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();

    if (it == m_vShaders.end())
    {
        OutputDebugString(L"Shader not found, can't remove it");
        profiler.LogWarning("Shader not found, can't remove it");
    }
    else
    {
        OutputDebugString(L"Shader removed successfully");
        profiler.LogInfo("Shader removed successfully");
    }

    m_vShaders.erase(it);
    delete pShader;
}

void GCGraphics::RemoveMaterial(GCMaterial* pMaterial) 
{
    //Removes material from the vector then the material itself
    auto it = std::find(m_vMaterials.begin(), m_vMaterials.end(), pMaterial);

    GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();

    if (it == m_vMaterials.end())
    {
        OutputDebugString(L"Material not found, can't remove it");
        profiler.LogWarning("Material not found, can't remove it");
    }
    else 
    {
        OutputDebugString(L"Material removed successfully");
        profiler.LogInfo("Material removed successfully");
    }

    m_vMaterials.erase(it);
    delete pMaterial;
}

void GCGraphics::RemoveMesh(GCMesh* pMesh) 
{
    //Removes Mesh
    auto it = std::find(m_vMeshes.begin(), m_vMeshes.end(), pMesh);

    GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();

    if (it == m_vMeshes.end())
    {
        OutputDebugString(L"Mesh not found, can't remove it");
        profiler.LogWarning("Mesh not found, can't remove it");
    }
    else
    {
        OutputDebugString(L"Mesh removed successfully");
        profiler.LogInfo("Mesh removed successfully");
    }

    m_vMeshes.erase(it);
    delete pMesh;
}

void GCGraphics::RemoveTexture(GCTexture* pTexture)
{
    //Removes Texture
    auto it = std::find(m_vTextures.begin(), m_vTextures.end(), pTexture);

    GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();

    if (it == m_vTextures.end())
    {
        OutputDebugString(L"Texture not found, can't remove it");
        profiler.LogWarning("Texture not found, can't remove it");
    }
    else
    {
        OutputDebugString(L"Texture removed successfully");
        profiler.LogInfo("Texture removed successfully");
    }

    m_vTextures.erase(it);
    delete pTexture;
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

