#include "pch.h"

GCRender::GCRender() {

}

bool GCRender::Initialize(Window* pWindow, int renderWidth, int renderHeight, GCGraphics* pGraphics)
{
	if (!CHECK_POINTERSNULL("Graphics Initialized with window sucessfully", "Can't initialize Graphics, Window is empty", pWindow))
		return false;

	m_pGraphics = pGraphics;
	m_renderWidth = renderWidth;
	m_renderHeight = renderHeight;
	m_pWindow = pWindow;
	InitDirect3D();
	OnResize();

	return true;
}

void GCRender::ResetCommandList() 
{
	m_CommandList->Reset(m_DirectCmdListAlloc, nullptr);
}

void GCRender::ExecuteCommandList() 
{
	ID3D12CommandList* cmdsLists[] = { m_CommandList };
	m_CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
}

void GCRender::CloseCommandList() 
{
	m_CommandList->Close();
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

	CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory));

	// Try to create hardware device.
	HRESULT hardwareResult = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_d3dDevice));

	// Fallback to WARP device.
	if (FAILED(hardwareResult))
	{
		IDXGIAdapter* pWarpAdapter;
		m_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter));

		D3D12CreateDevice(
			pWarpAdapter,
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&m_d3dDevice));
	}

	m_d3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence));

	m_rtvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	m_dsvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	m_cbvSrvUavDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
	msQualityLevels.Format = m_BackBufferFormat;
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;

	m_d3dDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,&msQualityLevels, sizeof(msQualityLevels));

	m_4xMsaaQuality = msQualityLevels.NumQualityLevels;
	assert(m_4xMsaaQuality > 0 && "Unexpected MSAA quality level.");

	#ifdef _DEBUG
		LogAdapters();
	#endif

	CreateCommandObjects();
	CreateSwapChain();
	CreateRtvAndDsvDescriptorHeaps();
	CreateCbvSrvUavDescriptorHeaps();

	CreatePostProcessingResources();

	m_canResize = true;

	return true;
}

void GCRender::LogAdapters()
{
	UINT i = 0;
	IDXGIAdapter* adapter = nullptr;
	std::vector<IDXGIAdapter*> adapterList;
	while (m_dxgiFactory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND)
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

		LogOutputDisplayModes(output, m_BackBufferFormat);

		ReleaseCom(output);

		++i;
	}
	m_canResize = true;
}

void GCRender::CreateCommandObjects()
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	m_d3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_CommandQueue));

	m_d3dDevice->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_DirectCmdListAlloc));

	m_d3dDevice->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_DirectCmdListAlloc, // Associated command allocator
		nullptr, // Initial PipelineStateObject
		IID_PPV_ARGS(&m_CommandList)
	);

	// Start off in a closed state.  This is because the first time we refer 
	// to the command list we will Reset it, and it needs to be closed before
	// calling Reset.
	m_CommandList->Close();
}

void GCRender::CreateSwapChain()
{
	// Release the previous swapchain we will be recreating.
	if (m_SwapChain != nullptr)
	{
		m_SwapChain->Release();
	}

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = m_pWindow->GetClientWidth();
	sd.BufferDesc.Height = m_pWindow->GetClientHeight();
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = m_BackBufferFormat;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = m_4xMsaaState ? 4 : 1;
	sd.SampleDesc.Quality = m_4xMsaaState ? (m_4xMsaaQuality - 1) : 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	sd.OutputWindow = m_pWindow->GetHMainWnd();
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// Note: Swap chain uses queue to perform flush.
	m_dxgiFactory->CreateSwapChain(m_CommandQueue, &sd, &m_SwapChain);
}

void GCRender::CreateCbvSrvUavDescriptorHeaps()
{
	// Create CBV / SRV / UAV descriptor heap
	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc = {};
	cbvHeapDesc.NumDescriptors = 1000;
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvHeapDesc.NodeMask = 0;
	m_d3dDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&m_cbvSrvUavDescriptorHeap));
}

void GCRender::CreateRtvAndDsvDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
	rtvHeapDesc.NumDescriptors = 1000;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;
	m_d3dDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap));


	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask = 0;
	m_d3dDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_dsvHeap));
}

