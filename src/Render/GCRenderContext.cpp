#include "pch.h"

GCRenderContext::GCRenderContext() {

}

bool GCRenderContext::Initialize(Window* pWindow, int renderWidth, int renderHeight, GCGraphics* pGraphics)
{
	if (!CHECK_POINTERSNULL("Graphics Initialized with window sucessfully", "Can't initialize Graphics, Window is empty", pWindow))
		return false;

	m_pGCRenderResources = new GCRenderResources();
	m_pGCRenderResources->m_pGraphics = pGraphics;
	m_pGCRenderResources->m_renderWidth = renderWidth;
	m_pGCRenderResources->m_renderHeight = renderHeight;
	m_pGCRenderResources->m_pWindow = pWindow;


	InitDirect3D();
	OnResize();

	return true;
}

void GCRenderContext::ResetCommandList() 
{
	m_pGCRenderResources->m_CommandList->Reset(m_pGCRenderResources->m_DirectCmdListAlloc, nullptr);
}

void GCRenderContext::ExecuteCommandList() 
{
	ID3D12CommandList* cmdsLists[] = { m_pGCRenderResources->m_CommandList };
	m_pGCRenderResources->m_CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
}

void GCRenderContext::CloseCommandList() 
{
	m_pGCRenderResources->m_CommandList->Close();
}

void GCRenderContext::EnableDebugController() 
{
	#if defined(DEBUG) || defined(_DEBUG) 
		// Enable the D3D12 debug layer.
		{
			ID3D12Debug* debugController;
			D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
			debugController->EnableDebugLayer();
		}
	#endif
}

bool GCRenderContext::InitDirect3D()
{
	EnableDebugController();

	CreateDXGIFactory1(IID_PPV_ARGS(&m_pGCRenderResources->m_dxgiFactory));

	// Try to create hardware device.
	HRESULT hardwareResult = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pGCRenderResources->m_d3dDevice));

	// Fallback to WARP device.
	if (FAILED(hardwareResult))
	{
		IDXGIAdapter* pWarpAdapter;
		m_pGCRenderResources->m_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter));

		D3D12CreateDevice(
			pWarpAdapter,
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&m_pGCRenderResources->m_d3dDevice));
	}

	m_pGCRenderResources->m_d3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pGCRenderResources->m_Fence));

	m_pGCRenderResources->m_rtvDescriptorSize = m_pGCRenderResources->m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	m_pGCRenderResources->m_dsvDescriptorSize = m_pGCRenderResources->m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	m_pGCRenderResources->m_cbvSrvUavDescriptorSize = m_pGCRenderResources->m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
	msQualityLevels.Format = m_pGCRenderResources->m_BackBufferFormat;
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;

	m_pGCRenderResources->m_d3dDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,&msQualityLevels, sizeof(msQualityLevels));

	m_pGCRenderResources->m_4xMsaaQuality = msQualityLevels.NumQualityLevels;
	assert(m_pGCRenderResources->m_4xMsaaQuality > 0 && "Unexpected MSAA quality level.");

	#ifdef _DEBUG
		LogAdapters();
	#endif

	CreateCommandObjects();
	CreateSwapChain();
	CreateRtvAndDsvDescriptorHeaps();
	CreateCbvSrvUavDescriptorHeaps();

	CreatePostProcessingResources();

	m_pGCRenderResources->m_canResize = true;

	return true;
}

void GCRenderContext::LogAdapters()
{
	UINT i = 0;
	IDXGIAdapter* adapter = nullptr;
	std::vector<IDXGIAdapter*> adapterList;
	while (m_pGCRenderResources->m_dxgiFactory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);

		std::wstring text = L"***Adapter: ";
		text += desc.Description;
		text += L"\n";

		OutputDebugString(text.c_str());

		adapterList.push_back(adapter);

		++i;
	}

	for (size_t i = 0; i < adapterList.size(); ++i)
	{
		LogAdapterOutputs(adapterList[i]);
		ReleaseCom(adapterList[i]);
	}
}

void GCRenderContext::LogAdapterOutputs(IDXGIAdapter* pAdapter)
{
	UINT i = 0;
	IDXGIOutput* output = nullptr;
	while (pAdapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_OUTPUT_DESC desc;
		output->GetDesc(&desc);

		std::wstring text = L"***Output: ";
		text += desc.DeviceName;
		text += L"\n";
		OutputDebugString(text.c_str());

		LogOutputDisplayModes(output, m_pGCRenderResources->m_BackBufferFormat);

		ReleaseCom(output);

		++i;
	}
	m_pGCRenderResources->m_canResize = true;
}

