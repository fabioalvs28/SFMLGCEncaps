#include "pch.h"

bool GCRenderContext::Initialize(Window* pWindow, int renderWidth, int renderHeight, GCGraphics* pGraphics)
{
	if (!CHECK_POINTERSNULL("Graphics Initialized with window sucessfully", "Can't initialize Graphics, Window is empty", pWindow))
		return false;

	m_pGCRenderResources = new GCRenderResources();
	m_pGCRenderResources->m_pGraphics = pGraphics;
	m_pGCRenderResources->m_renderWidth = renderWidth;
	m_pGCRenderResources->m_renderHeight = renderHeight;
	m_pGCRenderResources->m_pWindow = pWindow;


	InitDX12RenderPipeline();


	return true;
}

bool GCRenderContext::ResetCommandList() 
{
	HRESULT hr = m_pGCRenderResources->m_CommandList->Reset(m_pGCRenderResources->m_DirectCmdListAlloc, nullptr);
	if (CHECK_HRESULT(hr, "Failed to Reset command list") == false) {
		return false;
	}
	return true;
}

void GCRenderContext::ExecuteCommandList() 
{
	ID3D12CommandList* cmdsLists[] = { m_pGCRenderResources->m_CommandList };
	m_pGCRenderResources->m_CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
}

bool GCRenderContext::CloseCommandList() 
{
	HRESULT hr = m_pGCRenderResources->m_CommandList->Close();
	if (CHECK_HRESULT(hr, "Failed to Close command list") == false) {
		return false;
	}
	return true;
}

bool GCRenderContext::InitDX12RenderPipeline()
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

		D3D12CreateDevice(pWarpAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pGCRenderResources->m_d3dDevice));
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
	
	//Create RTV/DSV Descriptor Heaps
	m_pGCRenderResources->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 1000, false, &m_pGCRenderResources->m_pRtvHeap);
	m_pGCRenderResources->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 2, false, &m_pGCRenderResources->m_pDsvHeap);
	//Create CBV/SRV/UAV Descriptor Heaps
	m_pGCRenderResources->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 2000, true, &m_pGCRenderResources->m_pCbvSrvUavDescriptorHeap);

	//*

	CreatePostProcessingResources();

	m_pGCRenderResources->m_canResize = true;

	OnResize();

	return true;
}

void GCRenderContext::CreateCommandObjects()
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	m_pGCRenderResources->m_d3dDevice->CreateCommandQueue(
		&queueDesc, 
		IID_PPV_ARGS(&m_pGCRenderResources->m_CommandQueue)
	);

	m_pGCRenderResources->m_d3dDevice->CreateCommandAllocator( 
		D3D12_COMMAND_LIST_TYPE_DIRECT, 
		IID_PPV_ARGS(&m_pGCRenderResources->m_DirectCmdListAlloc)
	);

	m_pGCRenderResources->m_d3dDevice->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_pGCRenderResources->m_DirectCmdListAlloc, // Associated command allocator
		nullptr, // Initial PipelineStateObject
		IID_PPV_ARGS(&m_pGCRenderResources->m_CommandList)
	);

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
	sd.BufferDesc.Width = m_pGCRenderResources->m_renderWidth;
	sd.BufferDesc.Height = m_pGCRenderResources->m_renderHeight;
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

	// #NOTE -> Swap chain uses queue to perform flush.
	m_pGCRenderResources->m_dxgiFactory->CreateSwapChain(m_pGCRenderResources->m_CommandQueue, &sd, &m_pGCRenderResources->m_SwapChain);
}