void GCRender::CreatePostProcessingResources() {

	// Create Base texture for m_copyTexture, for copy backbuffer inside
	D3D12_RESOURCE_DESC textureDesc = {};
	textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureDesc.Width = m_pWindow->GetClientWidth();
	textureDesc.Height = m_pWindow->GetClientHeight();
	textureDesc.DepthOrArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.Format = GetBackBufferFormat();
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	
	CD3DX12_HEAP_PROPERTIES heapProperties(D3D12_HEAP_TYPE_DEFAULT);
	HRESULT hr = m_d3dDevice->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&textureDesc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(&m_copyTexture)
	);
	// ***

	// Create RTV from Texture Resource, from it self
	m_d3dDevice->CreateCommittedResource(&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&textureDesc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(&m_pRenderTargetTexture));

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // Format de la texture
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	rtvDesc.Texture2D.PlaneSlice = 0;

	m_pRenderTargetTextureAdress = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
	m_pRenderTargetTextureAdress.Offset(90, m_rtvDescriptorSize);  // Assurez-vous que m_rtvDescriptorSize est correct
	m_d3dDevice->CreateRenderTargetView(m_pRenderTargetTexture, &rtvDesc, m_pRenderTargetTextureAdress);
	// 90 offset cpu handle, we store it for use it in SetOMRenderTarget At post processing stage



	// Create A post Processing Shader
	m_postProcessingShader = new GCShader();

	std::string shaderFilePath = "../../../src/Render/Shaders/PostProcessing.hlsl";
	std::string csoDestinationPath = "../../../src/Render/CsoCompiled/PostProcessing";

	int flags = 0;
	SET_FLAG(flags, HAS_POSITION);
	SET_FLAG(flags, HAS_UV);

	m_postProcessingShader->Initialize(this, shaderFilePath, csoDestinationPath, flags);
	m_postProcessingShader->Load();
}

void GCRender::OnResize() 
{
	// Checks initial conditions
	m_canResize = true;
	if (m_canResize == false)
		return;

	assert(m_d3dDevice);
	assert(m_SwapChain);
	assert(m_DirectCmdListAlloc);

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

void GCRender::ReleasePreviousResources() 
{
	for (int i = 0; i < SwapChainBufferCount; ++i) 
	{
		if (m_SwapChainBuffer[i] != nullptr) 
		{
			m_SwapChainBuffer[i]->Release();
		}
	}
	if (m_DepthStencilBuffer != nullptr) 
	{
		m_DepthStencilBuffer->Release();
	}
}

void GCRender::ResizeSwapChain() 
{
	m_SwapChain->ResizeBuffers(
		SwapChainBufferCount,
		m_pWindow->GetClientWidth(), m_pWindow->GetClientHeight(),
		m_BackBufferFormat,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	);

	m_CurrBackBuffer = 0;
}

void GCRender::CreateRenderTargetViews() 
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < SwapChainBufferCount; i++) 
	{
		m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_SwapChainBuffer[i]));
		m_d3dDevice->CreateRenderTargetView(m_SwapChainBuffer[i], nullptr, rtvHeapHandle);
		rtvHeapHandle.Offset(1, m_rtvDescriptorSize);
	}
}

void GCRender::CreateDepthStencilBufferAndView()
{
	D3D12_RESOURCE_DESC depthStencilDesc = {};
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = m_pWindow->GetClientWidth();
	depthStencilDesc.Height = m_pWindow->GetClientHeight();
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthStencilDesc.SampleDesc.Count = m_4xMsaaState ? 4 : 1;
	depthStencilDesc.SampleDesc.Quality = m_4xMsaaState ? (m_4xMsaaQuality - 1) : 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear = {};
	optClear.Format = m_DepthStencilFormat;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;

	CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_DEFAULT);
	m_d3dDevice->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&depthStencilDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&optClear,
		IID_PPV_ARGS(&m_DepthStencilBuffer)
	);

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = m_DepthStencilFormat;
	dsvDesc.Texture2D.MipSlice = 0;
	m_d3dDevice->CreateDepthStencilView(m_DepthStencilBuffer, &dsvDesc, GetDepthStencilView());

	CD3DX12_RESOURCE_BARRIER ResBar(CD3DX12_RESOURCE_BARRIER::Transition(m_DepthStencilBuffer, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));

	m_CommandList->ResourceBarrier(1, &ResBar);
}

