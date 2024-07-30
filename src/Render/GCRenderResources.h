#pragma once

struct GC_DESCRIPTOR_RESOURCE
{
	ID3D12Resource* resource;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
};

class GCRenderResources {
public:

	GCRenderResources();
	~GCRenderResources();

	//Double RTV Resource For Final Render
	inline ID3D12Resource* CurrentBackBuffer() const { return m_SwapChainBuffer[m_CurrBackBuffer]; }
	//CPU Handle Address of the both
	inline D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferViewAddress() const { return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_pRtvHeap->GetCPUDescriptorHandleForHeapStart(), m_CurrBackBuffer, m_rtvDescriptorSize); }
	//Render format
	inline DXGI_FORMAT GetBackBufferFormat() const { return m_BackBufferFormat; }
	inline int GetCurrBackBuffer() const { return m_CurrBackBuffer; }
	//Msaa
	inline bool Get4xMsaaState() const { return m_4xMsaaState; }
	inline UINT Get4xMsaaQuality() const { return m_4xMsaaQuality; }

	//CPU Handle address Depth Stencil Final Render
	inline D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilViewAddress() const { return m_DepthStencilBufferAddress; }
	//It's format
	inline DXGI_FORMAT GetDepthStencilFormat() const { return m_DepthStencilFormat; }

	//DirectX12 Instances
	inline ID3D12GraphicsCommandList* GetCommandList() const { return m_CommandList; }
	inline ID3D12Device* Getmd3dDevice() const { return m_d3dDevice; }
	inline ID3D12CommandQueue* GetCommandQueue() const { return m_CommandQueue; }
	inline ID3D12CommandAllocator* GetCommandAllocator() const { return m_DirectCmdListAlloc; }
	inline ID3D12Fence* GetFence() { return m_Fence; }
	inline ID3D12Debug* GetDebugController() { return m_pDebugController; }


	//Descriptor Heaps
	inline ID3D12DescriptorHeap* GetRtvHeap() { return m_pRtvHeap; }
	inline ID3D12DescriptorHeap* GetDsvHeap() { return m_pDsvHeap; }
	inline ID3D12DescriptorHeap* GetCbvSrvUavSrvDescriptorHeap() { return m_pCbvSrvUavDescriptorHeap; }

	//Descriptor Size
	inline UINT GetRtvDescriptorSize() const { return m_rtvDescriptorSize; }
	inline UINT GetDsvDescriptorSize() const { return m_dsvDescriptorSize; }
	inline UINT GetCbvSrvUavDescriptorSize() const { return m_cbvSrvUavDescriptorSize; }

	inline Window* GetCurrentWindow() { return m_pWindow; }

	inline UINT GetRenderWidth() const { return m_renderWidth; }
	inline UINT GetRenderHeight() const { return m_renderHeight; }
	// Setter 
	inline void ResizeRender(int width, int height) { m_renderWidth = width;  m_renderHeight = height; }
private:
	friend class GCRenderContext;

	Window* m_pWindow;

	//Swap chain size
	int m_renderWidth;
	int	m_renderHeight;

	//Debug
	GCGraphics* m_pGraphics;

	static const int SwapChainBufferCount = 2;

	//DirectX12 3D Instances
	IDXGIFactory4* m_dxgiFactory;
	ID3D12Device* m_d3dDevice;

	ID3D12GraphicsCommandList* m_CommandList;
	IDXGISwapChain* m_SwapChain;
	ID3D12Resource* m_SwapChainBuffer[SwapChainBufferCount];

	ID3D12Resource* m_DepthStencilBuffer;
	CD3DX12_CPU_DESCRIPTOR_HANDLE m_DepthStencilBufferAddress;

	ID3D12CommandQueue* m_CommandQueue;
	ID3D12CommandAllocator* m_DirectCmdListAlloc;

	//Fence
	ID3D12Fence* m_Fence;
	UINT64 m_CurrentFence;

	//Debug Controller
	ID3D12Debug* m_pDebugController;

	//Descriptor heaps
	ID3D12DescriptorHeap* m_pRtvHeap;
	ID3D12DescriptorHeap* m_pDsvHeap;
	ID3D12DescriptorHeap* m_pCbvSrvUavDescriptorHeap;

	//Descriptors size
	UINT m_rtvDescriptorSize;
	UINT m_dsvDescriptorSize;
	UINT m_cbvSrvUavDescriptorSize;

	//State var
	bool m_canResize;
	int m_CurrBackBuffer;

	//Format
	DXGI_FORMAT m_DepthStencilFormat;
	DXGI_FORMAT m_BackBufferFormat;
	DXGI_FORMAT m_rgbaFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

	//Msaa
	bool m_4xMsaaState;    // 4X MSAA enabled
	UINT m_4xMsaaQuality;      // quality level of 4X MSAA

	//Screen
	D3D12_VIEWPORT m_ScreenViewport;
	D3D12_RECT m_ScissorRect;

	CD3DX12_STATIC_SAMPLER_DESC staticSample;

	//Descriptor Heap Creation
	void CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible, ID3D12DescriptorHeap** ppDescriptorHeap);

	//Rtv Manager
	int m_rtvOffsetCount = 2;
	std::list<GC_DESCRIPTOR_RESOURCE*> m_lRenderTargets;
	GC_DESCRIPTOR_RESOURCE* CreateRTVTexture(DXGI_FORMAT format, D3D12_RESOURCE_FLAGS resourceFlags = D3D12_RESOURCE_FLAG_NONE, D3D12_CLEAR_VALUE* clearValue = nullptr);

	//Srv Manager
	int m_srvStaticOffsetCount = 300;
	int m_srvDynamicOffsetCount = 320;
	std::list<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_lShaderResourceView;
	CD3DX12_GPU_DESCRIPTOR_HANDLE CreateDynamicSrvWithTexture(ID3D12Resource* textureResource, DXGI_FORMAT format);
	CD3DX12_GPU_DESCRIPTOR_HANDLE CreateStaticSrvWithTexture(ID3D12Resource* textureResource, DXGI_FORMAT format);

	//Uav Manager
	int m_uavOffsetCount = 400;
	std::list<CD3DX12_CPU_DESCRIPTOR_HANDLE> m_lUnorderedAccessView;
	CD3DX12_GPU_DESCRIPTOR_HANDLE CreateUavTexture(ID3D12Resource* textureResource);

	//Dsv Manager
	int m_dsvOffsetCount = 0;
	std::list<GC_DESCRIPTOR_RESOURCE*> m_lDepthStencilView;
	GC_DESCRIPTOR_RESOURCE* CreateDepthStencilBufferAndView(DXGI_FORMAT depthStencilFormat, D3D12_RESOURCE_STATES resourceFlags);
	
	GCShader* m_postProcessingShader;
	GCComputeShader* m_postProcessingShaderCS;
	// Object / Layers Buffer Id Resources
	GCShader* m_objectBufferIdShader;
};

// #TODO optimiser les srv dynamic, les passer en statique?