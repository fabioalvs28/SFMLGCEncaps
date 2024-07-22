#include "pch.h"

GCRender::GCRender() {

}

bool GCRender::Initialize(Window* pWindow, int renderWidth, int renderHeight, GCGraphics* pGraphics)
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

void GCRender::ResetCommandList() 
{
	m_pGCRenderResources->m_CommandList->Reset(m_pGCRenderResources->m_DirectCmdListAlloc, nullptr);
}

void GCRender::ExecuteCommandList() 
{
	ID3D12CommandList* cmdsLists[] = { m_pGCRenderResources->m_CommandList };
	m_pGCRenderResources->m_CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
}

void GCRender::CloseCommandList() 
{
	m_pGCRenderResources->m_CommandList->Close();
}

void GCRender::EnableDebugController() 
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

bool GCRender::InitDirect3D()
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

void GCRender::LogAdapters()
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

void GCRender::LogAdapterOutputs(IDXGIAdapter* pAdapter)
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

void GCRender::CreateCommandObjects()
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

void GCRender::CreateSwapChain()
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

void GCRender::CreateCbvSrvUavDescriptorHeaps()
{
	m_pGCRenderResources->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1000, true, &m_pGCRenderResources->m_pCbvSrvUavDescriptorHeap);
}

void GCRender::CreateRtvAndDsvDescriptorHeaps()
{
	m_pGCRenderResources->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, m_pGCRenderResources->SwapChainBufferCount + 2,false, &m_pGCRenderResources->m_pRtvHeap);
	m_pGCRenderResources->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV,2,false, &m_pGCRenderResources->m_pDsvHeap);
}

void GCRender::CreatePostProcessingResources() {
	m_pGCRenderResources->m_pPostProcessingRtv= m_pGCRenderResources->CreateRTT(true);
	

	// Create A post Processing Shader
	m_postProcessingShader = new GCShader();
	m_brightShader = new GCComputeShader();
	m_blurShader = new GCComputeShader();
	m_combineShader = new GCComputeShader();
	m_testShader = new GCComputeShader();
	std::string shaderFilePath = "../../../src/RenderApp/Shaders/PostProcessing.hlsl";
	std::string csoDestinationPath = "../../../src/RenderApp/CsoCompiled/PostProcessing";
	std::string shaderFilePath1 = "../../../src/RenderApp/Shaders/BrightExtract.hlsl";
	std::string csoDestinationPath1 = "../../../src/RenderApp/CsoCompiled/BrightExtract";
	std::string shaderFilePath2 = "../../../src/RenderApp/Shaders/Blur.hlsl";
	std::string csoDestinationPath2 = "../../../src/RenderApp/CsoCompiled/Blur";
	std::string shaderFilePath3 = "../../../src/RenderApp/Shaders/Combine.hlsl";
	std::string csoDestinationPath3 = "../../../src/RenderApp/CsoCompiled/Combine";
	std::string shaderFilePath4 = "../../../src/RenderApp/Shaders/test.hlsl";
	std::string csoDestinationPath4 = "../../../src/RenderApp/CsoCompiled/test";
	int flags = 0;
	SET_FLAG(flags, VERTEX_POSITION);
	SET_FLAG(flags, VERTEX_UV);

	m_postProcessingShader->Initialize(this, shaderFilePath, csoDestinationPath, flags);
	m_postProcessingShader->Load();

	m_brightShader->Initialize(this, shaderFilePath1, csoDestinationPath1, flags);
	//m_brightShader->Load();

	m_blurShader->Initialize(this, shaderFilePath2, csoDestinationPath2, flags);
	//m_blurShader->Load();

	m_combineShader->Initialize(this, shaderFilePath3, csoDestinationPath3, flags);
	//m_combineShader->Load();

	m_testShader->Initialize(this, shaderFilePath4, csoDestinationPath4, flags);
	m_testShader->Load();

	// Create RTV For Object Buffer Id For pass Mesh id to pixel, to apply them on a texture 
	//m_pGCRenderResources->m_ObjectIdBufferRtv = m_pGCRenderResources->CreateRTT(false);

	//// Create Object buffer Id Shader 
	int flags2 = 0;
	SET_FLAG(flags2, VERTEX_POSITION);

	m_objectBufferIdShader = new GCShader();
	std::string shaderFilePath5 = "../../../src/RenderApp/Shaders/ObjectBufferId.hlsl";
	std::string csoDestinationPath5 = "../../../src/RenderApp/CsoCompiled/ObjectBufferId";

	m_objectBufferIdShader->Initialize(this, shaderFilePath5, csoDestinationPath5, flags2);
	m_objectBufferIdShader->Load();
	
}