void GCRender::UpdateViewport() 
{
	m_ScreenViewport.TopLeftX = (m_pWindow->GetClientWidth() - m_renderWidth) / 2;
	m_ScreenViewport.TopLeftY = (m_pWindow->GetClientHeight() - m_renderHeight) / 2;
	m_ScreenViewport.Width = static_cast<float>(m_renderWidth);
	m_ScreenViewport.Height = static_cast<float>(m_renderHeight);
	m_ScreenViewport.MinDepth = 0.0f;
	m_ScreenViewport.MaxDepth = 1.0f;

	m_ScissorRect = { 
		(m_pWindow->GetClientWidth() - m_renderWidth) / 2, 
		(m_pWindow->GetClientHeight() - m_renderHeight) / 2,
		(m_pWindow->GetClientWidth() - m_renderWidth)/ 2 + m_renderWidth,
		(m_pWindow->GetClientHeight() - m_renderHeight) / 2 + m_renderHeight
	};
}

bool GCRender::FlushCommandQueue()
{
	HRESULT hr;
	// Advance the fence value to mark commands up to this fence point.
	m_CurrentFence++;
	// Add an instruction to the command queue to set a new fence point.  Because we 
	// are on the GPU timeline, the new fence point won't be set until the GPU finishes
	// processing all the commands prior to this Signal().
	hr = m_CommandQueue->Signal(m_Fence, m_CurrentFence);
	if (!CHECK_HRESULT(hr, "m_CommandQueue->Signal")) {
		return false;
	};

	// Wait until the GPU has completed commands up to this fence point.
	if (m_Fence->GetCompletedValue() < m_CurrentFence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, NULL, false, EVENT_ALL_ACCESS);

		// Fire event when GPU hits current fence.  
		hr = m_Fence->SetEventOnCompletion(m_CurrentFence, eventHandle);
		if (!CHECK_HRESULT(hr, "Fence->SetEventOnCompletion")) {
			return false;
		};

		// Wait until the GPU hits current fence event is fired.
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
	return true;
}

