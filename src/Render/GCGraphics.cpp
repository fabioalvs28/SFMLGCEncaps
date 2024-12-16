#include "pch.h"

GCGraphics::GCGraphics()
    : m_pRender(nullptr),
    m_pPrimitiveFactory(nullptr),
    m_pModelParserFactory(nullptr),
    m_pCbLightPropertiesInstance(nullptr)
{
    m_lTextureActiveFlags.clear();
    m_lTextures.clear();
    m_vShaders.clear();
    m_vMaterials.clear();
    m_vMeshes.clear();
    m_cbCameraInstances.clear();
}

GCGraphics::~GCGraphics()
{
    for (auto shader : m_vShaders)
    {
        GC_DELETE(shader);
    }
    m_vShaders.clear();

    for (auto material : m_vMaterials)
    {
        GC_DELETE(material);
    }
    m_vMaterials.clear();

    for (auto mesh : m_vMeshes)
    {
        GC_DELETE(mesh);
    }
    m_vMeshes.clear();

    for (auto texture : m_lTextures)
    {
        GC_DELETE(texture);
    }
    m_lTextures.clear();

    for (auto buffer : m_cbCameraInstances)
    {
        GC_DELETE(buffer);
    }
    m_cbCameraInstances.clear();

    GC_DELETE(m_pRender);
    GC_DELETE(m_pPrimitiveFactory);
    GC_DELETE(m_pModelParserFactory);
}

bool GCGraphics::Initialize(Window* pWindow, int renderWidth,int renderHeight)
{
    if (GC_CHECK_POINTERSNULL("Graphics Initialized with window sucessfully", "Can't initialize Graphics, Window is empty", pWindow) == false)
        return false;

    //Creates Primitive and parser instances
    m_pPrimitiveFactory = new GCPrimitiveFactory();
    m_pModelParserFactory = new GCModelParserObj();
    m_pFontGeometryLoader = new GCFontGeometryLoader();
    m_pFontGeometryLoader->GenerateFontMetadata("../../../res/Fonts/LetterUV.txt");

    m_pPrimitiveFactory->Initialize();

    //Initializes Graphics for a window
    m_pRender = new GCRenderContext();
    m_pRender->Initialize(pWindow, renderWidth, renderHeight, this);

	GCShaderUploadBufferBase* pCbInstance = new GCShaderUploadBuffer<GCVIEWPROJCB>(m_pRender->GetRenderResources()->Getmd3dDevice(), 1, true);
    m_cbCameraInstances.push_back(pCbInstance);

    return true;
}

bool GCGraphics::StartFrame()
{
    m_pRender->m_materialsUsedInFrame.clear();

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

    m_pRender->CompleteDraw();

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
    HRESULT hr = m_pRender->GetRenderResources()->GetCommandList()->Reset(m_pRender->GetRenderResources()->GetCommandAllocator(), nullptr);
    if (!GC_CHECK_HRESULT(hr, "CommandList->Reset()")) {
        return false;
    };

    return true;
}
bool GCGraphics::InitializeGraphicsResourcesEnd() {
    HRESULT hr;
    hr = m_pRender->GetRenderResources()->GetCommandList()->Close();
    if (!GC_CHECK_HRESULT(hr, "CommandList()->Close()")) {
        return false;
    };
    m_pRender->ExecuteCommandList();

    if (!m_pRender->FlushCommandQueue())
        return false;

    return true;
}


GC_RESOURCE_CREATION_RESULT<GCTexture*> GCGraphics::CreateTexture(const std::string& filePath) {
    GCGraphicsLogger& profiler = GCGraphicsLogger::GetInstance();

    // Creates and initializes a texture using a path
    GCTexture* texture = new GCTexture();

    // Find the first inactive slot in m_lTextureActiveFlags
    auto it = std::find(m_lTextureActiveFlags.begin(), m_lTextureActiveFlags.end(), false);

    size_t index;
    auto listIt = m_lTextures.begin();

    if (it != m_lTextureActiveFlags.end()) {
        // Calculate the index of the found position
        index = std::distance(m_lTextureActiveFlags.begin(), it);

        // Advance the iterator to the same position in m_lTextures
        std::advance(listIt, index);

        // Insert the texture at the found index
        m_lTextures.insert(listIt, texture);

        // Set the corresponding flag to true
        *it = true;
    }
    else {
        // If no inactive slot was found, push the texture to the end
        index = m_lTextures.size(); // Index will be the last position

        m_lTextures.push_back(texture);
        m_lTextureActiveFlags.push_back(true);
    }

    INT intIndex = static_cast<INT>(index);

    // Initialize the texture with the specified index
    GC_GRAPHICS_ERROR errorState = texture->Initialize(filePath, this, intIndex);
    if (errorState != GCRENDER_SUCCESS_OK) {
        return GC_RESOURCE_CREATION_RESULT<GCTexture*>(false, nullptr, errorState);
    }

    // Return the result of the creation operation
    return GC_RESOURCE_CREATION_RESULT<GCTexture*>(true, texture, errorState);
}