void GCRender::OnResize() 
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
	m_pGCRenderResources->CreateDepthStencilBufferAndView(m_pGCRenderResources->m_DepthStencilFormat, D3D12_RESOURCE_STATE_COMMON);
	auto depthStencilView = m_pGCRenderResources->m_lDepthStencilView.front();
	CD3DX12_RESOURCE_BARRIER CommonToDepthWrite(CD3DX12_RESOURCE_BARRIER::Transition(depthStencilView->resource, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));
	m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &CommonToDepthWrite);

	CloseCommandList();
	ExecuteCommandList();
	FlushCommandQueue();

	UpdateViewport();
}

void GCRender::ReleasePreviousResources() 
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

void GCRender::ResizeSwapChain() 
{
	m_pGCRenderResources->m_SwapChain->ResizeBuffers(
		m_pGCRenderResources->SwapChainBufferCount,
		m_pGCRenderResources->m_pWindow->GetClientWidth(), m_pGCRenderResources->m_pWindow->GetClientHeight(),
		m_pGCRenderResources->m_BackBufferFormat,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	);

	m_pGCRenderResources->m_CurrBackBuffer = 0;
}

void GCRender::CreateRenderTargetViews() 
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_pGCRenderResources->m_pRtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < m_pGCRenderResources->SwapChainBufferCount; i++)
	{
		m_pGCRenderResources->m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_pGCRenderResources->m_SwapChainBuffer[i]));
		m_pGCRenderResources->m_d3dDevice->CreateRenderTargetView(m_pGCRenderResources->m_SwapChainBuffer[i], nullptr, rtvHeapHandle);
		rtvHeapHandle.Offset(1, m_pGCRenderResources->m_rtvDescriptorSize);
		m_pGCRenderResources->CreateSrvWithTexture(m_pGCRenderResources->m_SwapChainBuffer[i], DXGI_FORMAT_R8G8B8A8_UNORM);
	}
}

void GCRender::UpdateViewport() 
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

bool GCRender::FlushCommandQueue()
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


bool GCRender::PrepareDraw() 
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

	//CD3DX12_RESOURCE_BARRIER CommonToRT(CD3DX12_RESOURCE_BARRIER::Transition(m_pGCRenderResources->m_ObjectIdBufferRtv, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_RENDER_TARGET));
	//m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &CommonToRT);


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