//Always needs to be called right before drawing!!!
bool GCRender::PrepareDraw() 
{
	//Always needs to be called right before drawing!!!

	HRESULT hr = m_DirectCmdListAlloc->Reset();
	if (!CHECK_HRESULT(hr, "m_DirectCmdListAlloc->Reset()")) {
		return false;
	};

	hr = m_CommandList->Reset(m_DirectCmdListAlloc, nullptr);

	if (!CHECK_HRESULT(hr, "m_CommandList->Reset()")) {
		return false;
	};

	m_CommandList->RSSetViewports(1, &m_ScreenViewport);
	m_CommandList->RSSetScissorRects(1, &m_ScissorRect);

	// Swap
	CD3DX12_RESOURCE_BARRIER ResBar(CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
	m_CommandList->ResourceBarrier(1, &ResBar);
	m_CommandList->ClearRenderTargetView(CurrentBackBufferView(), DirectX::Colors::LightBlue, 1, &m_ScissorRect);
	m_CommandList->ClearDepthStencilView(GetDepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	D3D12_CPU_DESCRIPTOR_HANDLE dsv = GetDepthStencilView();
	D3D12_CPU_DESCRIPTOR_HANDLE cbbv = CurrentBackBufferView();
	m_CommandList->OMSetRenderTargets(1, &cbbv, true, &dsv);

	ID3D12DescriptorHeap* descriptorHeaps[] = { m_cbvSrvUavDescriptorHeap };
	m_CommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	return true;
}

//Draws an object specified in the arguments using a specified material and mesh, You can call many times this function without call again a each draw Prepare and Post function, 
bool GCRender::DrawObject(GCMesh* pMesh, GCMaterial* pMaterial)
{
	if (pMaterial == nullptr || pMaterial->GetShader() == nullptr || pMesh == nullptr)
		return false;
	if (!COMPARE_SHADER_MESH_FLAGS(pMaterial, pMesh)) 
		return false;

	m_CommandList->SetPipelineState(pMaterial->GetShader()->GetPso());
	m_CommandList->SetGraphicsRootSignature(pMaterial->GetShader()->GetRootSign());

	m_CommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView = pMesh->GetBufferGeometryData()->VertexBufferView();
	m_CommandList->IASetVertexBuffers(0, 1, &vertexBufferView);
	D3D12_INDEX_BUFFER_VIEW indexBufferView = pMesh->GetBufferGeometryData()->IndexBufferView();
	m_CommandList->IASetIndexBuffer(&indexBufferView);

	// Update texture if material has texture
	pMaterial->UpdateTexture();

	// Update cb0, cb of object
	m_CommandList->SetGraphicsRootConstantBufferView(CBV_SLOT_CB0, pMaterial->GetCbObjectInstance()[pMaterial->GetCount()]->Resource()->GetGPUVirtualAddress());
	// Set cb object buffer on used
	pMaterial->GetCbObjectInstance()[pMaterial->GetCount()]->m_isUsed = true;
	pMaterial->IncrementCBCount();

	// cb1, Camera
	m_CommandList->SetGraphicsRootConstantBufferView(CBV_SLOT_CB1, m_pCbCurrentViewProjInstance->Resource()->GetGPUVirtualAddress());
	// cb2, Material Properties
	m_CommandList->SetGraphicsRootConstantBufferView(CBV_SLOT_CB2, pMaterial->GetCbMaterialPropertiesInstance()->Resource()->GetGPUVirtualAddress());
	// cb3, Light Properties
	m_CommandList->SetGraphicsRootConstantBufferView(CBV_SLOT_CB3, m_pCbLightPropertiesInstance->Resource()->GetGPUVirtualAddress());

	// Draw
	m_CommandList->DrawIndexedInstanced(pMesh->GetBufferGeometryData()->IndexCount, 1, 0, 0, 0);

	return true;
}

void GCRender::PerformPostProcessing()
{
	// 1. Transitionner le back buffer vers l'état COPY_SOURCE
	CD3DX12_RESOURCE_BARRIER barrierToCopySource = CD3DX12_RESOURCE_BARRIER::Transition(
		CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_PRESENT,  // État actuel du back buffer lorsqu'il est présenté
		D3D12_RESOURCE_STATE_COPY_SOURCE);
	m_CommandList->ResourceBarrier(1, &barrierToCopySource);

	// 2. Transitionner la texture vers l'état COPY_DEST
	CD3DX12_RESOURCE_BARRIER barrierToCopyDest = CD3DX12_RESOURCE_BARRIER::Transition(
		m_copyTexture,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,  // État actuel de la texture après le post-processing
		D3D12_RESOURCE_STATE_COPY_DEST);
	m_CommandList->ResourceBarrier(1, &barrierToCopyDest);

	// 3. Copier le contenu du back buffer dans la texture de rendu cible
	m_CommandList->CopyResource(m_copyTexture, CurrentBackBuffer());

	// 4. Transitionner la texture de retour vers l'état RENDER_TARGET
	CD3DX12_RESOURCE_BARRIER barrierBackToRT = CD3DX12_RESOURCE_BARRIER::Transition(
		m_copyTexture,
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_CommandList->ResourceBarrier(1, &barrierBackToRT);

	// 5. Rétransitionner le back buffer vers l'état RENDER_TARGET (préparation pour la présentation)
	CD3DX12_RESOURCE_BARRIER barrierBackToPresent = CD3DX12_RESOURCE_BARRIER::Transition(
		CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_COPY_SOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_CommandList->ResourceBarrier(1, &barrierBackToPresent);

	// 6. Transitionner la texture vers l'état PIXEL_SHADER_RESOURCE
	CD3DX12_RESOURCE_BARRIER barrierToPixelShaderResource = CD3DX12_RESOURCE_BARRIER::Transition(
		m_copyTexture,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	m_CommandList->ResourceBarrier(1, &barrierToPixelShaderResource);

	CD3DX12_RESOURCE_BARRIER barrierToRT = CD3DX12_RESOURCE_BARRIER::Transition(
		m_pRenderTargetTexture,
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_CommandList->ResourceBarrier(1, &barrierToRT);

	// 7. Définir les cibles de rendu pour le post-processing
	D3D12_CPU_DESCRIPTOR_HANDLE dsv = GetDepthStencilView();
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
	rtvHandle.Offset(90, m_rtvDescriptorSize);  // Offset correct pour votre RTV
	m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsv);

	//// 8. Définir l'état du pipeline et la signature racine
	m_CommandList->SetPipelineState(m_postProcessingShader->GetPso());
	m_CommandList->SetGraphicsRootSignature(m_postProcessingShader->GetRootSign());

	//// 9. Définir la table de descripteurs pour l'entrée texture
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // Format de la texture
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	CD3DX12_CPU_DESCRIPTOR_HANDLE srvCpuHandle(m_cbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	srvCpuHandle.Offset(90, m_cbvSrvUavDescriptorSize);  // Assurez-vous que m_cbvSrvUavDescriptorSize est correct
	m_d3dDevice->CreateShaderResourceView(m_copyTexture, &srvDesc, srvCpuHandle);
	CD3DX12_GPU_DESCRIPTOR_HANDLE srvGpuHandle(m_cbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
	srvGpuHandle.Offset(90, m_cbvSrvUavDescriptorSize);  // Assurez-vous que m_cbvSrvUavDescriptorSize est correct
	m_CommandList->SetGraphicsRootDescriptorTable(DESCRIPTOR_TABLE_SLOT_TEXTURE, srvGpuHandle);

	// 10. Définir les buffers de vertex et d'index pour le quad
	m_CommandList->SetPipelineState(m_postProcessingShader->GetPso());
	m_CommandList->SetGraphicsRootSignature(m_postProcessingShader->GetRootSign());

	GCMesh* theMesh = m_pGraphics->GetMeshes()[0];
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView = theMesh->GetBufferGeometryData()->VertexBufferView();
	D3D12_INDEX_BUFFER_VIEW indexBufferView = theMesh->GetBufferGeometryData()->IndexBufferView();
	m_CommandList->IASetVertexBuffers(0, 1, &vertexBufferView);
	m_CommandList->IASetIndexBuffer(&indexBufferView);
	m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//// 11. Dessiner un quad plein écran pour appliquer l'effet de post-processing
	m_CommandList->DrawIndexedInstanced(theMesh->GetBufferGeometryData()->IndexCount, 1, 0, 0, 0);

	// 1. Transitionner m_pRenderTargetTexture vers l'état COPY_SOURCE
	CD3DX12_RESOURCE_BARRIER barrierToCopySource2 = CD3DX12_RESOURCE_BARRIER::Transition(
		m_pRenderTargetTexture,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_COPY_SOURCE);
	m_CommandList->ResourceBarrier(1, &barrierToCopySource2);

	// 2. Transitionner le back buffer vers l'état COPY_DEST
	CD3DX12_RESOURCE_BARRIER barrierToCopyDest2 = CD3DX12_RESOURCE_BARRIER::Transition(
		CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_COPY_DEST);
	m_CommandList->ResourceBarrier(1, &barrierToCopyDest2);

	// 3. Copier le contenu de m_pRenderTargetTexture dans le back buffer
	m_CommandList->CopyResource(CurrentBackBuffer(), m_pRenderTargetTexture);


}

//Always needs to be called right after drawing!!!
bool GCRender::PostDraw()
{


    PerformPostProcessing();



	CD3DX12_RESOURCE_BARRIER barrierBackToPresent2 = CD3DX12_RESOURCE_BARRIER::Transition(
		CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_PRESENT);
	m_CommandList->ResourceBarrier(1, &barrierBackToPresent2);

	

	// Fermer la liste de commandes
	HRESULT hr = m_CommandList->Close();
	if (!CHECK_HRESULT(hr, "Failed to close command list")) {
		return false;
	}

	// Exécuter la liste de commandes
	ID3D12CommandList* cmdsLists[] = { m_CommandList };
	m_CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Présenter le swap chain
	hr = m_SwapChain->Present(0, 0);
	if (!CHECK_HRESULT(hr, "Failed to present swap chain")) {
		return false;
	}

	// Passer au back buffer suivant
	m_CurrBackBuffer = (m_CurrBackBuffer + 1) % SwapChainBufferCount;

	// Flush the command queue
	if (!FlushCommandQueue())
		return false;

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