void GCRenderContext::CreateCommandObjects()
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	m_pGCRenderResources->m_d3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_pGCRenderResources->m_CommandQueue));

	m_pGCRenderResources->m_d3dDevice->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pGCRenderResources->m_DirectCmdListAlloc));

	m_pGCRenderResources->m_d3dDevice->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_pGCRenderResources->m_DirectCmdListAlloc, // Associated command allocator
		nullptr, // Initial PipelineStateObject
		IID_PPV_ARGS(&m_pGCRenderResources->m_CommandList)
	);

	// Start off in a closed state.  This is because the first time we refer 
	// to the command list we will Reset it, and it needs to be closed before
	// calling Reset.
	m_pGCRenderResources->m_CommandList->Close();
}

void GCRenderContext::CreateSwapChain()
{
	// Release the previous swapchain we will be recreating.
	if (m_pGCRenderResources->m_SwapChain != nullptr)
	{
		m_pGCRenderResources->m_SwapChain->Release();
	}

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = m_pGCRenderResources->m_pWindow->GetClientWidth();
	sd.BufferDesc.Height = m_pGCRenderResources->m_pWindow->GetClientHeight();
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = m_pGCRenderResources->m_BackBufferFormat;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = m_pGCRenderResources->m_4xMsaaState ? 4 : 1;
	sd.SampleDesc.Quality = m_pGCRenderResources->m_4xMsaaState ? (m_pGCRenderResources->m_4xMsaaQuality - 1) : 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	sd.OutputWindow = m_pGCRenderResources->m_pWindow->GetHMainWnd();
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// Note: Swap chain uses queue to perform flush.
	m_pGCRenderResources->m_dxgiFactory->CreateSwapChain(m_pGCRenderResources->m_CommandQueue, &sd, &m_pGCRenderResources->m_SwapChain);
}

void GCRenderContext::CreateCbvSrvUavDescriptorHeaps()
{
	// Create CBV / SRV / UAV descriptor heap
	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc = {};
	cbvHeapDesc.NumDescriptors = 1000;
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvHeapDesc.NodeMask = 0;
	m_pGCRenderResources->m_d3dDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&m_pGCRenderResources->m_pCbvSrvUavDescriptorHeap));
}

void GCRenderContext::CreateRtvAndDsvDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
	rtvHeapDesc.NumDescriptors = m_pGCRenderResources->SwapChainBufferCount + 2;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;
	m_pGCRenderResources->m_d3dDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_pGCRenderResources->m_pRtvHeap));


	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
	dsvHeapDesc.NumDescriptors = 2;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask = 0;
	m_pGCRenderResources->m_d3dDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_pGCRenderResources->m_pDsvHeap));
}

void GCRenderContext::CreatePostProcessingResources() {
	m_pGCRenderResources->m_pPostProcessingRtv= m_pGCRenderResources->CreateRTT(true);
	

	// Create A post Processing Shader
	m_postProcessingShader = new GCShader();

	std::string shaderFilePath = "../../../src/Render/Shaders/PostProcessing.hlsl";
	std::string csoDestinationPath = "../../../src/Render/CsoCompiled/PostProcessing";

	int flags = 0;
	SET_FLAG(flags, VERTEX_POSITION);
	SET_FLAG(flags, VERTEX_UV);

	m_postProcessingShader->Initialize(this, shaderFilePath, csoDestinationPath, flags);
	m_postProcessingShader->Load();



	// Create RTV For Object Buffer Id For pass Mesh id to pixel, to apply them on a texture 
	m_pGCRenderResources->m_ObjectIdBufferRtv = m_pGCRenderResources->CreateRTT(false);

	//// Create Object buffer Id Shader 
	int flags2 = 0;
	SET_FLAG(flags2, VERTEX_POSITION);

	m_objectBufferIdShader = new GCShader();
	std::string shaderFilePath2 = "../../../src/Render/Shaders/ObjectBufferId.hlsl";
	std::string csoDestinationPath2 = "../../../src/Render/CsoCompiled/ObjectBufferId";

	m_objectBufferIdShader->Initialize(this, shaderFilePath2, csoDestinationPath2, flags2);
	m_objectBufferIdShader->Load();
	
}

