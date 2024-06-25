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

	void Initialize(Window* pWindow, int renderWidth, int renderHeight);

	// Each Frame
	void StartFrame();
	void EndFrame();

	void InitializeGraphicsResourcesStart();
	void InitializeGraphicsResourcesEnd();

	// Shader
	ResourceCreationResult<GCShader*> CreateShaderColor();
	ResourceCreationResult<GCShader*> CreateShaderTexture();
	ResourceCreationResult<GCShader*> CreateShaderCustom(std::string& filePath, std::string& compiledShaderDestinationPath, int& flagEnabledBits);



	DirectX::XMFLOAT4X4 ToPixel(int pixelX, int pixelY, DirectX::XMFLOAT4X4 proj, DirectX::XMFLOAT4X4 view);

	ResourceCreationResult<GCMaterial*> CreateMaterial(GCShader* pShader);

	ResourceCreationResult<GCMesh*> CreateMesh(GCGeometry* pGeometry);

	ResourceCreationResult<GCGeometry*> CreateGeometryPrimitiveTexture(const std::string& primitiveName);
	ResourceCreationResult<GCGeometry*> CreateGeometryPrimitiveColor(const std::string& primitiveName, const DirectX::XMFLOAT4& color);
	ResourceCreationResult<GCGeometry*> CreateGeometryPrimitiveCustom(const std::string& primitiveName, const DirectX::XMFLOAT4& color, int& flagEnabledBits);

	ResourceCreationResult<GCGeometry*> CreateGeometryModelParserTexture(const std::string& filePath, Extensions fileExtensionType);
	ResourceCreationResult<GCGeometry*> CreateGeometryModelParserColor(const std::string& filePath, DirectX::XMFLOAT4 color, Extensions fileExtensionType);
	ResourceCreationResult<GCGeometry*> CreateGeometryModelParserCustom(const std::string& filePath, DirectX::XMFLOAT4 color, Extensions fileExtensionType, int& flagEnabledBits);

	ResourceCreationResult<GCTexture*> CreateTexture(const std::string& filePath);

	// Update Constant Buffer 
	//Updates a cb data of a given material, using a count for now that'll need to be reset after each draw, might be subject to changes in the near future
	//Update Camera and Object Constant Buffer / But They can also send their own structure
	void UpdateConstantBuffer(const GCSHADERCB& objectData, GCShaderUploadBufferBase* uploadBufferInstance);
	// Base
	void UpdateViewProjConstantBuffer(DirectX::XMFLOAT4X4 projectionMatrix, DirectX::XMFLOAT4X4 viewMatrix);
	void UpdateWorldConstantBuffer(GCMaterial* pMaterial, DirectX::XMFLOAT4X4 worldMatrix);
	// Custom
	template<typename ShaderTypeConstantBuffer>
	void UpdateCustomCBObject(GCMaterial* pMaterial, const GCSHADERCB& objectData);
	// Near Update Camera

	template<typename ShaderTypeConstantBuffer>
	void CreateCBCamera();

	// Remove Resources
	bool RemoveShader(GCShader* pShader);
	bool RemoveMaterial(GCMaterial* pMaterial);
	bool RemoveMesh(GCMesh* pMesh);
	bool RemoveTexture(GCTexture* pTexture);

	std::vector<GCShader*> GetShaders();
	std::vector<GCMaterial*> GetMaterials();
	std::vector<GCMesh*> GetMeshes();
	std::vector<GCTexture*> GetTextures();

	// Id / #TODO Se poser la question du suivi des ressources
	//int GetTextureId() const { return m_textureId; }

	GCRender* GetRender() const { return m_pRender; }
	
	GCPrimitiveFactory* GetPrimitiveFactory() const { return m_pPrimitiveFactory; }
	GCModelParser* GetModelParserFactory() const { return m_pModelParserFactory; }

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

	std::vector<GCShaderUploadBufferBase*> m_vpCameraCB;

	GCPrimitiveFactory* m_pPrimitiveFactory;
	GCModelParser* m_pModelParserFactory;
};


template<typename ShaderTypeConstantBuffer>
void GCGraphics::CreateCBCamera()
{
	// Create a new shader upload buffer for the camera constant buffer
	GCShaderUploadBufferBase* pCameraCB = new GCShaderUploadBuffer<ShaderTypeConstantBuffer>(m_pRender->Getmd3dDevice(), 1, true);
	// Store the buffer in a vector
	m_vpCameraCB.push_back(pCameraCB);
}

template<typename ShaderTypeConstantBuffer>
void GCGraphics::UpdateCustomCBObject(GCMaterial* pMaterial, const GCSHADERCB& objectData)
{
	// Don't create UploadBuffer if the number of object draw in the same material don't increase
	if (pMaterial->GetCount() >= pMaterial->GetObjectCBData().size()) {
		pMaterial->CreateCBObject<ShaderTypeConstantBuffer>();
	}
		
	// Update 
	pMaterial->UpdateConstantBuffer(objectData, pMaterial->GetObjectCBData()[pMaterial->GetCount()]);
}

