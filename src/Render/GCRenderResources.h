#pragma once

struct GC_DESCRIPTOR_RESOURCE
{
	ID3D12Resource* resource;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
};

class GCRenderResources {
public:

	//Double RTV Resource For Final Render
	inline ID3D12Resource* CurrentBackBuffer() const { return m_SwapChainBuffer[m_CurrBackBuffer]; }
	//CPU Handle Address of the both
	inline D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferViewAddress() const { return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_pRtvHeap->GetCPUDescriptorHandleForHeapStart(), m_CurrBackBuffer, m_rtvDescriptorSize); }
	//Render format
	inline DXGI_FORMAT GetBackBufferFormat() const { return m_BackBufferFormat; }

	//Msaa
	inline bool Get4xMsaaState() const { return m_4xMsaaState; }
	inline UINT Get4xMsaaQuality() const { return m_4xMsaaQuality; }

	// CPU Handle address Depth Stencil Final Render
	inline D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilViewAddress() const { return m_DepthStencilBufferAddress; }
	//It's format
	inline DXGI_FORMAT GetDepthStencilFormat() const { return m_DepthStencilFormat; }

	//DirectX12 Instances
	inline ID3D12GraphicsCommandList* GetCommandList() const { return m_CommandList; }
	inline ID3D12Device* Getmd3dDevice() const { return m_d3dDevice; }
	inline ID3D12CommandQueue* GetCommandQueue() const { return m_CommandQueue; }
	inline ID3D12CommandAllocator* GetCommandAllocator() const { return m_DirectCmdListAlloc; }
	inline ID3D12Fence* GetFence() { return m_Fence; }

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
	UINT64 m_CurrentFence = 0;

	//Descriptor heaps
	ID3D12DescriptorHeap* m_pRtvHeap;
	ID3D12DescriptorHeap* m_pDsvHeap;
	ID3D12DescriptorHeap* m_pCbvSrvUavDescriptorHeap;

	//Descriptors size
	UINT m_rtvDescriptorSize = 0;
	UINT m_dsvDescriptorSize = 0;
	UINT m_cbvSrvUavDescriptorSize = 0;

	//State var
	bool m_canResize = true;
	int m_CurrBackBuffer = 0;

	//Format
	DXGI_FORMAT m_DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DXGI_FORMAT m_BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	//Msaa
	bool      m_4xMsaaState = false;    // 4X MSAA enabled
	UINT      m_4xMsaaQuality = 0;      // quality level of 4X MSAA

	//Screen
	D3D12_VIEWPORT m_ScreenViewport;
	D3D12_RECT m_ScissorRect;

	CD3DX12_STATIC_SAMPLER_DESC staticSample;

	//Post Processing Resources
	ID3D12Resource* m_pPostProcessingRtv;
	CD3DX12_CPU_DESCRIPTOR_HANDLE m_pPostProcessingRtvAddress;

	//Object / Layers Buffer Id Resources -> #TODO Change the norm for object id, layers id
	ID3D12Resource* m_ObjectIdBufferRtv; //Rtv Buffer
	CD3DX12_CPU_DESCRIPTOR_HANDLE m_ObjectIdBufferRtvAddress; //Cpu Handle Address
	//#TODO Use the principal, in reading
	ID3D12Resource* m_ObjectIdDepthStencilBuffer; //Rtv Buffer
	CD3DX12_CPU_DESCRIPTOR_HANDLE m_ObjectIdDepthStencilBufferAddress; //Cpu Handle Address


	//Descriptor Heap Creation
	void CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible, ID3D12DescriptorHeap** ppDescriptorHeap);

	// Rtv Manager
	int m_rtvOffsetCount = 2;
	std::list<GC_DESCRIPTOR_RESOURCE*> m_lRenderTargets;
	GC_DESCRIPTOR_RESOURCE* CreateRTVTexture(DXGI_FORMAT format, D3D12_RESOURCE_FLAGS resourceFlags = D3D12_RESOURCE_FLAG_NONE, D3D12_CLEAR_VALUE* clearValue = nullptr);
	GC_DESCRIPTOR_RESOURCE* CreateRTV(ID3D12Resource* pResource);
	//*




	// Srv Manager
	int m_srvOffsetCount = 0;
	std::list<GC_DESCRIPTOR_RESOURCE*> m_lShaderResourceView;
	//*

	// Dsv Manager
	int m_dsvOffsetCount = 0;
	std::list<GC_DESCRIPTOR_RESOURCE*> m_lDepthStencilView;
	GC_DESCRIPTOR_RESOURCE* CreateDepthStencilBufferAndView(DXGI_FORMAT depthStencilFormat);




	//ID3D12Resource* CreateRTT(bool test);
	//void DeleteRenderTarget(ID3D12Resource* pRenderTarget);
	//int testCount = 0;
	//std::vector<ID3D12Resource*> m_renderTargets;



};

#ifndef ReleaseCom
#define ReleaseCom(x) { if(x){ x->Release(); x = 0; } }
#endif