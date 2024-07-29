#pragma once

enum GC_GRAPHICS_ERROR {
	GCRENDER_SUCCESS_OK = 0,
	GCRENDER_ERROR_GENERIC = -1,
	GCRENDER_ERROR_POINTER_NULL = -100,
	GCRENDER_ERROR_TEXTURE_CREATION_FAILED = -101,
	GCRENDER_ERROR_SHADER_CREATION_FAILED = -102,
	GCRENDER_ERROR_MESH_CREATION_FAILED = -103,
	GCRENDER_ERROR_MATERIAL_CREATION_FAILED = -104,
	GCRENDER_ERROR_GEOMETRY_CREATION_FAILED = -105,
	GCRENDER_ERROR_FILEPATH_NOT_FOUND = -106,
	GCRENDER_ERROR_BAD_EXTENSION = -107,
	GCRENDER_ERROR_RESOURCE_TO_REMOVE_DONT_FIND = -108,
	GCRENDER_ERROR_UNKNOWN = -999,

};

template<typename ResourcePtr>
struct ResourceCreationResult {
	bool success;
	ResourcePtr resource;
	GC_GRAPHICS_ERROR errorState = GCRENDER_ERROR_UNKNOWN;
};

enum GC_PROJECTIONTYPE {
	ORTHOGRAPHIC,
	PERSPECTIVE
};

class GCGraphics
{
public:
	GCGraphics();
	~GCGraphics();

	/************************************************************************************************
	* @brief Initializes graphics, This function takes into account the window you want to use as well as sizes you want your render to be(not the window,the actual renderer screen).
	*
	* @param[in] window               Window that'll be used.
	* @param[in] renderWidth          Window's desired width(preferably 1920 or lower).
	* @param[in] renderHeight         Window's desired height(preferably 1080 or lower).      
	* 
	* @return bool (success).
	************************************************************************************************/
	bool Initialize(Window* pWindow, int renderWidth, int renderHeight);

	/************************************************************************************************
	* @brief Call it at the beginning of each frame
	*
	* @return bool (success).
	* 
	* @note This function contains the prepareDraw func.
	************************************************************************************************/
	bool StartFrame();

	/************************************************************************************************
	* @brief Call it at the end of each frame.
	*
	* @return bool (success).
	*
	* @note This function contains the postDraw func.
	************************************************************************************************/
	bool EndFrame();

	/************************************************************************************************
	* @brief Call it before initializing ressources, This function resets the command list so that you can create ressources, don't forget to call InitializeGraphicsResourcesEnd after your 
	last initialization.
	*
	* @return bool (success).
	************************************************************************************************/
	bool InitializeGraphicsResourcesStart();

	/************************************************************************************************
	* @brief Call it after initializing ressources, This function executes what's inside the command list, don't forget to call InitializeGraphicsResourcesStart before.
	*
	* @return bool (success).
	************************************************************************************************/
	bool InitializeGraphicsResourcesEnd();

	/************************************************************************************************
	* @brief Creates a shader color using color.hlsl (template).
	*
	* @return ResourceCreationResult -> bool(success), GCShader(color), errorState
	************************************************************************************************/
	ResourceCreationResult<GCShader*> CreateShaderColor();

	/************************************************************************************************
	* @brief Creates a shader texture using texture.hlsl (template).
	*
	* @return ResourceCreationResult -> bool(success), GCShader(texture), errorState
	************************************************************************************************/
	ResourceCreationResult<GCShader*> CreateShaderTexture();


	/************************************************************************************************
	* @brief Creates a shader custom using your custom shader hlsl with your custom shader entry, custom parameter.
	*
	* @param[in] std::string& filePath -> ex : ./shader.hlsl
	* @param[in] std::string& the destination path where you want store custom shader after it's compilation -> ex : ./Folder/custom -> custom is the file name you want, the rest is the path
	* @param[in] int& flag, you can set it with set flag, -> ex : uv, color, normals
	* @param[in] D3D12_CULL_MODE cullMode -> Back by default
	* @param[in] int flagRootParameters -> set flag for shader parameter, options, all are possible by default
	* 
	* @return ResourceCreationResult -> bool(success), GCShader(custom), errorState
	************************************************************************************************/
	ResourceCreationResult<GCShader*> CreateShaderCustom(std::string& filePath, std::string& compiledShaderDestinationPath, int& flagEnabledBits, D3D12_CULL_MODE cullMode = D3D12_CULL_MODE_BACK, int flagRootParameters = DEFAULT_ROOT_PARAMETER_FLAG);