void GCRenderContext::CreatePostProcessingResources() {
	{
		GC_DESCRIPTOR_RESOURCE* rtv = m_pGCRenderResources->CreateRTVTexture(m_pGCRenderResources->GetBackBufferFormat(), D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
		m_pGCRenderResources->m_pPostProcessingRtv = rtv->resource;
		m_pGCRenderResources->m_pPostProcessingRtvAddress = rtv->cpuHandle;

		//Create Post Processing Shader
		m_postProcessingShader = new GCShader();
		std::string shaderFilePath = "../../../src/Render/Shaders/PostProcessing.hlsl";
		std::string csoDestinationPath = "../../../src/Render/CsoCompiled/PostProcessing";
		int flags = 0;
		SET_FLAG(flags, VERTEX_POSITION);
		SET_FLAG(flags, VERTEX_UV);
		m_postProcessingShader->Initialize(this, shaderFilePath, csoDestinationPath, flags);
		m_postProcessingShader->Load();
	}
	
	{
		//Create RTV For Object Buffer Id For pass Mesh id to pixel, to apply them on a texture 
		GC_DESCRIPTOR_RESOURCE* rtv = m_pGCRenderResources->CreateRTVTexture(m_pGCRenderResources->GetBackBufferFormat(), D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
		m_pGCRenderResources->m_ObjectIdBufferRtv = rtv->resource;
		m_pGCRenderResources->m_ObjectIdBufferRtvAddress = rtv->cpuHandle;

		//Create Object buffer Id Shader 
		int flags2 = 0;
		SET_FLAG(flags2, VERTEX_POSITION);
		m_objectBufferIdShader = new GCShader();
		std::string shaderFilePath2 = "../../../src/Render/Shaders/ObjectBufferId.hlsl";
		std::string csoDestinationPath2 = "../../../src/Render/CsoCompiled/ObjectBufferId";
		m_objectBufferIdShader->Initialize(this, shaderFilePath2, csoDestinationPath2, flags2);
		m_objectBufferIdShader->SetRenderTargetFormats(m_pGCRenderResources->GetBackBufferFormat(), 1);
		m_objectBufferIdShader->Load();
	}
	

}

void GCRenderContext::OnResize() 
{
	// #TODO NeedLess ?
	
	//m_pGCRenderResources->m_canResize = true;
	//if (m_pGCRenderResources->m_canResize == false)
	//	return;

	CHECK_POINTERSNULL("Device not null", "Device is null", m_pGCRenderResources->m_d3dDevice);
	CHECK_POINTERSNULL("SwapChain not null", "SwapChain is null", m_pGCRenderResources->m_d3dDevice);
	CHECK_POINTERSNULL("Command Allocator not null", "Command Allocator is null", m_pGCRenderResources->m_d3dDevice);

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
		m_pGCRenderResources->m_renderWidth, m_pGCRenderResources->m_renderHeight,
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

}

void GCRenderContext::CreateDepthStencilBufferAndView()
{
	auto dsv1 = m_pGCRenderResources->CreateDepthStencilBufferAndView(DXGI_FORMAT_D24_UNORM_S8_UINT, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	m_pGCRenderResources->m_DepthStencilBuffer = dsv1->resource;
	m_pGCRenderResources->m_DepthStencilBufferAddress = dsv1->cpuHandle;

	auto dsv2 = m_pGCRenderResources->CreateDepthStencilBufferAndView(DXGI_FORMAT_D24_UNORM_S8_UINT, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	m_pGCRenderResources->m_ObjectIdDepthStencilBuffer = dsv2->resource;
	m_pGCRenderResources->m_ObjectIdDepthStencilBufferAddress = dsv2->cpuHandle;

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
	//Always needs to be called right before drawing!!!

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


	m_pGCRenderResources->m_CommandList->ClearRenderTargetView(m_pGCRenderResources->CurrentBackBufferViewAddress(), DirectX::Colors::LightBlue, 1, &m_pGCRenderResources->m_ScissorRect);
	m_pGCRenderResources->m_CommandList->ClearDepthStencilView(m_pGCRenderResources->GetDepthStencilViewAddress(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	m_pGCRenderResources->m_CommandList->ClearRenderTargetView(m_pGCRenderResources->m_ObjectIdBufferRtvAddress, DirectX::Colors::LightBlue, 1, &m_pGCRenderResources->m_ScissorRect);
	m_pGCRenderResources->m_CommandList->ClearDepthStencilView(m_pGCRenderResources->m_ObjectIdDepthStencilBufferAddress, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);


	D3D12_CPU_DESCRIPTOR_HANDLE dsv = m_pGCRenderResources->GetDepthStencilViewAddress();
	D3D12_CPU_DESCRIPTOR_HANDLE rtv = m_pGCRenderResources->CurrentBackBufferViewAddress();
	m_pGCRenderResources->m_CommandList->OMSetRenderTargets(1, &rtv, TRUE, &dsv);

	ID3D12DescriptorHeap* descriptorHeaps[] = { m_pGCRenderResources->m_pCbvSrvUavDescriptorHeap };
	m_pGCRenderResources->m_CommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	return true;
}

void GCRenderContext::PerformPixelIdMapping(GCMesh* pMesh, bool alpha) {

	m_pGCRenderResources->m_CommandList->SetPipelineState(m_objectBufferIdShader->GetPso(alpha));
	m_pGCRenderResources->m_CommandList->SetGraphicsRootSignature(m_objectBufferIdShader->GetRootSign());

	D3D12_CPU_DESCRIPTOR_HANDLE dsv = m_pGCRenderResources->GetDepthStencilViewAddress();

	//Read State 
	//CD3DX12_RESOURCE_BARRIER CommonToRead(CD3DX12_RESOURCE_BARRIER::Transition(m_pGCRenderResources->m_DepthStencilBuffer, D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_DEPTH_READ));
	//m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &CommonToRead);

	m_pGCRenderResources->m_CommandList->OMSetRenderTargets(1, &m_pGCRenderResources->m_ObjectIdBufferRtvAddress, TRUE, &m_pGCRenderResources->m_ObjectIdDepthStencilBufferAddress);

	m_pGCRenderResources->m_CommandList->DrawIndexedInstanced(pMesh->GetBufferGeometryData()->IndexCount, 1, 0, 0, 0);

	//Write State
	//CD3DX12_RESOURCE_BARRIER ReadToWrite(CD3DX12_RESOURCE_BARRIER::Transition(m_pGCRenderResources->m_DepthStencilBuffer, D3D12_RESOURCE_STATE_DEPTH_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE));
	//m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &ReadToWrite);

	// Set Again Old Render target

	D3D12_CPU_DESCRIPTOR_HANDLE rtv = m_pGCRenderResources->CurrentBackBufferViewAddress();
	m_pGCRenderResources->m_CommandList->OMSetRenderTargets(1, &rtv, true, &dsv);
}

bool GCRenderContext::DrawObject(GCMesh* pMesh, GCMaterial* pMaterial, bool alpha)
{
	if (pMaterial == nullptr || pMaterial->GetShader() == nullptr || pMesh == nullptr)
		return false;
	if (!COMPARE_SHADER_MESH_FLAGS(pMaterial, pMesh))
		return false;

	//Basic Draw
	{
		m_pGCRenderResources->m_CommandList->SetPipelineState(pMaterial->GetShader()->GetPso(true));
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
	if (m_isPixelIDMappingActivated)
	{
		PerformPixelIdMapping(pMesh, alpha);
	}


	// ******************

	return true;
}

bool GCRenderContext::CompleteDraw()
{
	if (m_isBasicPostProcessingActivated) PerformPostProcessing();
	

	//CD3DX12_RESOURCE_BARRIER RtToPresent = CD3DX12_RESOURCE_BARRIER::Transition(m_pGCRenderResources->CurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	//m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &RtToPresent);

	HRESULT hr = m_pGCRenderResources->m_CommandList->Close();
	if (CHECK_HRESULT(hr, "Failed to close command list") == false) return false;
	ID3D12CommandList* cmdsLists[] = { m_pGCRenderResources->m_CommandList };
	m_pGCRenderResources->m_CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);


	hr = m_pGCRenderResources->m_SwapChain->Present(0, 0);
	if (CHECK_HRESULT(hr, "Failed to present swap chain") == false) return false;
	// Swap front - back buffer index
	m_pGCRenderResources->m_CurrBackBuffer = (m_pGCRenderResources->m_CurrBackBuffer + 1) % m_pGCRenderResources->SwapChainBufferCount;


	//Clear frame counter for resource using - Release short time resource
	// #WARNING It will replace resource on these offset already allocated
	m_pGCRenderResources->m_srvOffsetCount = 300;

	// Flush the command queue
	if (FlushCommandQueue() == false) return false;

	return true;
}

void GCRenderContext::PerformPostProcessing()
{
	// Intermediate Rtv - Common To RT
	//CD3DX12_RESOURCE_BARRIER barrierToRT = CD3DX12_RESOURCE_BARRIER::Transition(m_pGCRenderResources->m_pPostProcessingRtv, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_RENDER_TARGET);
	//m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &barrierToRT);

	// Set Render target
	//D3D12_CPU_DESCRIPTOR_HANDLE dsv = m_pGCRenderResources->GetDepthStencilViewAddress();
	m_pGCRenderResources->m_CommandList->OMSetRenderTargets(1, &m_pGCRenderResources->m_pPostProcessingRtvAddress, FALSE, nullptr);

	// Root Sign / Pso
	m_pGCRenderResources->m_CommandList->SetPipelineState(m_postProcessingShader->GetPso(true));
	m_pGCRenderResources->m_CommandList->SetGraphicsRootSignature(m_postProcessingShader->GetRootSign());

	// CurrentBackBuffer() - RT To Pixel Shader Entry
	CD3DX12_RESOURCE_BARRIER RtToPixelShader = CD3DX12_RESOURCE_BARRIER::Transition(m_pGCRenderResources->CurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &RtToPixelShader);

	//Post process texture linking to shader
	CD3DX12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = m_pGCRenderResources->CreateSrvWithTexture(m_pGCRenderResources->CurrentBackBuffer(), m_pGCRenderResources->GetBackBufferFormat());
	m_pGCRenderResources->m_CommandList->SetGraphicsRootDescriptorTable(DESCRIPTOR_TABLE_SLOT_TEXTURE, srvGpuHandle);

	// Transition pour la texture d'Object ID (g_ObjectIdBuffer)
	//CD3DX12_RESOURCE_BARRIER barrierToShaderResource2 = CD3DX12_RESOURCE_BARRIER::Transition(m_pGCRenderResources->m_ObjectIdBufferRtv, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	//m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &barrierToShaderResource2);

	//Object/Layers BufferId Linking to shader
	if (m_isPixelIDMappingActivated) 
	{
		srvGpuHandle = m_pGCRenderResources->CreateSrvWithTexture(m_pGCRenderResources->CurrentBackBuffer(), m_pGCRenderResources->GetBackBufferFormat());
		m_pGCRenderResources->m_CommandList->SetGraphicsRootDescriptorTable(DESCRIPTOR_TABLE_SLOT_TEXTURE2, srvGpuHandle);
	}

	// Draw quad on post process rtv
	GCMesh* theMesh = m_pGCRenderResources->m_pGraphics->GetMeshes()[0];
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView = theMesh->GetBufferGeometryData()->VertexBufferView();
	D3D12_INDEX_BUFFER_VIEW indexBufferView = theMesh->GetBufferGeometryData()->IndexBufferView();
	m_pGCRenderResources->m_CommandList->IASetVertexBuffers(0, 1, &vertexBufferView);
	m_pGCRenderResources->m_CommandList->IASetIndexBuffer(&indexBufferView);
	m_pGCRenderResources->m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pGCRenderResources->m_CommandList->DrawIndexedInstanced(theMesh->GetBufferGeometryData()->IndexCount, 1, 0, 0, 0);
	//

	// 
	CD3DX12_RESOURCE_BARRIER RtToCopySrc = CD3DX12_RESOURCE_BARRIER::Transition(m_pGCRenderResources->m_pPostProcessingRtv, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COPY_SOURCE);
	m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &RtToCopySrc);
	CD3DX12_RESOURCE_BARRIER PixelShaderToCopyDst = CD3DX12_RESOURCE_BARRIER::Transition(m_pGCRenderResources->CurrentBackBuffer(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_COPY_DEST);
	m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &PixelShaderToCopyDst);

	// Copy On Final
	m_pGCRenderResources->m_CommandList->CopyResource(m_pGCRenderResources->CurrentBackBuffer(), m_pGCRenderResources->m_pPostProcessingRtv);

	//
	CD3DX12_RESOURCE_BARRIER DstToPresent = CD3DX12_RESOURCE_BARRIER::Transition(m_pGCRenderResources->CurrentBackBuffer(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PRESENT);
	m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &DstToPresent);
	CD3DX12_RESOURCE_BARRIER SrcToRt = CD3DX12_RESOURCE_BARRIER::Transition(m_pGCRenderResources->m_pPostProcessingRtv, D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_pGCRenderResources->m_CommandList->ResourceBarrier(1, &SrcToRt);


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

void GCRenderContext::ActiveBasicPostProcessing() {
	m_isBasicPostProcessingActivated = true;
}

void GCRenderContext::DesactiveBasicPostProcessing() {
	m_isBasicPostProcessingActivated = false;
}

void GCRenderContext::ActivePixelIDMapping() {
	m_isPixelIDMappingActivated = true;
}

void GCRenderContext::DesactivePixelIDMapping() {
	m_isPixelIDMappingActivated = false;
}