void GCRenderContext::OnResize() 
{
	// Checks initial conditions
	m_pGCRenderResources->m_canResize = true;
	if (m_pGCRenderResources->m_canResize == false)
		return;

	assert(m_pGCRenderResources->m_d3dDevice);
	assert(m_pGCRenderResources->m_SwapChain);
	assert(m_pGCRenderResources->m_DirectCmdListAlloc);

	FlushCommandQueue();

	ResetCommandList();

	ReleasePreviousResources();
	ResizeSwapChain();
	CreateRenderTargetViews();
	CreateDepthStencilBufferAndView();

	CloseCommandList();
	ExecuteCommandList();
	FlushCommandQueue();

	UpdateViewport();
}

void GCRenderContext::ReleasePreviousResources() 
{
	for (int i = 0; i < m_pGCRenderResources->SwapChainBufferCount; ++i)
	{
		if (m_pGCRenderResources->m_SwapChainBuffer[i] != nullptr)
		{
			m_pGCRenderResources->m_SwapChainBuffer[i]->Release();
		}
	}
	if (m_pGCRenderResources->m_DepthStencilBuffer != nullptr)
	{
		m_pGCRenderResources->m_DepthStencilBuffer->Release();
	}
}

void GCRenderContext::ResizeSwapChain() 
{
	m_pGCRenderResources->m_SwapChain->ResizeBuffers(
		m_pGCRenderResources->SwapChainBufferCount,
		m_pGCRenderResources->m_pWindow->GetClientWidth(), m_pGCRenderResources->m_pWindow->GetClientHeight(),
		m_pGCRenderResources->m_BackBufferFormat,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	);

	m_pGCRenderResources->m_CurrBackBuffer = 0;
}

void GCRenderContext::CreateRenderTargetViews() 
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_pGCRenderResources->m_pRtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < m_pGCRenderResources->SwapChainBufferCount; i++)
	{
		m_pGCRenderResources->m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_pGCRenderResources->m_SwapChainBuffer[i]));
		m_pGCRenderResources->m_d3dDevice->CreateRenderTargetView(m_pGCRenderResources->m_SwapChainBuffer[i], nullptr, rtvHeapHandle);
		rtvHeapHandle.Offset(1, m_pGCRenderResources->m_rtvDescriptorSize);
	}

	//ID3D12Resource* renderTargetTexture = nullptr;

	//// Define the texture description
	//D3D12_RESOURCE_DESC textureDesc = {};
	//textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	//textureDesc.Width = 1024; // Example width
	//textureDesc.Height = 1024; // Example height
	//textureDesc.DepthOrArraySize = 1;
	//textureDesc.MipLevels = 1;
	//textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//textureDesc.SampleDesc.Count = 1;
	//textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	//// Define the clear value
	//D3D12_CLEAR_VALUE clearValue = {};
	//clearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//clearValue.Color[0] = 0.0f;
	//clearValue.Color[1] = 0.0f;
	//clearValue.Color[2] = 0.0f;
	//clearValue.Color[3] = 1.0f;

	//// Create the render target texture
	//CD3DX12_HEAP_PROPERTIES test = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	//HRESULT hr = m_d3dDevice->CreateCommittedResource(
	//	&test,
	//	D3D12_HEAP_FLAG_NONE,
	//	&textureDesc,
	//	D3D12_RESOURCE_STATE_RENDER_TARGET,
	//	&clearValue,
	//	IID_PPV_ARGS(&renderTargetTexture)
	//);
	//if (FAILED(hr)) {
	//	// Handle error
	//	MessageBoxA(nullptr, "Failed to create render target texture", "Error", MB_OK);
	//	return ;
	//}

	//m_d3dDevice->CreateRenderTargetView(renderTargetTexture, nullptr, rtvHeapHandle);
	//rtvHeapHandle.Offset(1, m_rtvDescriptorSize);
}

