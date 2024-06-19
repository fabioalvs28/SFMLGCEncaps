#pragma once

#define FLAG_COORDS 0x01
#define FLAG_UV 0x02

class GCGraphics
{
public:
	GCGraphics();

	void Initialize(Window* window, int renderWidth, int renderHeight);

	// Principal Object Creation

	GCShader* CreateShaderColor();
	GCShader* CreateShaderTexture();
	GCShader* CreateShaderCustom(std::string& filePath, std::string& compiledShaderDestinationPath, int type);

	template<typename ShaderTypeConstantBuffer>
	ShaderTypeConstantBuffer ToPixel(int pixelX, int pixelY, DirectX::XMFLOAT4X4 proj, DirectX::XMFLOAT4X4 view);


	GCMaterial* CreateMaterial(GCShader* pShader, GCTexture* pTexture);

	GCMesh* CreateMesh(GCGeometry* pGeometry);
	GCTexture* CreateTexture(const std::string& filePath);


	void UpdateViewProjConstantBuffer(DirectX::XMFLOAT4X4 projectionMatrix, DirectX::XMFLOAT4X4 viewMatrix);
	//Updates a cb data of a given material using the three matrices world/view/proj
	//using a count for now that'll need to be reset after each draw,might be subject to changes in the near future
	void UpdateWorldConstantBuffer(GCMaterial* pMaterial, DirectX::XMFLOAT4X4 worldMatrix);
	//Update Camera and Object Constant Buffer / But They can also send their own structure
	void UpdateConstantBuffer(const GCSHADERCB& objectData, GCShaderUploadBufferBase* uploadBufferInstance);

	template<typename ShaderTypeConstantBuffer>
	void CreateCBCamera();

	template<typename ShaderTypeConstantBuffer>
	void UpdateCustomCBObject(GCMaterial* pMaterial, const GCSHADERCB& objectData);

	//Delete
	void RemoveShader(GCShader* pShader);
	void RemoveMaterial(GCMaterial* pMaterial);
	void RemoveMesh(GCMesh* pMesh);
	void RemoveTexture(GCTexture* pTexture);

	//// Resources Manager
	std::vector<GCShader*> GetShaders();
	std::vector<GCMaterial*> GetMaterials();
	std::vector<GCMesh*> GetMeshes();
	std::vector<GCTexture*> GetTextures();

	// Update Resources Need
	//void UpdateMaterials();

	// Id
	// #TODO Se poser la question du suivi des ressources
	int GetTextureId() const { return m_textureId; }

	// Render
	GCRender* GetRender() const { return m_pRender; }

	GCPrimitiveFactory* GetPrimitiveFactory() const { return m_pPrimitiveFactory; }
	GCModelParser* GetModelParserFactory() const { return m_pModelParserFactory; }

private:
	// Render instance contain Window
	int m_renderWidth;
	int	m_renderHeight;

	GCRender* m_pRender;

	int m_meshId = 0;
	int m_shaderId = 0;
	int m_materialId = 0;
	int m_textureId = 0;

	std::vector<GCTexture*> m_vTextures;
	std::vector<GCShader*> m_vShaders;
	std::vector<GCMaterial*> m_vMaterials;
	std::vector<GCMesh*> m_vMeshes;

	// They Can use their own
	std::vector<GCShaderUploadBufferBase*> m_vpCameraCB;

	// Mesh
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