bool GCRender::DrawObject(GCMesh* pMesh, GCMaterial* pMaterial, bool alpha)
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
void GCRender::PerformPostProcessing3()
{
	CD3DX12_RESOURCE_BARRIER barrierToUAV = CD3DX12_RESOURCE_BARRIER::Transition(
		m_pGCRenderResources->m_pPostProcessingRtv,
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS
	);
	m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &barrierToUAV);

	// Transition CurrentBackBuffer to PIXEL_SHADER_RESOURCE for read
	CD3DX12_RESOURCE_BARRIER barrierToShaderResource = CD3DX12_RESOURCE_BARRIER::Transition(
		m_pGCRenderResources->CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
	);
	m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &barrierToShaderResource);

	// Set up compute shader
	m_pGCRenderResources->m_CommandList->SetPipelineState(m_testShader->GetPso());
	m_pGCRenderResources->m_CommandList->SetComputeRootSignature(m_testShader->GetRootSign());

	// Set descriptor tables for compute shader
	auto srvGpuHandleIter = m_pGCRenderResources->m_lShaderResourceView.begin();
	std::advance(srvGpuHandleIter, m_pGCRenderResources->GetCurrBackBuffer());
	CD3DX12_GPU_DESCRIPTOR_HANDLE srvGpuHandleInput = *srvGpuHandleIter;
	m_pGCRenderResources->m_CommandList->SetComputeRootDescriptorTable(0, srvGpuHandleInput); // Input texture

	// Create a UAV descriptor handle for the output texture (PostProcessingRtv)
	CD3DX12_GPU_DESCRIPTOR_HANDLE uavGpuHandleOutput(
		m_pGCRenderResources->m_pCbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
		m_pGCRenderResources->SwapChainBufferCount, // Assuming UAVs start right after SRVs
		m_pGCRenderResources->m_cbvSrvUavDescriptorSize
	);
	m_pGCRenderResources->m_CommandList->SetComputeRootDescriptorTable(1, uavGpuHandleOutput); // Output texture

	// Dispatch compute shader
	m_pGCRenderResources->m_CommandList->Dispatch(
		(m_pGCRenderResources->GetRenderWidth() + 15) / 16,
		(m_pGCRenderResources->GetRenderHeight() + 15) / 16,
		1
	);

	// Transition PostProcessingRtv to COPY_SOURCE for read
	CD3DX12_RESOURCE_BARRIER barrierToUAV2 = CD3DX12_RESOURCE_BARRIER::Transition(
		m_pGCRenderResources->m_pPostProcessingRtv,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_COPY_SOURCE
	);
	m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &barrierToUAV2);

	// Transition CurrentBackBuffer to COPY_DEST for write
	CD3DX12_RESOURCE_BARRIER barrierToCopyFromBackBuffer2 = CD3DX12_RESOURCE_BARRIER::Transition(
		m_pGCRenderResources->CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_COPY_DEST
	);
	m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &barrierToCopyFromBackBuffer2);

	// Copy m_pPostProcessingRtv to CurrentBackBuffer
	m_pGCRenderResources->m_CommandList->CopyResource(
		m_pGCRenderResources->CurrentBackBuffer(),
		m_pGCRenderResources->m_pPostProcessingRtv
	);
}
// #TODO RESTRUCTURE RENDER -> GCRenderContext GCRenderResources -> Compute Shader
void GCRender::PerformPostProcessing2(){
	// Transition pour le rendu sur m_pPostProcessingRtv
	CD3DX12_RESOURCE_BARRIER barrierToRT = CD3DX12_RESOURCE_BARRIER::Transition(m_pGCRenderResources->m_pPostProcessingRtv, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &barrierToRT);
	CD3DX12_CPU_DESCRIPTOR_HANDLE test2 = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_pGCRenderResources->m_pRtvHeap->GetCPUDescriptorHandleForHeapStart(), 3, m_pGCRenderResources->m_rtvDescriptorSize);
	// Set Render target
	D3D12_CPU_DESCRIPTOR_HANDLE dsv = m_pGCRenderResources->GetDepthStencilViewAddress();
	m_pGCRenderResources->m_CommandList->OMSetRenderTargets(1, &test2, FALSE, &dsv);
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
void GCRender::PerformPostProcessing()
{

	// Transition m_pPostProcessingRtv to RENDER_TARGET state
	CD3DX12_RESOURCE_BARRIER barrierToRT = CD3DX12_RESOURCE_BARRIER::Transition(
		m_pGCRenderResources->m_pPostProcessingRtv,
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_RENDER_TARGET
	);
	m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &barrierToRT);

	// Transition CurrentBackBuffer to PIXEL_SHADER_RESOURCE state
	CD3DX12_RESOURCE_BARRIER barrierToShaderResource = CD3DX12_RESOURCE_BARRIER::Transition(
		m_pGCRenderResources->CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
	);
	m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &barrierToShaderResource);

	// Bright extraction using compute shader
	m_pGCRenderResources->m_CommandList->SetPipelineState(m_brightShader->GetPso());
	m_pGCRenderResources->m_CommandList->SetComputeRootSignature(m_brightShader->GetRootSign());

	// Set constants if needed (example)
	UINT blurRadius = 5;
	m_pGCRenderResources->m_CommandList->SetComputeRoot32BitConstants(0, 1, &blurRadius, 0);

	// Set descriptor tables for bright shader
	CD3DX12_GPU_DESCRIPTOR_HANDLE srvGpuHandleBright(
		m_pGCRenderResources->m_pCbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
		90,
		m_pGCRenderResources->m_cbvSrvUavDescriptorSize
	);
	m_pGCRenderResources->m_CommandList->SetComputeRootDescriptorTable(1, srvGpuHandleBright);

	CD3DX12_GPU_DESCRIPTOR_HANDLE uavGpuHandleBright(
		m_pGCRenderResources->m_pCbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
		150,
		m_pGCRenderResources->m_cbvSrvUavDescriptorSize
	);
	m_pGCRenderResources->m_CommandList->SetComputeRootDescriptorTable(2, uavGpuHandleBright);

	// Dispatch bright extraction compute shader
	m_pGCRenderResources->m_CommandList->Dispatch(
		(m_pGCRenderResources->GetRenderWidth() + 15) / 16,
		(m_pGCRenderResources->GetRenderHeight() + 15) / 16,
		1
	);

	// Transition for blur shader
	CD3DX12_RESOURCE_BARRIER barrierToUAVBlur = CD3DX12_RESOURCE_BARRIER::Transition(
		m_pGCRenderResources->m_BrightImageBuffer,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE
	);
	m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &barrierToUAVBlur);

	// Blur bright areas using compute shader
	m_pGCRenderResources->m_CommandList->SetPipelineState(m_blurShader->GetPso());
	m_pGCRenderResources->m_CommandList->SetComputeRootSignature(m_blurShader->GetRootSign());

	// Set descriptor tables for blur shader
	CD3DX12_GPU_DESCRIPTOR_HANDLE srvGpuHandleBlur(
		m_pGCRenderResources->m_pCbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
		150,
		m_pGCRenderResources->m_cbvSrvUavDescriptorSize
	);
	m_pGCRenderResources->m_CommandList->SetComputeRootDescriptorTable(1, srvGpuHandleBlur);

	CD3DX12_GPU_DESCRIPTOR_HANDLE uavGpuHandleBlur(
		m_pGCRenderResources->m_pCbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
		200,
		m_pGCRenderResources->m_cbvSrvUavDescriptorSize
	);
	m_pGCRenderResources->m_CommandList->SetComputeRootDescriptorTable(2, uavGpuHandleBlur);

	// Dispatch blur compute shader
	m_pGCRenderResources->m_CommandList->Dispatch(
		(m_pGCRenderResources->GetRenderWidth() + 15) / 16,
		(m_pGCRenderResources->GetRenderHeight() + 15) / 16,
		1
	);

	// Transition for combine shader
	CD3DX12_RESOURCE_BARRIER barrierToUAVCombine = CD3DX12_RESOURCE_BARRIER::Transition(
		m_pGCRenderResources->m_BlurImageBuffer,
		D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
	);
	m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &barrierToUAVCombine);

	// Combine blurred bright areas with the original image using compute shader
	m_pGCRenderResources->m_CommandList->SetPipelineState(m_combineShader->GetPso());
	m_pGCRenderResources->m_CommandList->SetComputeRootSignature(m_combineShader->GetRootSign());

	// Set descriptor tables for combine shader
	CD3DX12_GPU_DESCRIPTOR_HANDLE srvGpuHandleCombine(
		m_pGCRenderResources->m_pCbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
		90,
		m_pGCRenderResources->m_cbvSrvUavDescriptorSize
	);
	m_pGCRenderResources->m_CommandList->SetComputeRootDescriptorTable(1, srvGpuHandleCombine); // OriginalImage

	CD3DX12_GPU_DESCRIPTOR_HANDLE srvGpuHandleBlurCombine(
		m_pGCRenderResources->m_pCbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
		200,
		m_pGCRenderResources->m_cbvSrvUavDescriptorSize
	);
	m_pGCRenderResources->m_CommandList->SetComputeRootDescriptorTable(2, srvGpuHandleBlurCombine); // BlurredImage

	CD3DX12_GPU_DESCRIPTOR_HANDLE uavGpuHandleOutput(
		m_pGCRenderResources->m_pCbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
		250,
		m_pGCRenderResources->m_cbvSrvUavDescriptorSize
	);
	m_pGCRenderResources->m_CommandList->SetComputeRootDescriptorTable(3, uavGpuHandleOutput); // OutputImage

	// Dispatch combine compute shader
	m_pGCRenderResources->m_CommandList->Dispatch(
		(m_pGCRenderResources->GetRenderWidth() + 15) / 16,
		(m_pGCRenderResources->GetRenderHeight() + 15) / 16,
		1
	);

	// Barrier: Transition for the final copy
	CD3DX12_RESOURCE_BARRIER barrierToCopySource = CD3DX12_RESOURCE_BARRIER::Transition(
		m_pGCRenderResources->m_pPostProcessingRtv,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_COPY_SOURCE
	);
	m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &barrierToCopySource);

	CD3DX12_RESOURCE_BARRIER barrierToCopyDest = CD3DX12_RESOURCE_BARRIER::Transition(
		m_pGCRenderResources->CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_COPY_DEST
	);
	m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &barrierToCopyDest);

	// Copy m_pPostProcessingRtv to the final render target
	m_pGCRenderResources->m_CommandList->CopyResource(
		m_pGCRenderResources->CurrentBackBuffer(),
		m_pGCRenderResources->m_pPostProcessingRtv
	);

}

	//CD3DX12_RESOURCE_BARRIER ResBar2(CD3DX12_RESOURCE_BARRIER::Transition(m_pGCRenderResources->CurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
	//m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &ResBar2);

