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

	/**
* Initializes graphics.
 * @brief
 *
 * This function takes into account the window you want to use as well as sizes you want your render to be(not the window,the actual renderer screen).
 *
 * @param Window that'll be used.
 * @param render-Window's desired width(preferably 1920 or lower).
 * @param render-Window's desired height(preferably 1080 or lower).
 */
	void Initialize(Window* pWindow, int renderWidth, int renderHeight);

	// Each Frame
	void StartFrame();
	void EndFrame();

	void InitializeGraphicsResourcesStart();
	void InitializeGraphicsResourcesEnd();

	// Shader
	/**
	 * Creates a shader color using color.hlsl(template).
	 * @brief
	 *
	 *
	 *
	 * @return Shader color
	 */
	ResourceCreationResult<GCShader*> CreateShaderColor();
	/**
	 * Creates a shader texture using texture.hlsl(template).
	 * @brief
	 *
	 *
	 *
	 * @return Shader texture
	 */
	ResourceCreationResult<GCShader*> CreateShaderTexture();
	ResourceCreationResult<GCShader*> CreateShaderCustom(std::string& filePath, std::string& compiledShaderDestinationPath, int& flagEnabledBits);
	/**
	 * Converts pixel pos to world pos.
	 * @brief
	 *
	 * This function converts the pixels given in args into ndc coordinates
	 *
	 * @param pixel where you want the center of your object to be(x axis)
	 * @param pixel where you want the center of your object to be(y axis)
	 * @param proj matrix
	 * @param view matrix
	 *
	 * @return world matrix of the object at said coordinates(pixelX/pixelY)
	 */
	DirectX::XMFLOAT4X4 ToPixel(int pixelX, int pixelY, DirectX::XMFLOAT4X4 proj, DirectX::XMFLOAT4X4 view);

	ResourceCreationResult<GCMaterial*> CreateMaterial(GCShader* pShader);

	ResourceCreationResult<GCMesh*> CreateMesh(GCGeometry* pGeometry);

	ResourceCreationResult<GCGeometry*> CreateGeometryPrimitiveTexture(const std::string& primitiveName);
	ResourceCreationResult<GCGeometry*> CreateGeometryPrimitiveColor(const std::string& primitiveName, const DirectX::XMFLOAT4& color);
	ResourceCreationResult<GCGeometry*> CreateGeometryPrimitiveCustom(const std::string& primitiveName, const DirectX::XMFLOAT4& color, int& flagEnabledBits);
	/**
	 * Creates Geometry using a file that's using UVs.
	 * @brief
	 *
	 *
	 * @param filePath of said model
	 * @param extension type(.obj/.fbx/etc..)
	 *
	 * @return Geometry using UVs created using a model
	 */
	ResourceCreationResult<GCGeometry*> CreateGeometryModelParserTexture(const std::string& filePath, Extensions fileExtensionType);
	/**
	 * Creates Geometry using a file that's using colors.
	 * @brief
	 *
	 *
	 * @param filePath of said model
	 * @param color used to draw the object with
	 * @param extension type(.obj/.fbx/etc..)
	 *
	 * @return Geometry using colors created using a model
	 */
	ResourceCreationResult<GCGeometry*> CreateGeometryModelParserColor(const std::string& filePath, DirectX::XMFLOAT4 color, Extensions fileExtensionType);
	ResourceCreationResult<GCGeometry*> CreateGeometryModelParserCustom(const std::string& filePath, DirectX::XMFLOAT4 color, Extensions fileExtensionType, int& flagEnabledBits);
	/**
	* Creates a texture.
	 * @brief
	 *
	 * This function creates a texture using a .dds file.
	 *
	 * @param Filepath of said texture(.dds)
	 * @return Texture
	 */
	ResourceCreationResult<GCTexture*> CreateTexture(const std::string& filePath);

	// Update Constant Buffer 
	void UpdateConstantBuffer(const GCSHADERCB& objectData, GCShaderUploadBufferBase* uploadBufferInstance);

	// Update ViewProj, use for Camera
	void UpdateViewProjConstantBuffer(DirectX::XMFLOAT4X4 projectionMatrix, DirectX::XMFLOAT4X4 viewMatrix);
	// Update world cb buffer with GCWORLDCB Struct
	void UpdateWorldConstantBuffer(GCMaterial* pMaterial, DirectX::XMFLOAT4X4 worldMatrix);
	// Update world with custom struct
	template<typename ShaderTypeConstantBuffer>
	void UpdateCustomCBObject(GCMaterial* pMaterial, const GCSHADERCB& objectData);

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

	// Manage inactive slot to push resource
	std::vector<bool> m_vTextureActiveFlags;
	std::vector<bool> m_vMeshActiveFlags;

private:
	template<typename ShaderTypeConstantBuffer>
	void CreateCBCamera();

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