GC_RESOURCE_CREATION_RESULT<GCShader*> GCGraphics::CreateShaderColor()
{
    GCShader* pShader = new GCShader();

    int vertexFlags = 0;
    GC_SET_FLAG(vertexFlags, GC_VERTEX_POSITION);
    GC_SET_FLAG(vertexFlags, GC_VERTEX_COLOR);

    int rootParametersFlag = 0;
    GC_SET_FLAG(rootParametersFlag, GC_ROOT_PARAMETER_CB0);
    GC_SET_FLAG(rootParametersFlag, GC_ROOT_PARAMETER_CB1);

    GC_GRAPHICS_ERROR errorState = pShader->Initialize(m_pRender, "../../../res/Shaders/color.hlsl", "../../../res/CsoCompiled/color", vertexFlags, D3D12_CULL_MODE_BACK, rootParametersFlag);
    if (errorState != 0)
        return GC_RESOURCE_CREATION_RESULT<GCShader*>(false, nullptr, errorState);
    errorState = pShader->Load();
    if (errorState != 0)
        return GC_RESOURCE_CREATION_RESULT<GCShader*>(false, nullptr, errorState);

    m_vShaders.push_back(pShader);

    return GC_RESOURCE_CREATION_RESULT<GCShader*>(true, pShader, errorState);
}

GC_RESOURCE_CREATION_RESULT<GCShader*> GCGraphics::CreateShaderTexture()
{
    GCShader* pShader = new GCShader();

    int vertexFlags = 0;
    GC_SET_FLAG(vertexFlags, GC_VERTEX_POSITION);
    GC_SET_FLAG(vertexFlags, GC_VERTEX_UV);

    int rootParametersFlag = 0;
    GC_SET_FLAG(rootParametersFlag, GC_ROOT_PARAMETER_CB0);
    GC_SET_FLAG(rootParametersFlag, GC_ROOT_PARAMETER_CB1);
    GC_SET_FLAG(rootParametersFlag, GC_ROOT_PARAMETER_DESCRIPTOR_TABLE_SLOT1);

    GC_GRAPHICS_ERROR errorState = pShader->Initialize(m_pRender, "../../../res/Shaders/texture.hlsl", "../../../res/CsoCompiled/texture", vertexFlags, D3D12_CULL_MODE_BACK, rootParametersFlag);
    GCGraphicsLogger& profiler = GCGraphicsLogger::GetInstance();
    if (errorState != 0)
        return GC_RESOURCE_CREATION_RESULT<GCShader*>(false, nullptr, errorState);
    errorState = pShader->Load();
    if (errorState != 0)
        return GC_RESOURCE_CREATION_RESULT<GCShader*>(false, nullptr, errorState);

    m_vShaders.push_back(pShader);

    return GC_RESOURCE_CREATION_RESULT<GCShader*>(true, pShader, errorState);
}

// Specify the path, with the name of the shader at the file creation , example : CsoCompiled/texture, texture is the name of the file in Cso Compiled Folder
GC_RESOURCE_CREATION_RESULT<GCShader*> GCGraphics::CreateShaderCustom(std::string& filePath, std::string& compiledShaderDestinationPath, int& flagEnabledBits, D3D12_CULL_MODE cullMode, int flagRootParameters)
{
    GCShader* pShader = new GCShader();

    GC_GRAPHICS_ERROR errorState = pShader->Initialize(m_pRender, filePath, compiledShaderDestinationPath, flagEnabledBits, cullMode, flagRootParameters);
    if (errorState != 0)
        GC_RESOURCE_CREATION_RESULT<GCShader*>(false, nullptr, errorState);
    errorState = pShader->Load();
    if (errorState != 0)
        GC_RESOURCE_CREATION_RESULT<GCShader*>(false, nullptr, errorState);

    m_vShaders.push_back(pShader);

    return GC_RESOURCE_CREATION_RESULT<GCShader*>(true, pShader, errorState);
}