//Always needs to be called right after drawing!!!
bool GCRender::CompleteDraw()
{
	PerformPostProcessing3();

	CD3DX12_RESOURCE_BARRIER ResBar2 = CD3DX12_RESOURCE_BARRIER::Transition(
		m_pGCRenderResources->CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_PRESENT
	);
	m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &ResBar2);

	CD3DX12_RESOURCE_BARRIER ResBar3 = CD3DX12_RESOURCE_BARRIER::Transition(
		m_pGCRenderResources->m_pPostProcessingRtv,
		D3D12_RESOURCE_STATE_COPY_SOURCE,
		D3D12_RESOURCE_STATE_PRESENT
	);
	m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &ResBar3);

	// Close the command list
	HRESULT hr = m_pGCRenderResources->m_CommandList->Close();
	if (!CHECK_HRESULT(hr, "Failed to close command list"))
		return false;

	// Execute the command list
	ID3D12CommandList* cmdsLists[] = { m_pGCRenderResources->m_CommandList };
	m_pGCRenderResources->m_CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Present the swap chain
	hr = m_pGCRenderResources->m_SwapChain->Present(0, 0);
	if (!CHECK_HRESULT(hr, "Failed to present swap chain")) {
		return false;
	}

	// Update the current back buffer index
	m_pGCRenderResources->m_CurrBackBuffer = (m_pGCRenderResources->m_CurrBackBuffer + 1) % m_pGCRenderResources->SwapChainBufferCount;

	// Flush the command queue
	FlushCommandQueue();

	return true;
}

