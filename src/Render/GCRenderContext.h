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

	void CreatePostProcessingResources();

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
	void PerformPostProcessing();
	/**
	* Pre-Draw.
	 * @brief
	 *
	 * This function always needs to be called right before drawing!
	 *
	 * @return True if the preparation was a success/false otherwise.
	 */
	bool PrepareDraw();

	bool DrawObject(GCMesh* pMesh, GCMaterial* pMaterial, bool alpha);

	/**
	* Post-Draw.
	 * @brief
	 *
	 * This function always needs to be called right after drawing!
	 *
	 * @return True if the execution of the drawing was a success/false otherwise.
	 */
	bool CompleteDraw();
	/**
	*  Draws an object(sends the data to the commandlist to be exact).
	 * @brief
	 *
	 * This function takes into account the mesh you want to draw as well as the material you want to use to draw it.
	 * Always call PrepareDraw before starting drawing/PostDraw after finishing drawing(you can call DrawObject multiple times in between the two PrepareDraw/PostDraw calls)
	 *
	 * @param Mesh.
	 * @param Material.
	 * @param Alpha(true for transparency,false for opaque).
	 * @return True if the drawing was a success/false otherwise.
	 */

	void OnResize(); // #TODO -> Remove from Window and Allow to Engine to use it when they want resize, and allow graphic creation specify dimensions for swapchain / viewport


	void ActiveBasicPostProcessing();
	void ActivePixelIDMapping();
	void ActiveDeferredLightPass();

	void DesactiveBasicPostProcessing();
	void DesactivePixelIDMapping();
	void DesactiveDeferredLightPass();

	// Camera & Light -> Temporarily
	GCShaderUploadBufferBase* m_pCbCurrentViewProjInstance;
	GCShaderUploadBufferBase* m_pCbLightPropertiesInstance;

	inline GCRenderResources* GetRenderResources() { return m_pGCRenderResources; }

	inline void Set2DMode() { m_renderMode = 0; }
	inline void Set3DMode() { m_renderMode = 1; }
private:

	bool m_isBasicPostProcessingActivated;
	bool m_isPixelIDMappingActivated; 

	bool m_isDeferredLightPassActivated = false;

	int m_renderMode = 1; //2D or 3d

	// Post Processing Resources
	GCShader* m_postProcessingShader;
	// Pixel Id Mapping Buffer Resources
	GCShader* m_objectBufferIdShader;

	GCRenderResources* m_pGCRenderResources;
};