	/************************************************************************************************
	* @brief Creates a material using a shader.
	* 
	* @param[in] GCShader*, shader that'll be used by the material
	*
	* @return ResourceCreationResult -> bool(success), GCMaterial, errorState
	************************************************************************************************/
	ResourceCreationResult<GCMaterial*> CreateMaterial(GCShader* pShader);

	/************************************************************************************************
	* @brief Creates a texture from filePath.
	*
	* @param[in] const std::string&, filePath -> ex : ./Textures/cow.dds
	*
	* @return ResourceCreationResult -> bool(success), GCTexture, errorState
	************************************************************************************************/
	ResourceCreationResult<GCTexture*> CreateTexture(const std::string& filePath);

	/************************************************************************************************
	* @brief Creates a geometry from Primitive Factory, it generate all entry, vertex, color, uv, normal.
	*
	* @param[in] const GC_PRIMITIVE_ID primitiveIndex, define the primitive need be loaded
	* @param[in] const DirectX::XMFLOAT4& color, define geometry color, if it will be used by mesh after
	*
	* @return ResourceCreationResult -> bool(success), GCGeometry, errorState
	* 
	* @note the primitive list can be find in GC_PRIMITIVE_ID, there are 3D and 2D primitive
	************************************************************************************************/
	ResourceCreationResult<GCGeometry*> CreateGeometryPrimitive(const GC_PRIMITIVE_ID primitiveIndex, const DirectX::XMFLOAT4& color);

	/************************************************************************************************
	* @brief Creates a geometry from Model Parser Factory, it generate all entry, vertex, color, uv, normal.
	*
	* @param[in] const std::string& filePath 
	* @param[in] const DirectX::XMFLOAT4& color, define geometry color, if it will be used by mesh after
	* @param[in] GC_EXTENSIONS fileExtensionType -> obj/fbx -> only obj available, only triangulated 3d model
	*
	* @return ResourceCreationResult -> bool(success), GCGeometry, errorState
	************************************************************************************************/
	ResourceCreationResult<GCGeometry*> CreateGeometryModelParser(const std::string& filePath, DirectX::XMFLOAT4 color, GC_EXTENSIONS fileExtensionType);

	/************************************************************************************************
	* @brief Creates mesh custom from a geometry, we can create many mesh from the same geometry because mesh describe the geometry entry we want use, 
	-> ex : vertex/color/normal mesh for light colored mesh, vertex/uv mesh for no light textured mesh
	*
	* @param[in] GCGeometry pGeometry
	* @param[in] int& flagEnabledBits, set your flag to chose which entry you want to describe the mesh
	*
	* @return ResourceCreationResult -> bool(success), GCMesh, errorState
	************************************************************************************************/
	ResourceCreationResult<GCMesh*> CreateMeshCustom(GCGeometry* pGeometry, int& flagEnabledBits);

	/************************************************************************************************
	* @brief Creates mesh color from a geometry, color template
	*
	* @param[in] GCGeometry pGeometry
	*
	* @return ResourceCreationResult -> bool(success), GCMesh, errorState
	************************************************************************************************/
	ResourceCreationResult<GCMesh*> CreateMeshColor(GCGeometry* pGeometry);

	/************************************************************************************************
	* @brief Creates mesh texture from a geometry, texture template
	*
	* @param[in] GCGeometry pGeometry
	*
	* @return ResourceCreationResult -> bool(success), GCMesh, errorState
	************************************************************************************************/
	ResourceCreationResult<GCMesh*> CreateMeshTexture(GCGeometry* pGeometry);

