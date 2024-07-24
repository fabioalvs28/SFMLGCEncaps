#include "pch.h"


GCRenderResources::GCRenderResources()
	: m_pWindow(nullptr),
	m_renderWidth(800), // Default width
	m_renderHeight(600), // Default height
	m_ScreenViewport(),

	m_pGraphics(nullptr),
	m_dxgiFactory(nullptr),
	m_d3dDevice(nullptr),
	m_SwapChain(nullptr),
	m_SwapChainBuffer(),
	m_DepthStencilBuffer(nullptr),

	m_CommandList(nullptr),
	m_CommandQueue(nullptr),
	m_DirectCmdListAlloc(nullptr),

	m_Fence(nullptr),
	m_CurrentFence(0),

	m_pRtvHeap(nullptr),
	m_pDsvHeap(nullptr),
	m_pCbvSrvUavDescriptorHeap(nullptr),

	m_canResize(true),

	m_CurrBackBuffer(0),
	m_DepthStencilFormat(DXGI_FORMAT_D24_UNORM_S8_UINT),
	m_BackBufferFormat(DXGI_FORMAT_R8G8B8A8_UNORM),

	m_ScissorRect(),

	m_4xMsaaQuality(0),
	m_4xMsaaState(false),
	
	m_rtvDescriptorSize(0),
	m_dsvDescriptorSize(0),
	m_cbvSrvUavDescriptorSize(0)
{
}

GCRenderResources::~GCRenderResources() {
	// Release Descriptor Heaps
	m_pRtvHeap->Release();
	m_pDsvHeap->Release();
	m_pCbvSrvUavDescriptorHeap->Release();

	// Release all RTV resources
	for (auto& rtvResource : m_lRenderTargets) {
		rtvResource->resource->Release();
	}

	// Release all DSV resources
	for (auto& dsvResource : m_lDepthStencilView) {
		dsvResource->resource->Release();
	}

	m_lShaderResourceView.clear(); 
	m_lUnorderedAccessView.clear();

	// Release Swap Chain Buffers
	for (int i = 0; i < SwapChainBufferCount; ++i) {
		m_SwapChainBuffer[i]->Release();
	}

	// Release Command List and Allocator
	m_CommandList->Release();
	m_DirectCmdListAlloc->Release();

	// Release Command Queue
	m_CommandQueue->Release();

	// Release Fence
	m_Fence->Release();

	// Release Device and Factory
	m_d3dDevice->Release();
	m_dxgiFactory->Release();
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
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvCpuHandle(m_pRtvHeap->GetCPUDescriptorHandleForHeapStart());
	rtvCpuHandle.Offset(m_rtvOffsetCount, m_rtvDescriptorSize);

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
	textureDesc.Flags = resourceFlags;


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
		D3D12_RESOURCE_STATE_COMMON,
		resourceFlags == D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS ? nullptr: actualClearValue,
		IID_PPV_ARGS(&renderTargetTexture)
	);

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = format;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	rtvDesc.Texture2D.PlaneSlice = 0;

	if(resourceFlags != D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS)
	{
		m_d3dDevice->CreateRenderTargetView(renderTargetTexture, &rtvDesc, rtvCpuHandle);
	}

	// Manager Rtv
	GC_DESCRIPTOR_RESOURCE* descriptorResource = new GC_DESCRIPTOR_RESOURCE();
	descriptorResource->resource = renderTargetTexture;
	descriptorResource->cpuHandle = rtvCpuHandle;

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

CD3DX12_GPU_DESCRIPTOR_HANDLE GCRenderResources::CreateStaticSrvWithTexture(ID3D12Resource* textureResource, DXGI_FORMAT format)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	CD3DX12_CPU_DESCRIPTOR_HANDLE srvCpuHandle(m_pCbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	srvCpuHandle.Offset(m_srvStaticOffsetCount, m_cbvSrvUavDescriptorSize);

	CD3DX12_GPU_DESCRIPTOR_HANDLE srvGpuHandle(m_pCbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
	srvGpuHandle.Offset(m_srvStaticOffsetCount, m_cbvSrvUavDescriptorSize);

	m_d3dDevice->CreateShaderResourceView(textureResource, &srvDesc, srvCpuHandle);

	GCGraphicsLogger& profiler = GCGraphicsLogger::GetInstance();
	profiler.LogWarning("Offset srv count : " + std::to_string(m_srvStaticOffsetCount));

	//m_lShaderResourceView.push_back(srvGpuHandle);
	m_lShaderResourceView.push_back(srvGpuHandle);

	m_srvStaticOffsetCount++;

	return srvGpuHandle;
}

CD3DX12_GPU_DESCRIPTOR_HANDLE GCRenderResources::CreateDynamicSrvWithTexture(ID3D12Resource* textureResource, DXGI_FORMAT format)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	CD3DX12_CPU_DESCRIPTOR_HANDLE srvCpuHandle(m_pCbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	srvCpuHandle.Offset(m_srvDynamicOffsetCount, m_cbvSrvUavDescriptorSize);

	CD3DX12_GPU_DESCRIPTOR_HANDLE srvGpuHandle(m_pCbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
	srvGpuHandle.Offset(m_srvDynamicOffsetCount, m_cbvSrvUavDescriptorSize);

	m_d3dDevice->CreateShaderResourceView(textureResource, &srvDesc, srvCpuHandle);


	GCGraphicsLogger& profiler = GCGraphicsLogger::GetInstance();
	profiler.LogWarning("Offset srv count : " + std::to_string(m_srvDynamicOffsetCount));

	//m_lShaderResourceView.push_back(srvGpuHandle);
	m_lShaderResourceView.push_back(srvGpuHandle);
	
	m_srvDynamicOffsetCount++;

	return srvGpuHandle;
}

CD3DX12_GPU_DESCRIPTOR_HANDLE GCRenderResources::CreateUavTexture(ID3D12Resource* textureResource)
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE uavCpuHandle(m_pCbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	uavCpuHandle.Offset(m_uavOffsetCount, m_cbvSrvUavDescriptorSize);

	CD3DX12_GPU_DESCRIPTOR_HANDLE uavGpuHandle(m_pCbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
	uavGpuHandle.Offset(m_uavOffsetCount, m_cbvSrvUavDescriptorSize);

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice = 0;

	m_d3dDevice->CreateUnorderedAccessView(textureResource, nullptr, &uavDesc, uavCpuHandle);

	GCGraphicsLogger& logger = GCGraphicsLogger::GetInstance();
	logger.LogWarning("Offset uav count : " + std::to_string(m_uavOffsetCount));

	m_lUnorderedAccessView.push_back(uavCpuHandle);
	m_uavOffsetCount++;

	return uavGpuHandle;
}

//void GCRenderResources::DeleteRenderTarget(ID3D12Resource* pRenderTarget) {
//	auto it = std::find(m_renderTargets.begin(), m_renderTargets.end(), pRenderTarget);
//	if (it != m_renderTargets.end()) {
//		if (pRenderTarget) {
//			pRenderTarget->Release();
//			pRenderTarget = nullptr;
//		}
//		m_renderTargets.erase(it);
//	}
//}