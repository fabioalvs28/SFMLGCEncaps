#include "framework.h"

//#include "GCMesh.h"
//#include "GCGeometry.h"


GCGraphics::GCGraphics() {
    m_pRender = nullptr;
}

void GCGraphics::Initialize(Window* window) {
    m_pRender = new GCRender();
    m_pRender->Initialize(this,window);

    // 
    m_pPrimitiveFactory = new GCPrimitiveFactory();
    m_pModelParserFactory = new GCModelParserObj();
}

//Creates and initializes a texture using a path
GCTexture* GCGraphics::CreateTexture(const std::string& filePath) {
	GCTexture* texture = new GCTexture();
	texture->Initialize(filePath, this);
    m_vTextures.push_back(texture);
	return texture;
}

//Creates a color shader / initializes it
GCShader* GCGraphics::CreateShaderColor() {
    //HLSLFile* shaderFile = new HLSLFile(L"Shaders\\color.hlsl");

    GCShader* shader;
    shader = new GCShaderColor();



    shader->Initialize(m_pRender, "../../../src/Render/Shaders/color.hlsl", "../../../src/Render/CsoCompiled/color", STEnum::color);

    m_vShaders.push_back(shader);
    m_shaderId++;

    return shader;   
}

//Creates a texture shader / initializes it
GCShader* GCGraphics::CreateShaderTexture() {
    //HLSLFile* shaderFile = new HLSLFile(L"Shaders\\texture.hlsl");

    GCShader* shader;
    shader = new GCShaderTexture();
    shader->Initialize(m_pRender, "../../../src/Render/Shaders/texture.hlsl", "../../../src/Render/CsoCompiled/texture", STEnum::texture);
    m_vShaders.push_back(shader);
    m_shaderId++;

    return shader;
}

//Creates mesh using a specific geometry
GCMesh* GCGraphics::CreateMesh(GCGeometry* pGeometry) {
    GCMesh* mesh = new GCMesh();
    mesh->Initialize(m_pRender);
    if (pGeometry->texC.size() == 0)
        mesh->UploadGeometryDataColor(pGeometry);
    else
        mesh->UploadGeometryDataTexture(pGeometry);

    m_vMeshes.push_back(mesh);
    return mesh;
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

//Creates a material (WIP)
GCMaterial* GCGraphics::CreateMaterial(GCShader* pShader, GCTexture* pTexture) {
    GCMaterial* material = new GCMaterial();
    material->Initialize(pShader, pTexture);
    m_vMaterials.push_back(material);
    m_materialId++;
    return material;
}

std::vector<GCShader*> GCGraphics::GetShaders() {
    return m_vShaders;
}

std::vector<GCMaterial*> GCGraphics::GetMaterials() {
    return m_vMaterials;
}

std::vector<GCMesh*> GCGraphics::GetMeshes() {
    return m_vMeshes;
}

std::vector<GCTexture*> GCGraphics::GetTextures() {
    return m_vTextures;
}

//Removes Shader both from vector and the shader itself
void GCGraphics::RemoveShader(GCShader* pShader) {
    auto it = std::find(m_vShaders.begin(), m_vShaders.end(), pShader);

    if (it != m_vShaders.end()) {
        m_vShaders.erase(it);
    }

    delete pShader;
}

//Removes material from the vector then the material itself
void GCGraphics::RemoveMaterial(GCMaterial* pMaterial) {
    auto it = std::find(m_vMaterials.begin(), m_vMaterials.end(), pMaterial);

    if (it != m_vMaterials.end()) {
        m_vMaterials.erase(it);
    }

    delete pMaterial;
}

//Removes Mesh
void GCGraphics::RemoveMesh(GCMesh* pMesh) {
    auto it = std::find(m_vMeshes.begin(), m_vMeshes.end(), pMesh);

    if (it != m_vMeshes.end()) {
        m_vMeshes.erase(it);
    }
    delete pMesh;
}

//Removes Texture
void GCGraphics::RemoveTexture(GCTexture* pTexture) {
    auto it = std::find(m_vTextures.begin(), m_vTextures.end(), pTexture);

    if (it != m_vTextures.end()) {
        m_vTextures.erase(it);
    }

    delete pTexture;
}