void GCRenderContext::CreateDepthStencilBufferAndView()
{
	D3D12_RESOURCE_DESC depthStencilDesc = {};
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = m_pGCRenderResources->m_pWindow->GetClientWidth();
	depthStencilDesc.Height = m_pGCRenderResources->m_pWindow->GetClientHeight();
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.Format = m_pGCRenderResources->m_DepthStencilFormat;
	depthStencilDesc.SampleDesc.Count = m_pGCRenderResources->m_4xMsaaState ? 4 : 1;
	depthStencilDesc.SampleDesc.Quality = m_pGCRenderResources->m_4xMsaaState ? (m_pGCRenderResources->m_4xMsaaQuality - 1) : 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear = {};
	optClear.Format = m_pGCRenderResources->m_DepthStencilFormat;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;

	CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_DEFAULT);
	m_pGCRenderResources->m_d3dDevice->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&depthStencilDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&optClear,
		IID_PPV_ARGS(&m_pGCRenderResources->m_DepthStencilBuffer)
	);

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = m_pGCRenderResources->m_DepthStencilFormat;
	dsvDesc.Texture2D.MipSlice = 0;
	m_pGCRenderResources->m_d3dDevice->CreateDepthStencilView(m_pGCRenderResources->m_DepthStencilBuffer, &dsvDesc, m_pGCRenderResources->GetDepthStencilViewAddress());

	CD3DX12_RESOURCE_BARRIER CommonToDepthWrite(CD3DX12_RESOURCE_BARRIER::Transition(m_pGCRenderResources->m_DepthStencilBuffer, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));
	m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &CommonToDepthWrite);

	// Second depth stencil for Mesh buffer id 
	m_pGCRenderResources->m_d3dDevice->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&depthStencilDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&optClear,
		IID_PPV_ARGS(&m_pGCRenderResources->m_ObjectIdDepthStencilBuffer)
	);


	m_pGCRenderResources->m_ObjectIdDepthStencilBufferAddress = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_pGCRenderResources->m_pDsvHeap->GetCPUDescriptorHandleForHeapStart());
	m_pGCRenderResources->m_ObjectIdDepthStencilBufferAddress.Offset(1, m_pGCRenderResources->m_dsvDescriptorSize);
	m_pGCRenderResources->m_d3dDevice->CreateDepthStencilView(m_pGCRenderResources->m_ObjectIdDepthStencilBuffer, &dsvDesc, m_pGCRenderResources->m_ObjectIdDepthStencilBufferAddress);

	CD3DX12_RESOURCE_BARRIER CommonToDepthWrite2(CD3DX12_RESOURCE_BARRIER::Transition(m_pGCRenderResources->m_ObjectIdDepthStencilBuffer, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));
	m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &CommonToDepthWrite2);
}

void GCRenderContext::UpdateViewport() 
{
	m_pGCRenderResources->m_ScreenViewport.TopLeftX = (m_pGCRenderResources->m_pWindow->GetClientWidth() - m_pGCRenderResources->m_renderWidth) / 2;
	m_pGCRenderResources->m_ScreenViewport.TopLeftY = (m_pGCRenderResources->m_pWindow->GetClientHeight() - m_pGCRenderResources->m_renderHeight) / 2;
	m_pGCRenderResources->m_ScreenViewport.Width = static_cast<float>(m_pGCRenderResources->m_renderWidth);
	m_pGCRenderResources->m_ScreenViewport.Height = static_cast<float>(m_pGCRenderResources->m_renderHeight);
	m_pGCRenderResources->m_ScreenViewport.MinDepth = 0.0f;
	m_pGCRenderResources->m_ScreenViewport.MaxDepth = 1.0f;

	m_pGCRenderResources->m_ScissorRect = {
		(m_pGCRenderResources->m_pWindow->GetClientWidth() - m_pGCRenderResources->m_renderWidth) / 2,
		(m_pGCRenderResources->m_pWindow->GetClientHeight() - m_pGCRenderResources->m_renderHeight) / 2,
		(m_pGCRenderResources->m_pWindow->GetClientWidth() - m_pGCRenderResources->m_renderWidth)/ 2 + m_pGCRenderResources->m_renderWidth,
		(m_pGCRenderResources->m_pWindow->GetClientHeight() - m_pGCRenderResources->m_renderHeight) / 2 + m_pGCRenderResources->m_renderHeight
	};
}

bool GCRenderContext::FlushCommandQueue()
{
	HRESULT hr;
	// Advance the fence value to mark commands up to this fence point.
	m_pGCRenderResources->m_CurrentFence++;
	// Add an instruction to the command queue to set a new fence point.  Because we 
	// are on the GPU timeline, the new fence point won't be set until the GPU finishes
	// processing all the commands prior to this Signal().
	hr = m_pGCRenderResources->m_CommandQueue->Signal(m_pGCRenderResources->m_Fence, m_pGCRenderResources->m_CurrentFence);
	if (!CHECK_HRESULT(hr, "m_CommandQueue->Signal")) {
		return false;
	};

	// Wait until the GPU has completed commands up to this fence point.
	if (m_pGCRenderResources->m_Fence->GetCompletedValue() < m_pGCRenderResources->m_CurrentFence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, NULL, false, EVENT_ALL_ACCESS);

		// Fire event when GPU hits current fence.  
		hr = m_pGCRenderResources->m_Fence->SetEventOnCompletion(m_pGCRenderResources->m_CurrentFence, eventHandle);
		if (!CHECK_HRESULT(hr, "Fence->SetEventOnCompletion")) {
			return false;
		};

		// Wait until the GPU hits current fence event is fired.
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
	return true;
}


