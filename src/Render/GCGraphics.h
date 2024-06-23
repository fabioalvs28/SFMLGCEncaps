#pragma once

//template<typename ResourcePtr>
//struct ResourceCreationResult {
//	bool success;
//	ResourcePtr resource;
//};

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
	GCShader* CreateShaderColor();
	GCShader* CreateShaderTexture();
	GCShader* CreateShaderCustom(std::string& filePath, std::string& compiledShaderDestinationPath, int& flagEnabledBits);

	template<typename ShaderTypeConstantBuffer>
	ShaderTypeConstantBuffer ToPixel(int pixelX, int pixelY, DirectX::XMFLOAT4X4 proj, DirectX::XMFLOAT4X4 view);


	GCMaterial* CreateMaterial(GCShader* pShader);
	GCMesh* CreateMesh(GCGeometry* pGeometry);

	GCGeometry* CreateGeometryPrimitiveTexture(const std::string& primitiveName);
	GCGeometry* CreateGeometryPrimitiveColor(const std::string& primitiveName, const DirectX::XMFLOAT4& color);
	GCGeometry* CreateGeometryPrimitiveCustom(const std::string& primitiveName, const DirectX::XMFLOAT4& color, int& flagEnabledBits);

	GCGeometry* CreateGeometryModelParserTexture(const std::string& filePath, Extensions fileExtensionType);
	GCGeometry* CreateGeometryModelParserColor(const std::string& filePath, DirectX::XMFLOAT4 color, Extensions fileExtensionType);
	GCGeometry* CreateGeometryModelParserCustom(const std::string& filePath, DirectX::XMFLOAT4 color, Extensions fileExtensionType, int& flagEnabledBits);

	GCTexture* CreateTexture(const std::string& filePath);

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
	void RemoveShader(GCShader* pShader);
	void RemoveMaterial(GCMaterial* pMaterial);
	void RemoveMesh(GCMesh* pMesh);
	void RemoveTexture(GCTexture* pTexture);

	std::vector<GCShader*> GetShaders();
	std::vector<GCMaterial*> GetMaterials();
	std::vector<GCMesh*> GetMeshes();
	std::vector<GCTexture*> GetTextures();

	// Id / #TODO Se poser la question du suivi des ressources
	int GetTextureId() const { return m_textureId; }


	GCRender* GetRender() const { return m_pRender; }
	
	GCPrimitiveFactory* GetPrimitiveFactory() const { return m_pPrimitiveFactory; }
	GCModelParser* GetModelParserFactory() const { return m_pModelParserFactory; }

private:
	// Render instance contain Window
	int m_renderWidth;
	int	m_renderHeight;

	GCRender* m_pRender;

	int m_meshId;
	int m_shaderId;
	int m_materialId;
	int m_textureId;

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

template<typename ShaderTypeConstantBuffer>
ShaderTypeConstantBuffer GCGraphics::ToPixel(int pixelX, int pixelY, DirectX::XMFLOAT4X4 proj, DirectX::XMFLOAT4X4 view) {
	DirectX::XMFLOAT3 worldPos = GCUtils::PixelToWorld(pixelX, pixelY, m_renderWidth, m_renderHeight, proj, view);
	DirectX::XMMATRIX translationMatrix = DirectX::XMMatrixTranslation(worldPos.x, worldPos.y, worldPos.z);
	DirectX::XMFLOAT4X4 convertedMatrix;
	DirectX::XMStoreFloat4x4(&convertedMatrix, DirectX::XMMatrixTranspose(translationMatrix));


	ShaderTypeConstantBuffer worldData;
	worldData.world = convertedMatrix;

	return worldData;
}