GC_RESOURCE_CREATION_RESULT<GCMesh*> GCGraphics::CreateMeshCustom(GCGeometry* pGeometry, int& flagEnabledBits)
{
    if (GC_CHECK_POINTERSNULL("Geometry loaded successfully for mesh", "Can't create mesh, Geometry is empty", pGeometry) == false)
    {
        return GC_RESOURCE_CREATION_RESULT<GCMesh*>(false, nullptr, GCRENDER_ERROR_POINTER_NULL);
    }

    GCMesh* pMesh = new GCMesh();

    GC_GRAPHICS_ERROR errorState = pMesh->Initialize(m_pRender, pGeometry, flagEnabledBits);
    if (errorState != 0)
        return GC_RESOURCE_CREATION_RESULT<GCMesh*>(false, nullptr, errorState);

    m_vMeshes.push_back(pMesh);

    return GC_RESOURCE_CREATION_RESULT<GCMesh*>(true, pMesh, errorState);

}

GC_RESOURCE_CREATION_RESULT<GCMesh*> GCGraphics::CreateMeshColor(GCGeometry* pGeometry)
{
    int flagsLightColor = 0;
    GC_SET_FLAG(flagsLightColor, GC_VERTEX_POSITION);
    GC_SET_FLAG(flagsLightColor, GC_VERTEX_COLOR);

    // Check if Geometry is valid
    if (GC_CHECK_POINTERSNULL("Geometry loaded successfully for mesh", "Can't create mesh, Geometry is empty", pGeometry) == false)
        return GC_RESOURCE_CREATION_RESULT<GCMesh*>(false, nullptr, GCRENDER_ERROR_POINTER_NULL);

    GCMesh* pMesh = new GCMesh();

    GC_GRAPHICS_ERROR errorState = pMesh->Initialize(m_pRender, pGeometry, flagsLightColor);
    if (errorState != 0)
        return GC_RESOURCE_CREATION_RESULT<GCMesh*>(false, nullptr, errorState);

    m_vMeshes.push_back(pMesh);

    return GC_RESOURCE_CREATION_RESULT<GCMesh*>(true, pMesh, errorState);
}

GC_RESOURCE_CREATION_RESULT<GCMesh*> GCGraphics::CreateMeshTexture(GCGeometry* pGeometry)
{
    int flagsLightTexture = 0;
    GC_SET_FLAG(flagsLightTexture, GC_VERTEX_POSITION);
    GC_SET_FLAG(flagsLightTexture, GC_VERTEX_UV);

    // Check if Geometry is valid
    if (GC_CHECK_POINTERSNULL("Geometry loaded successfully for mesh", "Can't create mesh, Geometry is empty", pGeometry) == false)
        return GC_RESOURCE_CREATION_RESULT<GCMesh*>(false, nullptr, GCRENDER_ERROR_POINTER_NULL);

    GCMesh* pMesh = new GCMesh();

    GC_GRAPHICS_ERROR errorState = pMesh->Initialize(m_pRender, pGeometry, flagsLightTexture);
    if (errorState != 0)
        return GC_RESOURCE_CREATION_RESULT<GCMesh*>(false, nullptr, errorState);

    m_vMeshes.push_back(pMesh);

    return GC_RESOURCE_CREATION_RESULT<GCMesh*>(true, pMesh, errorState);
}

GC_RESOURCE_CREATION_RESULT<GCGeometry*> GCGraphics::CreateGeometryPrimitive(const GC_PRIMITIVE_ID primitiveIndex, const DirectX::XMFLOAT4& color)
{
    GCGeometry* pGeometry = new GCGeometry();

    if (GC_CHECK_POINTERSNULL("Pointer geometry not null", "Pointer geometry null", pGeometry) == false)
        return GC_RESOURCE_CREATION_RESULT<GCGeometry*>(false, nullptr, GCRENDER_ERROR_POINTER_NULL);

    GC_GRAPHICS_ERROR errorState = m_pPrimitiveFactory->BuildGeometry(primitiveIndex, color, pGeometry);
    if (errorState != 0)
        return GC_RESOURCE_CREATION_RESULT<GCGeometry*>(false, nullptr, errorState);



    return GC_RESOURCE_CREATION_RESULT<GCGeometry*>(true, pGeometry, errorState);
}

