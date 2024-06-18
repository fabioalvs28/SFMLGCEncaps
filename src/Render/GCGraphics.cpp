#include "framework.h"

GCGraphics::GCGraphics()
{
    m_pRender = nullptr;
}

void GCGraphics::Initialize(Window* pWindow)
{
    //Initializes Graphics for a window
    m_pRender = new GCRender();
    m_pRender->Initialize(this, pWindow);

    //Creates Primitive and parser instances
    m_pPrimitiveFactory = new GCPrimitiveFactory();
    m_pModelParserFactory = new GCModelParser();
}

GCTexture* GCGraphics::CreateTexture(const std::string& filePath) 
{
    //Creates and initializes a texture using a path
    std::wstring wideFilePath(filePath.begin(), filePath.end());
    GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();

    if (_waccess(wideFilePath.c_str(), 0) == -1)
    {
        OutputDebugString((L"Texture file not found: " + wideFilePath + L"\n").c_str());
        profiler.LogWarning("Texture file not found: " + filePath);
    }
    else
    {
        OutputDebugString((L"Texture file: " + wideFilePath + L" loaded sucessfully\n").c_str());
        profiler.LogInfo("Texture file : " + filePath + " loaded sucessfully");
    }
	GCTexture* texture = new GCTexture();
	texture->Initialize(filePath, this);
    m_vTextures.push_back(texture);
	return texture;
}

GCShader* GCGraphics::CreateShaderColor() 
{
    //Creates a color shader / initializes it
    //HLSLFile* shaderFile = new HLSLFile(L"Shaders\\color.hlsl");

    GCShader* pShader;
    pShader = new GCShaderColor();

    pShader->Initialize(m_pRender, "../../../src/Render/Shaders/color.hlsl", "../../../src/Render/CsoCompiled/color", STEnum::color);

    m_vShaders.push_back(pShader);
    m_shaderId++;

    return pShader;   
}

GCShader* GCGraphics::CreateShaderTexture() 
{
    //Creates a texture shader / initializes it
    //HLSLFile* shaderFile = new HLSLFile(L"Shaders\\texture.hlsl");

    GCShader* pShader;
    pShader = new GCShaderTexture();
    pShader->Initialize(m_pRender, "../../../src/Render/Shaders/texture.hlsl", "../../../src/Render/CsoCompiled/texture", STEnum::texture);
    m_vShaders.push_back(pShader);
    m_shaderId++;

    return pShader;
}

GCMesh* GCGraphics::CreateMesh(GCGeometry* pGeometry) 
{
    GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();
    //Creates mesh using a specific geometry
    if (CheckNull(pGeometry))
    {
        OutputDebugString(L"Mesh Geometry is empty \n");
        profiler.LogWarning("Mesh Geometry is empty");
    }

    GCMesh* pMesh = new GCMesh();
    pMesh->Initialize(m_pRender);
    if (pGeometry->texC.size() == 0)
    {
        pMesh->UploadGeometryDataColor(pGeometry);
    }
    else
    {
        pMesh->UploadGeometryDataTexture(pGeometry);
    }

    m_vMeshes.push_back(pMesh);
    return pMesh;
}

//GCShader* GCGraphics::CreateShaderCustom(HLSLFile* customShaderFile) {
//    GCShader* shader;
//    shader = new GCShaderCustom();
//    shader->Initialize(m_pRender, customShaderFile, STEnum::texture); // #TODO Upgrade Shader Code to Allow Custom Shader Properly
//
//    m_vShaders.push_back(shader);
//    m_shaderId++;
//
//    return shader;
//}

GCMaterial* GCGraphics::CreateMaterial() 
{
    //Creates a material (WIP)
    GCMaterial* pMaterial = new GCMaterial();
    //pMaterial->Initialize();
    //m_vMaterials.push_back(pMaterial);
    //m_materialId++;
    return pMaterial;
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

    if (it == m_vShaders.end())
    {
        OutputDebugString(L"Shader not found");
    }

    m_vShaders.erase(it);

    delete pShader;
}

void GCGraphics::RemoveMaterial(GCMaterial* pMaterial) 
{
    //Removes material from the vector then the material itself
    auto it = std::find(m_vMaterials.begin(), m_vMaterials.end(), pMaterial);

    if (it == m_vMaterials.end())
    {
        OutputDebugString(L"Material not found");
    }

    m_vMaterials.erase(it);

    delete pMaterial;
}

void GCGraphics::RemoveMesh(GCMesh* pMesh) 
{
    //Removes Mesh
    auto it = std::find(m_vMeshes.begin(), m_vMeshes.end(), pMesh);

    if (it == m_vMeshes.end())
    {
        OutputDebugString(L"Mesh not found");
    }

    m_vMeshes.erase(it);

    delete pMesh;
}

void GCGraphics::RemoveTexture(GCTexture* pTexture)
{
    //Removes Texture
    auto it = std::find(m_vTextures.begin(), m_vTextures.end(), pTexture);

    if (it == m_vTextures.end())
    {
        OutputDebugString(L"Texture not found");
    }

    m_vTextures.erase(it);

    delete pTexture;
}