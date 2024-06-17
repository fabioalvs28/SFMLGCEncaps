#pragma once

//struct ObjectConstants
//{
//	DirectX::XMFLOAT4X4 WorldViewProj;
//};


//struct ObjectCB {
//	
//};
//
//struct WorldCB : ObjectCB {
//	DirectX::XMFLOAT4X4 world; // Matrice du monde
//};
//
//struct LightAndWorld : ObjectCB {
//	DirectX::XMFLOAT4X4 world; // Matrice du monde
//	DirectX::XMFLOAT4X4 light; // Matrice du monde
//	DirectX::XMFLOAT4X4 normal; // Matrice du monde
//};








class GCRender
{
public:
	GCRender() {}

	bool Initialize(GCGraphics* pGraphics,Window* pWindow);
	bool InitDirect3D();


	//void BuildConstantBuffers();

	void LogAdapters();
	void LogAdapterOutputs(IDXGIAdapter* adapter);
	void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

	void EnableDebugController();

	void CreateCommandObjects();
	void CreateRtvAndDsvDescriptorHeaps();
	void CreateCbvSrvUavDescriptorHeaps();
	void CreateSwapChain();

	// Resize 



	void ReleasePreviousResources();
	void ResizeSwapChain();
	void CreateRenderTargetViews();
	void CreateDepthStencilBufferAndView();
	void UpdateViewport();


	// Draw Part
	void ResetCommandList();
	void ExecuteCommandList();
	void CloseCommandList();

	void FlushCommandQueue();
	void Update(const Timer& gt);

	void PrepareDraw();
	void PostDraw();
	void Draw(const Timer& gt);

	bool DrawOneObject(GCMesh* pMesh, GCMaterial* pMaterial);
	//void BuildBoxGeometry();


	void OnResize();

	// Getter
	bool Get4xMsaaState();
	ID3D12Device* Getmd3dDevice();
	ID3D12Resource* CurrentBackBuffer() const;
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView() const;
	UINT Get4xMsaaQuality();
	DXGI_FORMAT GetBackBufferFormat();
	DXGI_FORMAT GetDepthStencilFormat();

	ID3D12GraphicsCommandList* GetCommandList();

	// Add getter by william 
	ID3D12CommandQueue* GetCommandQueue() const { return m_CommandQueue; }
	ID3D12CommandAllocator* GetCommandAllocator() const { return m_DirectCmdListAlloc; }

	ID3D12Fence* GetFence() { return m_Fence; }


	ID3D12DescriptorHeap* GetRtvHeap() { return m_rtvHeap; }
	ID3D12DescriptorHeap* GetDsvHeap() { return m_dsvHeap; }
	ID3D12DescriptorHeap* GetCbvSrvUavSrvDescriptorHeap() { return m_cbvSrvUavDescriptorHeap; }
	UINT GetRtvDescriptorSize() const { return m_rtvDescriptorSize; }
	UINT GetDsvDescriptorSize() const { return m_dsvDescriptorSize; }
	UINT GetCbvSrvUavDescriptorSize() const { return m_cbvSrvUavDescriptorSize; }

	template<typename ShaderTypeConstantBuffer>
	GCShaderUploadBuffer<ShaderTypeConstantBuffer>* CreateObjectCB() {
		return new GCShaderUploadBuffer<ShaderTypeConstantBuffer>(Getmd3dDevice(), 1, true);
	}

	GCShaderUploadBuffer<GCCAMERACB>* CreateCameraCB();
	void UpdateBuffers(GCMaterial* pMaterial, DirectX::XMFLOAT4X4 worldMatrix, DirectX::XMFLOAT4X4 projectionMatrix, DirectX::XMFLOAT4X4 viewMatrix);
private:
	
	Window* m_pWindow;
	// Swap chain size
	static const int SwapChainBufferCount = 2;

	// DirectX12 3D instances
	IDXGIFactory4* m_dxgiFactory;
	ID3D12Device* m_d3dDevice;

	ID3D12GraphicsCommandList* m_CommandList;
	IDXGISwapChain* m_SwapChain;
	ID3D12Resource* m_SwapChainBuffer[SwapChainBufferCount];
	ID3D12Resource* m_DepthStencilBuffer;
	ID3D12CommandQueue* m_CommandQueue;
	ID3D12CommandAllocator* m_DirectCmdListAlloc;
	// Fence
	ID3D12Fence* m_Fence;
	UINT64 m_CurrentFence = 0;
	// Descriptor heaps


	ID3D12DescriptorHeap* m_rtvHeap;
	ID3D12DescriptorHeap* m_dsvHeap;
	ID3D12DescriptorHeap* m_cbvSrvUavDescriptorHeap;

	// Descriptors size
	UINT m_rtvDescriptorSize = 0;
	UINT m_dsvDescriptorSize = 0;

	UINT m_cbvSrvUavDescriptorSize = 0;


	// State var
	bool m_canResize = true;
	int m_CurrBackBuffer = 0;

	// Format
	DXGI_FORMAT m_DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DXGI_FORMAT m_BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Msaa
	bool      m_4xMsaaState = false;    // 4X MSAA enabled
	UINT      m_4xMsaaQuality = 0;      // quality level of 4X MSAA


	// Screen
	D3D12_VIEWPORT m_ScreenViewport;
	D3D12_RECT m_ScissorRect;

	// Camera (Temporary)
	CD3DX12_STATIC_SAMPLER_DESC staticSample;

};

#ifndef ReleaseCom
#define ReleaseCom(x) { if(x){ x->Release(); x = 0; } }
#endif