GC_RESOURCE_CREATION_RESULT<GCGeometry*> GCGraphics::CreateGeometryModelParser(const std::string& filePath, DirectX::XMFLOAT4 color, GC_EXTENSIONS fileExtensionType)
{
    GCGeometry* pGeometry = new GCGeometry;

    if (GC_CHECK_POINTERSNULL("Pointer geometry not null", "Pointer geometry null", pGeometry) == false)
        return GC_RESOURCE_CREATION_RESULT<GCGeometry*>(false, nullptr, GCRENDER_ERROR_POINTER_NULL);

    GC_GRAPHICS_ERROR errorState = m_pModelParserFactory->BuildModel(filePath, DirectX::XMFLOAT4(DirectX::Colors::Gray), fileExtensionType, pGeometry);
    if (errorState != 0)
        return GC_RESOURCE_CREATION_RESULT<GCGeometry*>(false, nullptr, errorState);



    return GC_RESOURCE_CREATION_RESULT<GCGeometry*>(true, pGeometry, errorState);
}

GC_RESOURCE_CREATION_RESULT<GCMaterial*> GCGraphics::CreateMaterial(GCShader* pShader)
{
    if (GC_CHECK_POINTERSNULL("Shader loaded successfully for material", "Can't create material, Shader is empty", pShader) == false)
        return GC_RESOURCE_CREATION_RESULT<GCMaterial*>(false, nullptr, GCRENDER_ERROR_POINTER_NULL);

    GCMaterial* material = new GCMaterial();

    GC_GRAPHICS_ERROR errorState = material->Initialize(pShader);
    if (errorState != 0)
        return GC_RESOURCE_CREATION_RESULT<GCMaterial*>(false, nullptr, errorState);

    material->m_materialId = static_cast<float>(m_vMaterials.size());
    m_vMaterials.push_back(material);

    return GC_RESOURCE_CREATION_RESULT<GCMaterial*>(true, material, errorState);
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

std::list<GCTexture*> GCGraphics::GetTextures() 
{
    return m_lTextures;
}

GC_GRAPHICS_ERROR GCGraphics::RemoveShader(GCShader* pShader)
{
    if (GC_CHECK_POINTERSNULL("Ptr for RemoveShader is not null", "Can't remove shader, pShader is null", pShader) == false)
        return GCRENDER_ERROR_POINTER_NULL;

    // Removes Shader both from vector and the shader itself
    auto it = std::find(m_vShaders.begin(), m_vShaders.end(), pShader);

    if (GC_LOG_REMOVE_RESOURCE(it, "Shader", m_vShaders))
    {
        m_vShaders.erase(it);
        delete pShader;
        return GCRENDER_SUCCESS_OK;
    }
    return GCRENDER_ERROR_RESOURCE_TO_REMOVE_DONT_FIND;
}

GC_GRAPHICS_ERROR GCGraphics::RemoveMaterial(GCMaterial* pMaterial)
{
    if (GC_CHECK_POINTERSNULL("Ptr for RemoveMaterial is not null", "Can't remove material, pMaterial is null", pMaterial) == false)
        return GCRENDER_ERROR_POINTER_NULL;

    auto it = std::find(m_vMaterials.begin(), m_vMaterials.end(), pMaterial);

    if (GC_LOG_REMOVE_RESOURCE(it, "Material", m_vMaterials))
    {
        m_vMaterials.erase(it);
        delete pMaterial;
        return GCRENDER_SUCCESS_OK;
    }
    return GCRENDER_ERROR_RESOURCE_TO_REMOVE_DONT_FIND;
}

GC_GRAPHICS_ERROR GCGraphics::RemoveMesh(GCMesh* pMesh)
{
    if (GC_CHECK_POINTERSNULL("Ptr for RemoveMesh is not null", "Can't remove mesh, pMesh is null", pMesh) == false)
        return GCRENDER_ERROR_POINTER_NULL;

    // Removes Mesh
    auto it = std::find(m_vMeshes.begin(), m_vMeshes.end(), pMesh);

    if (GC_LOG_REMOVE_RESOURCE(it, "Mesh", m_vMeshes))
    {
        m_vMeshes.erase(it);
        delete pMesh;
        return GCRENDER_SUCCESS_OK;
    }

    return GCRENDER_ERROR_RESOURCE_TO_REMOVE_DONT_FIND;
}

GC_GRAPHICS_ERROR GCGraphics::RemoveTexture(GCTexture* pTexture) {
    if (pTexture == nullptr) {
        return GCRENDER_ERROR_POINTER_NULL;
    }

    // Removes Texture
    auto it = std::find(m_lTextures.begin(), m_lTextures.end(), pTexture);

    if (it != m_lTextures.end()) {
        size_t index = std::distance(m_lTextures.begin(), it);

        auto flagIt = m_lTextureActiveFlags.begin();
        std::advance(flagIt, index);

        if (flagIt != m_lTextureActiveFlags.end()) {
            *flagIt = false;
        }

        delete pTexture;
        m_lTextures.erase(it);

        return GCRENDER_SUCCESS_OK;
    }
    return GCRENDER_ERROR_RESOURCE_TO_REMOVE_DONT_FIND;
}

bool GCGraphics::UpdateViewProjConstantBuffer(DirectX::XMMATRIX& projectionMatrix, DirectX::XMMATRIX& viewMatrix)
{
    GCVIEWPROJCB cameraData;

    DirectX::XMStoreFloat4x4( &cameraData.view, viewMatrix );
    DirectX::XMStoreFloat4x4( &cameraData.proj,projectionMatrix);

    UpdateConstantBuffer(cameraData, m_cbCameraInstances[0]);

    m_pRender->m_pCbCurrentViewProjInstance = m_cbCameraInstances[0];

    return true;
}

// Update per camera constant buffer
bool GCGraphics::CreateViewProjConstantBuffer(const DirectX::XMFLOAT3& cameraPosition, const DirectX::XMFLOAT3& cameraTarget, const DirectX::XMFLOAT3& cameraUp,
    float fieldOfView,
    float aspectRatio,
    float nearZ,
    float farZ,
    float viewWidth,  
    float viewHeight,  
    GC_PROJECTION_TYPE projType,
    DirectX::XMMATRIX& projectionMatrix,
    DirectX::XMMATRIX& viewMatrix)
{
    DirectX::XMVECTOR camPos = DirectX::XMLoadFloat3(&cameraPosition);
    DirectX::XMVECTOR camTarget = XMLoadFloat3(&cameraTarget);
    DirectX::XMVECTOR camUp = XMLoadFloat3(&cameraUp);

    DirectX::XMMATRIX viewMatrixXM = DirectX::XMMatrixLookAtLH(camPos, camTarget, camUp);

    DirectX::XMMATRIX projectionMatrixXM;

    if (projType == PERSPECTIVE) {
        projectionMatrixXM = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearZ, farZ);
    }
    else {
        projectionMatrixXM = DirectX::XMMatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ);
    }

    DirectX::XMMATRIX transposedProjectionMatrix = DirectX::XMMatrixTranspose(projectionMatrixXM);
    DirectX::XMMATRIX transposedViewMatrix = DirectX::XMMatrixTranspose(viewMatrixXM);

    // Output
    projectionMatrix = transposedProjectionMatrix;
    viewMatrix = transposedViewMatrix;

    // Cb buffer
    GCVIEWPROJCB cameraData;
    DirectX::XMStoreFloat4x4(&cameraData.view, viewMatrix);
    DirectX::XMStoreFloat4x4(&cameraData.proj, projectionMatrix);
    UpdateConstantBuffer(cameraData, m_cbCameraInstances[0]);

    m_pRender->m_pCbCurrentViewProjInstance = m_cbCameraInstances[0];

    return true;
}