	/************************************************************************************************
	* @brief Update View & Projection Matrix using GCVIEWPROJCB derived from GCSHADERCB, -> using for Camera, you can update at any moment, each frame if you want make move camera, 
	no update if you want static camera
	*
	* @param[in] DirectX::XMFLOAT4X4 projectionMatrix
	* @param[in] DirectX::XMFLOAT4X4 viewMatrix
	*
	* @return bool(success)
	************************************************************************************************/
	bool UpdateViewProjConstantBuffer(GCMATRIX& projectionMatrix, GCMATRIX& viewMatrix); //#TODO -> Have possibility to specify camera we want draw with
	bool CreateViewProjConstantBuffer(const GCVEC3& cameraPosition, const GCVEC3& cameraTarget, const GCVEC3& cameraUp, float fieldOfView, 
		float aspectRatio, 
		float nearZ, 
		float farZ, 
		float viewWidth,  
		float viewHeight,  
		GC_PROJECTIONTYPE projType,
		GCMATRIX& projectionMatrix,
		GCMATRIX& viewMatrix);

	/************************************************************************************************
	* @brief Update one object data, using GCWORLDCB Struct, template for update object matrix, and object/layer id in for PixelIdMapping
	*
	* @param[in] const GCSHADERCB& objectData, generic struct derived from GCSHADERCB
	*
	* @note Both object can use the same update
	*
	* @warning You must update with this function before each draw call, unless if you want use the same data for 2 draw call
	*
	* @return bool(success)
	************************************************************************************************/
	bool UpdateWorldConstantBuffer(GCMaterial* pMaterial, GCMATRIX& worldMatrix, float meshId = 0.0f);

	/************************************************************************************************
	* @template specify struct derived from GCSHADERCB
	* @brief Update one object data, you can use your custom struct derived from GCSHADERCB
	*
	* @param[in] GCMaterial pMaterial, the object material you want draw with
	* @param[in] const GCSHADERCB& objectData, generic struct derived from GCSHADERCB
	*
	* @note Both object can use the same update
	*
	* @warning You must update with this function before each draw call, unless if you want use the same data for 2 draw call
	*
	* @return bool(success)
	************************************************************************************************/
	template<typename ShaderTypeConstantBuffer>
	bool UpdateCustomCbPerObject(GCMaterial* pMaterial, const GCSHADERCB& objectData);

	/************************************************************************************************
	* @brief Update material properties from GCMATERIALPROPERTIES Struct derived from GCSHADERCB
	*
	* @param[in] GCMaterial pMaterial, the object material you want draw with
	* @param[in] DirectX::XMFLOAT4 ambientLightColor, color
	* @param[in] DirectX::XMFLOAT4 ambient, weight of effect
	* @param[in] DirectX::XMFLOAT4 diffuse
	* @param[in] DirectX::XMFLOAT4 specular
	* @param[in] float shininess
	*
	* @note this is made to be update at each frame or not update, but you can update as many times as you want if necessary for several objects using the same material
	*
	* @return bool(success)
	************************************************************************************************/
	bool UpdateMaterialProperties(GCMaterial* pMaterial, DirectX::XMFLOAT4& ambientLightColor, DirectX::XMFLOAT4& ambient, DirectX::XMFLOAT4& diffuse, DirectX::XMFLOAT4& specular, float shininess);

	/************************************************************************************************
	* @brief Same function as above but with GCMATERIALPROPERTIES data object already completed, without args
	*
	* @param[in] GCMaterial pMaterial, the object material you want draw with
	* @param[in] GCMATERIALPROPERTIES objectData, data already completed
	*
	* @note this is made to be update at each frame or not update, but you can update as many times as you want if necessary for several objects using the same material
	*
	* @return bool(success)
	************************************************************************************************/
	bool UpdateMaterialProperties(GCMaterial* pMaterial, GCMATERIALPROPERTIES& objectData);
	
	/************************************************************************************************
	* @brief Update lights (basic light system), 10 maximum
	*
	* @param[in] GCLIGHTSPROPERTIES& objectData, data already completed
	*
	* @note this is made to be update at each frame or not update, but you can update as many times as you want if necessary for all the scene
	*
	* @return bool(success)
	************************************************************************************************/
	bool UpdateLights(std::vector<GCLIGHT>& objectData);

	/************************************************************************************************
	* @brief Converts pixel pos to world pos, This function converts the pixels given in args into ndc coordinates
	*
	* @param[in] pixel where you want the center of your object to be(x axis)
	* @param[in] pixel where you want the center of your object to be(y axis)
	* @param[in] proj matrix
	* @param[in] view matrix
	*
	* @return world matrix of the object at said coordinates(pixelX/pixelY)
	************************************************************************************************/
	DirectX::XMFLOAT4X4 ToPixel(int pixelX, int pixelY, DirectX::XMFLOAT4X4& proj, DirectX::XMFLOAT4X4& view);