void GCRender::LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format) 	
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

ID3D12Resource* GCRenderResources::CreateRTT(bool isStateRenderTarget)
{
	if(isStateRenderTarget)
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_pCbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), 2 + testCount, m_cbvSrvUavDescriptorSize);

		// Define the texture description
		D3D12_RESOURCE_DESC textureDesc = {};
		textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		textureDesc.Width = GetRenderWidth();
		textureDesc.Height = GetRenderHeight();
		textureDesc.DepthOrArraySize = 1;
		textureDesc.MipLevels = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

		// Define the clear value
		D3D12_CLEAR_VALUE clearValue = {};
		clearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		clearValue.Color[0] = 0.0f;
		clearValue.Color[1] = 0.0f;
		clearValue.Color[2] = 0.0f;
		clearValue.Color[3] = 1.0f;

		// Create the render target texture
		CD3DX12_HEAP_PROPERTIES heapProperties(D3D12_HEAP_TYPE_DEFAULT);
		ID3D12Resource* renderTargetTexture = nullptr;
		HRESULT hr = m_d3dDevice->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&textureDesc,
			D3D12_RESOURCE_STATE_COMMON, // Initial state can be COMMON
			nullptr,
			IID_PPV_ARGS(&renderTargetTexture)
		);
		if (FAILED(hr)) {
			// Handle error
			MessageBoxA(nullptr, "Failed to create render target texture", "Error", MB_OK);
			return nullptr;
		}

		// Create the shader resource view (SRV)
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		m_d3dDevice->CreateShaderResourceView(renderTargetTexture,
			&srvDesc, rtvHeapHandle);
		rtvHeapHandle.Offset(1, m_rtvDescriptorSize); // Move to the next descriptor

		// Create the unordered access view (UAV)
		D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
		uavDesc.Texture2D.MipSlice = 0;
		m_d3dDevice->CreateUnorderedAccessView(renderTargetTexture,
			nullptr, &uavDesc, rtvHeapHandle);
		rtvHeapHandle.Offset(1, m_cbvSrvUavDescriptorSize); // Move to the next descriptor

		// Store the created render target texture
		m_renderTargets.push_back(renderTargetTexture);

		testCount += 1;

		return renderTargetTexture;
	}
	else {
		ID3D12Resource* renderTargetTexture = nullptr;
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_pRtvHeap->GetCPUDescriptorHandleForHeapStart(), 2 + testCount, m_rtvDescriptorSize);
		D3D12_RESOURCE_DESC textureDesc2 = {};
		textureDesc2.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		textureDesc2.Width = m_pWindow->GetClientWidth();
		textureDesc2.Height = m_pWindow->GetClientHeight();
		textureDesc2.DepthOrArraySize = 1;
		textureDesc2.MipLevels = 1;
		textureDesc2.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		textureDesc2.SampleDesc.Count = 1;
		textureDesc2.SampleDesc.Quality = 0;
		textureDesc2.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		textureDesc2.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

		CD3DX12_HEAP_PROPERTIES test2 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		HRESULT hr = m_d3dDevice->CreateCommittedResource(
			&test2,
			D3D12_HEAP_FLAG_NONE,
			&textureDesc2,
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			IID_PPV_ARGS(&renderTargetTexture));
		if (FAILED(hr)) {
			// Handle error
			MessageBoxA(nullptr, "Failed to create render target texture", "Error", MB_OK);
		}
		m_d3dDevice->CreateRenderTargetView(renderTargetTexture, nullptr, rtvHeapHandle);
		rtvHeapHandle.Offset(1, m_rtvDescriptorSize);
		m_renderTargets.push_back(renderTargetTexture);
		testCount += 1;
		return renderTargetTexture;
	}
}