bool GCRenderContext::PrepareDraw() 
{

	HRESULT hr = m_pGCRenderResources->m_DirectCmdListAlloc->Reset();
	if (!CHECK_HRESULT(hr, "m_DirectCmdListAlloc->Reset()")) {
		return false;
	};

	hr = m_pGCRenderResources->m_CommandList->Reset(m_pGCRenderResources->m_DirectCmdListAlloc, nullptr);

	if (!CHECK_HRESULT(hr, "m_CommandList->Reset()")) {
		return false;
	};

	m_pGCRenderResources->m_CommandList->RSSetViewports(1, &m_pGCRenderResources->m_ScreenViewport);
	m_pGCRenderResources->m_CommandList->RSSetScissorRects(1, &m_pGCRenderResources->m_ScissorRect);

	// Swap
	CD3DX12_RESOURCE_BARRIER ResBar(CD3DX12_RESOURCE_BARRIER::Transition(m_pGCRenderResources->CurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
	m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &ResBar);

	CD3DX12_RESOURCE_BARRIER CommonToRT(CD3DX12_RESOURCE_BARRIER::Transition(m_pGCRenderResources->m_ObjectIdBufferRtv, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_RENDER_TARGET));
	m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &CommonToRT);


	m_pGCRenderResources->m_CommandList->ClearRenderTargetView(m_pGCRenderResources->CurrentBackBufferViewAddress(), DirectX::Colors::LightBlue, 1, &m_pGCRenderResources->m_ScissorRect);
	m_pGCRenderResources->m_CommandList->ClearDepthStencilView(m_pGCRenderResources->GetDepthStencilViewAddress(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	//m_pGCRenderResources->m_CommandList->ClearRenderTargetView(m_pGCRenderResources->m_ObjectIdBufferRtvAddress, DirectX::Colors::LightBlue, 1, &m_pGCRenderResources->m_ScissorRect);
	/*m_pGCRenderResources->m_CommandList->ClearDepthStencilView(m_pGCRenderResources->m_ObjectIdDepthStencilBufferAddress, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);*/


	D3D12_CPU_DESCRIPTOR_HANDLE dsv = m_pGCRenderResources->GetDepthStencilViewAddress();
	D3D12_CPU_DESCRIPTOR_HANDLE rtv = m_pGCRenderResources->CurrentBackBufferViewAddress();
	m_pGCRenderResources->m_CommandList->OMSetRenderTargets(1, &rtv, true, &dsv);

	ID3D12DescriptorHeap* descriptorHeaps[] = { m_pGCRenderResources->m_pCbvSrvUavDescriptorHeap };
	m_pGCRenderResources->m_CommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	return true;
}






bool GCRenderContext::DrawObject(GCMesh* pMesh, GCMaterial* pMaterial, bool alpha)
{
	if (pMaterial == nullptr || pMaterial->GetShader() == nullptr || pMesh == nullptr)
		return false;
	if (!COMPARE_SHADER_MESH_FLAGS(pMaterial, pMesh)) 
		return false;

	//Basic Draw
	{
		m_pGCRenderResources->m_CommandList->SetPipelineState(pMaterial->GetShader()->GetPso(alpha));
		m_pGCRenderResources->m_CommandList->SetGraphicsRootSignature(pMaterial->GetShader()->GetRootSign());

		//if (pMaterial->GetShader()->m_pRtt == nullptr)
		//{
		//	D3D12_CPU_DESCRIPTOR_HANDLE dsv = m_pGCRenderResources->GetDepthStencilViewAddress();
		//	D3D12_CPU_DESCRIPTOR_HANDLE cbbv = m_pGCRenderResources->CurrentBackBufferViewAddress();
		//	m_pGCRenderResources->m_CommandList->OMSetRenderTargets(1, &cbbv, true, &dsv);
		//}
		//else 
		//{
		//	D3D12_CPU_DESCRIPTOR_HANDLE dsv = m_pGCRenderResources->GetDepthStencilViewAddress();
		//	CD3DX12_CPU_DESCRIPTOR_HANDLE test = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_pGCRenderResources->m_pRtvHeap->GetCPUDescriptorHandleForHeapStart(), 2, m_pGCRenderResources->m_rtvDescriptorSize);
		//	//CD3DX12_RESOURCE_BARRIER ResBar(CD3DX12_RESOURCE_BARRIER::Transition(pMaterial->GetShader()->m_pRtt, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
		//	//m_CommandList->ResourceBarrier(1, &ResBar);
		//	//m_CommandList->ClearRenderTargetView(test, DirectX::Colors::LightBlue, 1, &m_ScissorRect);
		//	//m_CommandList->ClearDepthStencilView(GetDepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
		//	m_pGCRenderResources->m_CommandList->OMSetRenderTargets(1, &test, true, &dsv);
		//}


		m_pGCRenderResources->m_CommandList->SetPipelineState(pMaterial->GetShader()->GetPso(alpha));
		m_pGCRenderResources->m_CommandList->SetGraphicsRootSignature(pMaterial->GetShader()->GetRootSign());

		m_pGCRenderResources->m_CommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView = pMesh->GetBufferGeometryData()->VertexBufferView();
		m_pGCRenderResources->m_CommandList->IASetVertexBuffers(0, 1, &vertexBufferView);
		D3D12_INDEX_BUFFER_VIEW indexBufferView = pMesh->GetBufferGeometryData()->IndexBufferView();
		m_pGCRenderResources->m_CommandList->IASetIndexBuffer(&indexBufferView);

		// Update texture if material has texture
		pMaterial->UpdateTexture();

		// Update cb0, cb of object
		m_pGCRenderResources->m_CommandList->SetGraphicsRootConstantBufferView(CBV_SLOT_CB0, pMaterial->GetCbObjectInstance()[pMaterial->GetCount()]->Resource()->GetGPUVirtualAddress());
		// Set cb object buffer on used
		pMaterial->GetCbObjectInstance()[pMaterial->GetCount()]->m_isUsed = true;
		pMaterial->IncrementCBCount();

		// cb1, Camera
		m_pGCRenderResources->m_CommandList->SetGraphicsRootConstantBufferView(CBV_SLOT_CB1, m_pCbCurrentViewProjInstance->Resource()->GetGPUVirtualAddress());
		// cb2, Material Properties
		m_pGCRenderResources->m_CommandList->SetGraphicsRootConstantBufferView(CBV_SLOT_CB2, pMaterial->GetCbMaterialPropertiesInstance()->Resource()->GetGPUVirtualAddress());
		// cb3, Light Properties
		m_pGCRenderResources->m_CommandList->SetGraphicsRootConstantBufferView(CBV_SLOT_CB3, m_pCbLightPropertiesInstance->Resource()->GetGPUVirtualAddress());

		// Draw
		m_pGCRenderResources->m_CommandList->DrawIndexedInstanced(pMesh->GetBufferGeometryData()->IndexCount, 1, 0, 0, 0);
	}



	//Object/Layers Buffer Id Draw
	{
		//m_pGCRenderResources->m_CommandList->SetPipelineState(m_objectBufferIdShader->GetPso(alpha));
		//m_pGCRenderResources->m_CommandList->SetGraphicsRootSignature(m_objectBufferIdShader->GetRootSign());

		//D3D12_CPU_DESCRIPTOR_HANDLE dsv = m_pGCRenderResources->GetDepthStencilViewAddress();

		////Read State 
		////CD3DX12_RESOURCE_BARRIER CommonToRead(CD3DX12_RESOURCE_BARRIER::Transition(m_pGCRenderResources->m_DepthStencilBuffer, D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_DEPTH_READ));
		////m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &CommonToRead);

		//m_pGCRenderResources->m_CommandList->OMSetRenderTargets(1, &m_pGCRenderResources->m_ObjectIdBufferRtvAddress, TRUE, &m_pGCRenderResources->m_ObjectIdDepthStencilBufferAddress);

		//m_pGCRenderResources->m_CommandList->DrawIndexedInstanced(pMesh->GetBufferGeometryData()->IndexCount, 1, 0, 0, 0);

		////Write State
		////CD3DX12_RESOURCE_BARRIER ReadToWrite(CD3DX12_RESOURCE_BARRIER::Transition(m_pGCRenderResources->m_DepthStencilBuffer, D3D12_RESOURCE_STATE_DEPTH_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE));
		////m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &ReadToWrite);

		//// Set Again Old Render target

		//D3D12_CPU_DESCRIPTOR_HANDLE rtv = m_pGCRenderResources->CurrentBackBufferViewAddress();
		//m_pGCRenderResources->m_CommandList->OMSetRenderTargets(1, &rtv, true, &dsv);
	}
	


	//// Object Buffer id ******************
	//
	//m_pGCRenderResources->m_CommandList->SetPipelineState(m_objectBufferIdShader->GetPso(alpha));
	//m_pGCRenderResources->m_CommandList->SetGraphicsRootSignature(m_objectBufferIdShader->GetRootSign());
	//CD3DX12_CPU_DESCRIPTOR_HANDLE test2 = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_pGCRenderResources->m_pRtvHeap->GetCPUDescriptorHandleForHeapStart(), 3, m_pGCRenderResources->m_rtvDescriptorSize);
	//m_pGCRenderResources->m_CommandList->OMSetRenderTargets(1, &test2, TRUE, &m_pGCRenderResources->m_ObjectIdDepthStencilBufferAddress);
	//m_pGCRenderResources->m_CommandList->DrawIndexedInstanced(pMesh->GetBufferGeometryData()->IndexCount, 1, 0, 0, 0);
	//// ***


	//// Set Again Old Render target
	//
	//D3D12_CPU_DESCRIPTOR_HANDLE dsv = m_pGCRenderResources->GetDepthStencilViewAddress();
	//D3D12_CPU_DESCRIPTOR_HANDLE rtv = m_pGCRenderResources->CurrentBackBufferViewAddress();
	//m_pGCRenderResources->m_CommandList->OMSetRenderTargets(1, &rtv, true, &dsv);

	// ******************

	return true;
}

// #TODO RESTRUCTURE RENDER -> GCRenderContext GCRenderResources -> Compute Shader

void GCRenderContext::PerformPostProcessing()
{
	// Transition pour le rendu sur m_pPostProcessingRtv
	CD3DX12_RESOURCE_BARRIER barrierToRT = CD3DX12_RESOURCE_BARRIER::Transition(m_pGCRenderResources->m_pPostProcessingRtv, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &barrierToRT);

	// Set Render target
	D3D12_CPU_DESCRIPTOR_HANDLE dsv = m_pGCRenderResources->GetDepthStencilViewAddress();
	m_pGCRenderResources->m_CommandList->OMSetRenderTargets(1, &m_pGCRenderResources->m_pPostProcessingRtvAddress, FALSE, &dsv);

	// Root Sign / Pso
	m_pGCRenderResources->m_CommandList->SetPipelineState(m_postProcessingShader->GetPso(true));
	m_pGCRenderResources->m_CommandList->SetGraphicsRootSignature(m_postProcessingShader->GetRootSign());
	CD3DX12_CPU_DESCRIPTOR_HANDLE test = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_pGCRenderResources->m_pRtvHeap->GetCPUDescriptorHandleForHeapStart(), 2, m_pGCRenderResources->m_rtvDescriptorSize);
	m_pGCRenderResources->m_CommandList->OMSetRenderTargets(1, &test, FALSE, &dsv);

	// Root Sign / Pso
	m_pGCRenderResources->m_CommandList->SetPipelineState(m_postProcessingShader->GetPso(true));
	m_pGCRenderResources->m_CommandList->SetGraphicsRootSignature(m_postProcessingShader->GetRootSign());

	// Transition pour la texture de post-processing (g_Texture)
	CD3DX12_RESOURCE_BARRIER barrierToShaderResource = CD3DX12_RESOURCE_BARRIER::Transition(m_pGCRenderResources->CurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &barrierToShaderResource);

	// Post process texture linking to shader
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = m_pGCRenderResources->GetBackBufferFormat();
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	CD3DX12_CPU_DESCRIPTOR_HANDLE srvCpuHandle(m_pGCRenderResources->m_pCbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	srvCpuHandle.Offset(90, m_pGCRenderResources->m_cbvSrvUavDescriptorSize);
	m_pGCRenderResources->m_d3dDevice->CreateShaderResourceView(m_pGCRenderResources->CurrentBackBuffer(), &srvDesc, srvCpuHandle);
	CD3DX12_GPU_DESCRIPTOR_HANDLE srvGpuHandle(m_pGCRenderResources->m_pCbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
	srvGpuHandle.Offset(90, m_pGCRenderResources->m_cbvSrvUavDescriptorSize);
	m_pGCRenderResources->m_CommandList->SetGraphicsRootDescriptorTable(DESCRIPTOR_TABLE_SLOT_TEXTURE, srvGpuHandle);

	// Transition pour la texture d'Object ID (g_ObjectIdBuffer)
	CD3DX12_RESOURCE_BARRIER barrierToShaderResource2 = CD3DX12_RESOURCE_BARRIER::Transition(m_pGCRenderResources->m_ObjectIdBufferRtv, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &barrierToShaderResource2);

	// Object / Layers buffer id Linking to shader
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2 = {};
	srvDesc2.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc2.Texture2D.MostDetailedMip = 0;
	srvDesc2.Texture2D.MipLevels = 1;
	srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	CD3DX12_CPU_DESCRIPTOR_HANDLE srvCpuHandle2(m_pGCRenderResources->m_pCbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	srvCpuHandle2.Offset(150, m_pGCRenderResources->m_cbvSrvUavDescriptorSize);
	m_pGCRenderResources->m_d3dDevice->CreateShaderResourceView(m_pGCRenderResources->m_ObjectIdBufferRtv, &srvDesc2, srvCpuHandle2);
	CD3DX12_GPU_DESCRIPTOR_HANDLE srvGpuHandle2(m_pGCRenderResources->m_pCbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
	srvGpuHandle2.Offset(150, m_pGCRenderResources->m_cbvSrvUavDescriptorSize);
	m_pGCRenderResources->m_CommandList->SetGraphicsRootDescriptorTable(DESCRIPTOR_TABLE_SLOT_TEXTURE2, srvGpuHandle2);
	//

	// Draw quad on post process rtv
	GCMesh* theMesh = m_pGCRenderResources->m_pGraphics->GetMeshes()[0];
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView = theMesh->GetBufferGeometryData()->VertexBufferView();
	D3D12_INDEX_BUFFER_VIEW indexBufferView = theMesh->GetBufferGeometryData()->IndexBufferView();
	m_pGCRenderResources->m_CommandList->IASetVertexBuffers(0, 1, &vertexBufferView);
	m_pGCRenderResources->m_CommandList->IASetIndexBuffer(&indexBufferView);
	m_pGCRenderResources->m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pGCRenderResources->m_CommandList->DrawIndexedInstanced(theMesh->GetBufferGeometryData()->IndexCount, 1, 0, 0, 0);
	//

	// Transitions pour la copie finale
	CD3DX12_RESOURCE_BARRIER barrierToCopySource2 = CD3DX12_RESOURCE_BARRIER::Transition(m_pGCRenderResources->m_pPostProcessingRtv, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COPY_SOURCE);
	m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &barrierToCopySource2);
	CD3DX12_RESOURCE_BARRIER barrierToCopyDest2 = CD3DX12_RESOURCE_BARRIER::Transition(m_pGCRenderResources->CurrentBackBuffer(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_COPY_DEST);
	m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &barrierToCopyDest2);

	// Copie de m_pPostProcessingRtv sur le rendu final
	m_pGCRenderResources->m_CommandList->CopyResource(m_pGCRenderResources->CurrentBackBuffer(), m_pGCRenderResources->m_pPostProcessingRtv);
}


//Always needs to be called right after drawing!!!
bool GCRenderContext::CompleteDraw()
{
    //PerformPostProcessing();

	CD3DX12_RESOURCE_BARRIER ResBar2(CD3DX12_RESOURCE_BARRIER::Transition(m_pGCRenderResources->CurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
	m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &ResBar2);

	HRESULT hr = m_pGCRenderResources->m_CommandList->Close();
	if (!CHECK_HRESULT(hr, "Failed to close command list"))
		return false;


	ID3D12CommandList* cmdsLists[] = { m_pGCRenderResources->m_CommandList };
	m_pGCRenderResources->m_CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	hr = m_pGCRenderResources->m_SwapChain->Present(0, 0);
	if (!CHECK_HRESULT(hr, "Failed to present swap chain")) {
		return false;
	}

	m_pGCRenderResources->m_CurrBackBuffer = (m_pGCRenderResources->m_CurrBackBuffer + 1) % m_pGCRenderResources->SwapChainBufferCount;

	FlushCommandQueue();

	return true;
}

void GCRenderContext::LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format) 	
{
	UINT count = 0;
	UINT flags = 0;

	// Call with nullptr to get list count.
	output->GetDisplayModeList(format, flags, &count, nullptr);

	std::vector<DXGI_MODE_DESC> modeList(count);
	output->GetDisplayModeList(format, flags, &count, &modeList[0]);

	for (auto& x : modeList)
	{
		UINT n = x.RefreshRate.Numerator;
		UINT d = x.RefreshRate.Denominator;
		std::wstring text =
			L"Width = " + std::to_wstring(x.Width) + L" " +
			L"Height = " + std::to_wstring(x.Height) + L" " +
			L"Refresh = " + std::to_wstring(n) + L"/" + std::to_wstring(d) +
			L"\n";
		::OutputDebugString(text.c_str());
	}
}

