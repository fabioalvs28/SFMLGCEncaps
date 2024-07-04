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
		/**
	* Call it at the beginning of a frame.
	 * @brief
	 *
	 * This function contains the prepareDraw func.
	 *
	 */
	void StartFrame();
	/**
	* Call it at the end of a frame.
	 * @brief
	 *
	 * This function contains the postDraw func.
	 *
	 */
	void EndFrame();
	/**
	* Call it before initializing ressources.
	 * @brief
	 *
	 * This function resets the command list so that you can create ressources,don't forget to call InitializeGraphicsResourcesEnd after your last initialization.
	 *
	 */
	void InitializeGraphicsResourcesStart();
	/**
	* Call it after initializing ressources.
	 * @brief
	 *
	 * This function executes what's inside the command list ,don't forget to call InitializeGraphicsResourcesStart before.
	 *
	 */
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
	/**
	 * Creates a material using a shader.
	 * @brief
	 *
	 *
	 * @param shader that'll be used by the material
	 *
	 * @return Material using said shader
	 */
	ResourceCreationResult<GCMaterial*> CreateMaterial(GCShader* pShader);
	/**
	 * Creates a mesh using a geometry.
	 * @brief
	 *
	 *
	 * @param geometry that'll be used by the mesh
	 *
	 * @return Mesh using said geometry
	 */
	ResourceCreationResult<GCMesh*> CreateMesh(GCGeometry* pGeometry);
	/**
	 * Creates a Geometry using a template that's using UVs.
	 * @brief
	 *
	 *
	 * @param name of the primitive(that will later be changed to an int using an enum I suppose?)
	 *
	 * @return Geometry using UVs created using a template
	 */
	ResourceCreationResult<GCGeometry*> CreateGeometryPrimitiveTexture(const std::string& primitiveName);
	/**
	 * Creates a Geometry using a template that's using colors.
	 * @brief
	 *
	 *
	 * @param name of the primitive(that will later be changed to an int using an enum I suppose?)
	 * @param color in which the object will be drawn
	 *
	 * @return Geometry using colors created using a template
	 */
	ResourceCreationResult<GCGeometry*> CreateGeometryPrimitiveColor(const std::string& primitiveName, const DirectX::XMFLOAT4& color);
	ResourceCreationResult<GCGeometry*> CreateGeometryPrimitiveCustom(const std::string& primitiveName, const DirectX::XMFLOAT4& color, int& flagEnabledBits);
	/**
	 * Creates a Geometry using a file that's using UVs.
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
	 * Creates a Geometry using a file that's using colors.
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

	void Resize(int width, int height);
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
	// Remove using active flags
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

