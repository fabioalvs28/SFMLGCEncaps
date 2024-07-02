#pragma once

template<typename ResourcePtr>
struct ResourceCreationResult {
	bool success;
	ResourcePtr resource;
};

class GCGraphics
{
public:
	GCGraphics();
	~GCGraphics();

	bool Initialize(Window* pWindow, int renderWidth, int renderHeight);

	// Each Frame
	bool StartFrame();
	bool EndFrame();

	bool InitializeGraphicsResourcesStart();
	bool InitializeGraphicsResourcesEnd();

	// Shader
	ResourceCreationResult<GCShader*> CreateShaderColor();
	ResourceCreationResult<GCShader*> CreateShaderTexture();
	ResourceCreationResult<GCShader*> CreateShaderCustom(std::string& filePath, std::string& compiledShaderDestinationPath, int& flagEnabledBits, D3D12_CULL_MODE cullMode);

	DirectX::XMFLOAT4X4 ToPixel(int pixelX, int pixelY, DirectX::XMFLOAT4X4 proj, DirectX::XMFLOAT4X4 view);

	ResourceCreationResult<GCMaterial*> CreateMaterial(GCShader* pShader);

	ResourceCreationResult<GCMesh*> CreateMeshCustom(GCGeometry* pGeometry, int& flagEnabledBits);
	ResourceCreationResult<GCMesh*> CreateMeshColor(GCGeometry* pGeometry);
	ResourceCreationResult<GCMesh*> CreateMeshTexture(GCGeometry* pGeometry);

	ResourceCreationResult<GCGeometry*> CreateGeometryPrimitive(const GC_PRIMITIVE_ID primitiveIndex, const DirectX::XMFLOAT4& color);
	ResourceCreationResult<GCGeometry*> CreateGeometryModelParser(const std::string& filePath, DirectX::XMFLOAT4 color, Extensions fileExtensionType);

	ResourceCreationResult<GCTexture*> CreateTexture(const std::string& filePath);

	// Update Constant Buffer 
	void UpdateConstantBuffer(const GCSHADERCB& objectData, GCShaderUploadBufferBase* uploadBufferInstance);

	// Update ViewProj, use for Camera
	bool UpdateViewProjConstantBuffer(DirectX::XMFLOAT4X4 projectionMatrix, DirectX::XMFLOAT4X4 viewMatrix);
	// Update world cb buffer with GCWORLDCB Struct
	bool UpdateWorldConstantBuffer(GCMaterial* pMaterial, DirectX::XMFLOAT4X4 worldMatrix);
	// Update world with custom struct
	template<typename ShaderTypeConstantBuffer>
	bool UpdateCustomCbPerObject(GCMaterial* pMaterial, const GCSHADERCB& objectData);

	bool UpdateMaterialProperties(GCMaterial* pMaterial, GCMATERIALPROPERTIES objectData);
	bool UpdateMaterialProperties(GCMaterial* pMaterial, DirectX::XMFLOAT4 ambientLightColor, DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 diffuse, DirectX::XMFLOAT4 specular, float shininess);

	bool UpdateLights(GCLIGHTSPROPERTIES objectData);

	// Remove Resources
	bool RemoveShader(GCShader* pShader);
	bool RemoveMaterial(GCMaterial* pMaterial);

	// Remove using active flags
	bool RemoveMesh(GCMesh* pMesh);
	bool RemoveTexture(GCTexture* pTexture);

	std::vector<GCShader*> GetShaders();
	std::vector<GCMaterial*> GetMaterials();
	std::vector<GCMesh*> GetMeshes();
	std::vector<GCTexture*> GetTextures();

	GCRender* GetRender() const { return m_pRender; }
	
	GCPrimitiveFactory* GetPrimitiveFactory() const { return m_pPrimitiveFactory; }
	GCModelParser* GetModelParserFactory() const { return m_pModelParserFactory; }

	GCShaderUploadBufferBase* GetCbLightPropertiesInstance() const { return m_pCbLightPropertiesInstance; }

	// Manage inactive slot to push resource
	std::vector<bool> m_vTextureActiveFlags;
	std::vector<bool> m_vMeshActiveFlags;

private:
	// Render instance contain Window
	int m_renderWidth;
	int	m_renderHeight;

	GCRender* m_pRender;

	std::vector<GCTexture*> m_vTextures;
	std::vector<GCShader*> m_vShaders;
	std::vector<GCMaterial*> m_vMaterials;
	std::vector<GCMesh*> m_vMeshes;

	std::vector<GCShaderUploadBufferBase*> m_pCbCameraInstances;
	GCShaderUploadBufferBase* m_pCbLightPropertiesInstance;

	GCPrimitiveFactory* m_pPrimitiveFactory;
	GCModelParser* m_pModelParserFactory;
};


// Update per object constant buffer by custom struct derived from gcshadercb
template<typename ShaderTypeConstantBuffer>
bool GCGraphics::UpdateCustomCbPerObject(GCMaterial* pMaterial, const GCSHADERCB& objectData)
{

	// Don't create UploadBuffer if the number of object draw in the same material don't increase
	if (pMaterial->GetCount() >= pMaterial->GetCbObjectInstance().size()) {
		pMaterial->AddCbPerObject<ShaderTypeConstantBuffer>();
	}
		
	// Update 
	pMaterial->UpdateConstantBuffer(objectData, pMaterial->GetCbObjectInstance()[pMaterial->GetCount()]);
}