// Update per object constant buffer
bool GCGraphics::UpdateWorldConstantBuffer(GCMaterial* pMaterial, DirectX::XMMATRIX& worldMatrix, float meshId)
{
    //if (GC_CHECK_POINTERSNULL("Ptr for Update World Constant Buffer is not null", "Ptr for UpdateMaterialProperties is null", pMaterial) == false)
        //return false;

    if (pMaterial->GetCount() >= pMaterial->GetCbObjectInstance().size()) {
        pMaterial->AddCbPerObject<GCWORLDCB>();
    }

    GCWORLDCB worldData;
    //Additional scaling for Screen Ratio not equilibrate, not ponderate
    if (m_pRender->GetRenderMode() == 0)//2D
    {
        //DirectX::XMMATRIX matrix = UpdateScalingRatio(worldMatrix);

        DirectX::XMStoreFloat4x4(&worldData.world, worldMatrix);
    }
    else if (m_pRender->GetRenderMode() == 1)//3D
    {
        DirectX::XMStoreFloat4x4(&worldData.world, worldMatrix);
    }

    worldData.objectId = meshId;
    worldData.materialId = pMaterial->m_materialId;

    // Update 
    pMaterial->UpdateConstantBuffer(worldData, pMaterial->GetCbObjectInstance()[pMaterial->GetCount()]);

    return true;
}