void GCRenderResources::DeleteRenderTarget(ID3D12Resource* pRenderTarget) {
	auto it = std::find(m_renderTargets.begin(), m_renderTargets.end(), pRenderTarget);
	if (it != m_renderTargets.end()) {
		if (pRenderTarget) {
			pRenderTarget->Release();
			pRenderTarget = nullptr;
		}
		m_renderTargets.erase(it);
	}
}

CD3DX12_GPU_DESCRIPTOR_HANDLE GCRenderResources::CreateSrvWithTexture(ID3D12Resource* textureResource, DXGI_FORMAT format)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	CD3DX12_CPU_DESCRIPTOR_HANDLE srvCpuHandle(m_pCbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	srvCpuHandle.Offset(m_srvOffsetCount, m_cbvSrvUavDescriptorSize);

	CD3DX12_GPU_DESCRIPTOR_HANDLE srvGpuHandle(m_pCbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
	srvGpuHandle.Offset(m_srvOffsetCount, m_cbvSrvUavDescriptorSize);

	m_d3dDevice->CreateShaderResourceView(textureResource, &srvDesc, srvCpuHandle);


	GCGraphicsLogger& profiler = GCGraphicsLogger::GetInstance();
	profiler.LogWarning("Offset srv count : " + std::to_string(m_srvOffsetCount));

	//m_lShaderResourceView.push_back(srvGpuHandle);
	m_lShaderResourceView.push_back(srvGpuHandle);

	m_srvOffsetCount++;

	return srvGpuHandle;
}

void GCRenderResources::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible, ID3D12DescriptorHeap** pDescriptorHeap)
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = type;
	heapDesc.NumDescriptors = numDescriptors;
	heapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	heapDesc.NodeMask = 0;

	HRESULT hr = m_d3dDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(pDescriptorHeap));
}