	/************************************************************************************************
	* @brief Remove the shader resource in ShaderManager, for Release resource, to avoid performance problems, if shader is not used
	*
	* @param[in] GCShader pShader
	*
	* @return GC_GRAPHICS_ERROR errorState
	************************************************************************************************/
	GC_GRAPHICS_ERROR RemoveShader(GCShader* pShader);

	/************************************************************************************************
	* @brief Remove the material resource in MaterialManager, for Release resource, to avoid performance problems, if material is not used
	*
	* @param[in] GCMaterial pMaterial
	*
	* @return GC_GRAPHICS_ERROR errorState
	* 
	* @warning If you remove your material, you need remove also the texture and shader than material contain
	************************************************************************************************/
	GC_GRAPHICS_ERROR RemoveMaterial(GCMaterial* pMaterial);

	/************************************************************************************************
	* @brief Remove the mesh resource in MeshManager, for Release resource, to avoid performance problems, if mesh is not used
	*
	* @param[in] GCShader pMesh
	*
	* @return GC_GRAPHICS_ERROR errorState
	************************************************************************************************/
	GC_GRAPHICS_ERROR RemoveMesh(GCMesh* pMesh);

	/************************************************************************************************
	* @brief Remove the texture resource in TextureManager, for Release resource, to avoid performance problems, if texture is not used
	*
	* @param[in] GCTexture pTexture
	*
	* @return GC_GRAPHICS_ERROR errorState
	************************************************************************************************/
	GC_GRAPHICS_ERROR RemoveTexture(GCTexture* pTexture);

	/************************************************************************************************
	* @brief Get all resources
	*
	* @get Shaders
	* @get Materials
	* @get Meshes
	* @get Textures
	*
	* @return std::vector | std::list with the good resource
	************************************************************************************************/
	std::vector<GCShader*> GetShaders();
	std::vector<GCMaterial*> GetMaterials();
	std::vector<GCMesh*> GetMeshes();
	std::list<GCTexture*> GetTextures();

	/************************************************************************************************
	* @brief Get Render for no encapsulate Render functions
	*
	* @return GCRenderContext
	************************************************************************************************/
	GCRenderContext* GetRender() const { return m_pRender; }

	/************************************************************************************************
	* @brief Interns Function
	************************************************************************************************/
	void UpdateConstantBuffer(const GCSHADERCB& objectData, GCShaderUploadBufferBase* uploadBufferInstance);
	void Resize(int width, int height);

	// Loaders
	GCPrimitiveFactory* GetPrimitiveFactory() const { return m_pPrimitiveFactory; }
	GCModelParserFactory* GetModelParserFactory() const { return m_pModelParserFactory; }

	GCFontGeometryLoader* GetFontGeometryLoader() const { return m_pFontGeometryLoader; }
	GCSpriteSheetGeometryLoader* GetSpriteSheetGeometryLoader() const { return m_pSpriteSheetGeometryLoader; }

	//Other
	GCShaderUploadBufferBase* GetCbLightPropertiesInstance() const { return m_pCbLightPropertiesInstance; }


	// Manage inactive slot to push resource
	std::list<bool> m_lTextureActiveFlags;

	GCFontGeometryLoader* m_pFontGeometryLoader;
	GCSpriteSheetGeometryLoader* m_pSpriteSheetGeometryLoader;

private:
	GCRenderContext* m_pRender;

	//Resources
	std::list<GCTexture*> m_lTextures;
	std::vector<GCShader*> m_vShaders;
	std::vector<GCMaterial*> m_vMaterials;
	std::vector<GCMesh*> m_vMeshes;

	// Scene properties
	std::vector<GCShaderUploadBufferBase*> m_pCbCameraInstances;
	GCShaderUploadBufferBase* m_pCbLightPropertiesInstance;

	//Vertex Parser
	GCPrimitiveFactory* m_pPrimitiveFactory;
	GCModelParserFactory* m_pModelParserFactory;

};

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