void GCGraphics::UpdateConstantBuffer(const GCSHADERCB& objectData, GCShaderUploadBufferBase* uploadBufferInstance)
{
    uploadBufferInstance->CopyData(0, objectData);
}

DirectX::XMFLOAT4X4 GCGraphics::ToPixel(int pixelX, int pixelY, DirectX::XMFLOAT4X4& proj, DirectX::XMFLOAT4X4& view) {
    DirectX::XMFLOAT3 worldPos = GCUtils::PixelToWorld(static_cast<float>(pixelX), static_cast<float>(pixelY), m_pRender->GetRenderResources()->GetRenderWidth(), m_pRender->GetRenderResources()->GetRenderHeight(), proj, view);

    DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslation(worldPos.x, worldPos.y, worldPos.z);

    DirectX::XMFLOAT4X4 worldMatrix;
    DirectX::XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixTranspose(translationMatrix));

    return worldMatrix;
}

void GCGraphics::Resize(int width, int height) {
    m_pRender->GetRenderResources()->ResizeRender(width, height);
    m_pRender->OnResize();
}

bool GCGraphics::UpdateMaterialProperties(GCMaterial* pMaterial, GCMATERIALPROPERTIES& objectData)
{
    if (GC_CHECK_POINTERSNULL("Ptr for Update Material Properties is not null", "Ptr for UpdateMaterialProperties is null", pMaterial) == false)
        return false;

    pMaterial->ambientLightColor = objectData.ambientLightColor;
    pMaterial->ambient = objectData.ambient;
    pMaterial->diffuse = objectData.diffuse;
    pMaterial->specular = objectData.specular;
    pMaterial->shininess = objectData.shininess;

    UpdateConstantBuffer(objectData, pMaterial->GetCbMaterialPropertiesInstance());
    return true;
}

bool GCGraphics::UpdateMaterialProperties(GCMaterial* pMaterial, DirectX::XMFLOAT4& ambientLightColor, DirectX::XMFLOAT4& ambient, DirectX::XMFLOAT4& diffuse, DirectX::XMFLOAT4& specular, float shininess)
{
    if (GC_CHECK_POINTERSNULL("Ptr for Update Material Properties is not null", "Ptr for UpdateMaterialProperties is null", pMaterial) == false)
        return false;

    GCMATERIALPROPERTIES materialData;
    materialData.ambientLightColor = ambientLightColor;
    materialData.ambient = ambient;
    materialData.diffuse = diffuse;
    materialData.specular = specular;
    materialData.shininess = shininess;

    pMaterial->ambientLightColor = ambientLightColor;
    pMaterial->ambient = ambient;
    pMaterial->diffuse = diffuse;
    pMaterial->specular = specular;
    pMaterial->shininess = shininess;

    UpdateConstantBuffer(materialData, pMaterial->GetCbMaterialPropertiesInstance());
    return true;
}

bool GCGraphics::UpdateLights(std::vector<GCLIGHT>& objectData) {
    size_t count = objectData.size();
    m_pRender->m_pCbLightPropertiesInstance->CopyData(0, objectData.data(), sizeof(GCLIGHT)*count);

    return true;
}