GC_DESCRIPTOR_RESOURCE* GCRenderResources::CreateRTVTexture(DXGI_FORMAT format, D3D12_RESOURCE_FLAGS resourceFlags, D3D12_CLEAR_VALUE* clearValue)
{
	//Handle Cpu
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_pRtvHeap->GetCPUDescriptorHandleForHeapStart(), m_rtvOffsetCount, m_rtvDescriptorSize);
	rtvHeapHandle.Offset(m_rtvOffsetCount, m_rtvDescriptorSize);

	D3D12_RESOURCE_DESC textureDesc = {};
	textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureDesc.Width = m_renderWidth;
	textureDesc.Height = m_renderHeight;
	textureDesc.DepthOrArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.Format = format;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;


	D3D12_CLEAR_VALUE defaultClearValue = {};
	defaultClearValue.Format = format;
	defaultClearValue.Color[0] = 0.0f;
	defaultClearValue.Color[1] = 0.0f;
	defaultClearValue.Color[2] = 0.0f;
	defaultClearValue.Color[3] = 1.0f;

	// Use the provided clear value or the default one
	D3D12_CLEAR_VALUE* actualClearValue = clearValue ? clearValue : &defaultClearValue;

	CD3DX12_HEAP_PROPERTIES heapProperties(D3D12_HEAP_TYPE_DEFAULT);
	ID3D12Resource* renderTargetTexture = nullptr;
	HRESULT hr = m_d3dDevice->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&textureDesc,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		actualClearValue,
		IID_PPV_ARGS(&renderTargetTexture)
	);

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = format;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	rtvDesc.Texture2D.PlaneSlice = 0;

	m_d3dDevice->CreateRenderTargetView(renderTargetTexture, &rtvDesc, rtvHeapHandle);

	// Manager Rtv
	GC_DESCRIPTOR_RESOURCE* descriptorResource = new GC_DESCRIPTOR_RESOURCE();
	descriptorResource->resource = renderTargetTexture;
	descriptorResource->cpuHandle = rtvHeapHandle;
	m_lRenderTargets.push_back(descriptorResource);

	m_rtvOffsetCount++;

	return descriptorResource;
}

GC_DESCRIPTOR_RESOURCE* GCRenderResources::CreateDepthStencilBufferAndView(DXGI_FORMAT depthStencilFormat, D3D12_RESOURCE_STATES resourceFlags)
{
	HRESULT hr;

	D3D12_RESOURCE_DESC depthStencilDesc = {};
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = m_renderWidth;
	depthStencilDesc.Height = m_renderHeight;
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.Format = depthStencilFormat;
	depthStencilDesc.SampleDesc.Count = m_4xMsaaState ? 4 : 1;
	depthStencilDesc.SampleDesc.Quality = m_4xMsaaState ? (m_4xMsaaQuality - 1) : 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear = {};
	optClear.Format = depthStencilFormat;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;

	CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_DEFAULT);
	ID3D12Resource* depthStencilBuffer = nullptr;
	hr = m_d3dDevice->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&depthStencilDesc,
		resourceFlags,
		&optClear,
		IID_PPV_ARGS(&depthStencilBuffer)
	);
	CHECK_HRESULT(hr, "Dsv Bad Init");

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = depthStencilFormat;
	dsvDesc.Texture2D.MipSlice = 0;

	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(m_pDsvHeap->GetCPUDescriptorHandleForHeapStart());
	dsvHandle.Offset(m_dsvOffsetCount, m_dsvDescriptorSize);

	m_d3dDevice->CreateDepthStencilView(depthStencilBuffer, &dsvDesc, dsvHandle);

	GC_DESCRIPTOR_RESOURCE* dsv = new GC_DESCRIPTOR_RESOURCE{ depthStencilBuffer, dsvHandle };

	m_dsvOffsetCount++;
	m_lDepthStencilView.push_back(dsv);

	return dsv;
}