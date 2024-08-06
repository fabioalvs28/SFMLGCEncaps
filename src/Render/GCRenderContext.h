#pragma once


class GCRenderContext
{
public:
	GCRenderContext();
	~GCRenderContext();

	bool Initialize(Window* pWindow, int renderWidth, int renderHeight, GCGraphics* pGraphics);
	bool InitDX12RenderPipeline();

	void LogAdapters();
	void LogAdapterOutputs(IDXGIAdapter* adapter);
	void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

	void EnableDebugController();

	void CreateCommandObjects();
	void CreateSwapChain();

	void CreatePostProcessingResources(std::string shaderfilePath, std::string csoDestinationPath);
	void CreateDeferredLightPassResources();

	// Resize 
	void ReleasePreviousResources();
	void ResizeSwapChain();
	void CreateRenderTargetViews();
	void CreateDepthStencilBufferAndView();
	void UpdateViewport();

	// Draw Part
	bool ResetCommandList();
	void ExecuteCommandList();
	bool CloseCommandList();

	bool FlushCommandQueue();

	void PerformPostProcessingCS();
	void PerformDeferredLightPass();

	bool PrepareDraw();

	bool DrawObject(GCMesh* pMesh, GCMaterial* pMaterial, bool alpha);

	bool CompleteDraw();

	void OnResize(); 


	void ActiveCSPostProcessing();
	void ActivePixelIDMapping();
	void ActiveDeferredLightPass();

	void DesactiveCSPostProcessing();
	void DesactivePixelIDMapping();
	void DesactiveDeferredLightPass();


	// Camera & Light Upload
	GCShaderUploadBufferBase* m_pCbCurrentViewProjInstance;
	GCUploadBufferBase* m_pCbLightPropertiesInstance;

	inline GCRenderResources* GetRenderResources() { return m_pGCRenderResources; }

	inline void Set2DMode() { m_renderMode = 0; }
	inline void Set3DMode() { m_renderMode = 1; }

	// DEFERRED SHADING LIGHT RESOURCES
	// All diferent material used in same frame
	std::vector<GC_MATERIAL_DSL> m_materialsUsedInFrame;
	// Upload Material DSL, Send to Deferred Shader
	GCUploadBuffer<GC_MATERIAL_DSL>* m_pCbMaterialDsl;

	std::string m_PPa;
	std::string m_PPb;
	//*
private:
	bool m_isPixelIDMappingActivated; 
	bool m_isDeferredLightPassActivated;

	bool m_isCSPostProcessingActivated;



	int m_renderMode = 0; //2D or 3d

	// Basic PP
	GCShader* m_pPostProcessingShader;
	// Compute Shader PP
	GCComputeShader* m_postProcessingShaderCS;
	GCShader* m_pPixelIdMappingShader;
	GCShader* m_pDeferredLightPassShader; // need PixelIdMapping pass

	////////////////////////*///////////////////////

	//Post Processing Resources
	GC_DESCRIPTOR_RESOURCE* m_pPostProcessingRtv;

	D3D12_GPU_DESCRIPTOR_HANDLE m_postProcessingFrontBufferSrvGpuHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE m_postProcessingBackBufferSrvGpuHandle;


	D3D12_GPU_DESCRIPTOR_HANDLE m_postProcessingUavGpuHandle;

	//Pixel Id Mapping Resources
	GC_DESCRIPTOR_RESOURCE* m_pPixelIdMappingBufferRtv; //Rtv Buffer
	//DSV for PIM -> #TODO Use the principal dsv, in reading 
	GC_DESCRIPTOR_RESOURCE* m_pPixelIdMappingDepthStencilBuffer; //Rtv Buffer

	// Deferred Shading Light Resources
	GCUploadBufferBase* m_materialsStructuredBuffer;

	GC_DESCRIPTOR_RESOURCE* m_pDeferredLightPassBufferRtv;
	GC_DESCRIPTOR_RESOURCE* m_pAlbedoGBuffer;
	GC_DESCRIPTOR_RESOURCE* m_pWorldPosGBuffer;
	GC_DESCRIPTOR_RESOURCE* m_pNormalGBuffer;

	////////////////////////*///////////////////////

	// Contain the bare minimum render pipeline resource
	GCRenderResources* m_pGCRenderResources;
};