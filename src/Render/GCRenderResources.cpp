#include "pch.h"


GCRenderResources::GCRenderResources()
	: m_pWindow(nullptr),
	m_renderWidth(800), // Default width
	m_renderHeight(600), // Default height
	m_pGraphics(nullptr),
	m_dxgiFactory(nullptr),
	m_d3dDevice(nullptr),
	m_CommandList(nullptr),
	m_SwapChain(nullptr),
	m_DepthStencilBuffer(nullptr),
	m_CommandQueue(nullptr),
	m_DirectCmdListAlloc(nullptr),
	m_Fence(nullptr),
	m_CurrentFence(0),
	m_pRtvHeap(nullptr),
	m_pDsvHeap(nullptr),
	m_pCbvSrvUavDescriptorHeap(nullptr),
	m_rtvDescriptorSize(0),
	m_dsvDescriptorSize(0),
	m_cbvSrvUavDescriptorSize(0),
	m_canResize(true),
	m_CurrBackBuffer(0),
	m_DepthStencilFormat(DXGI_FORMAT_D24_UNORM_S8_UINT),
	m_BackBufferFormat(DXGI_FORMAT_R8G8B8A8_UNORM),
	m_4xMsaaState(false),
	m_4xMsaaQuality(0),
	m_pPostProcessingRtv(nullptr),
	m_ObjectIdBufferRtv(nullptr),
	m_ObjectIdDepthStencilBuffer(nullptr)
{
}

GCRenderResources::~GCRenderResources() {

	// Release all RTV resources
	for (auto& rtvResource : m_lRenderTargets) {
		SAFE_RELEASE(&rtvResource->resource);
		SAFE_DELETE(&rtvResource);
	}

	// Release all DSV resources
	for (auto& dsvResource : m_lDepthStencilView) {
		SAFE_RELEASE(&dsvResource->resource);
		SAFE_DELETE(&dsvResource);
	}

	// Release Post Processing Resources
	SAFE_RELEASE(&m_pPostProcessingRtv);

	// Release Object/Layer ID Resources

	SAFE_RELEASE(&m_ObjectIdBufferRtv);
	SAFE_RELEASE(&m_ObjectIdDepthStencilBuffer);

	// Release Swap Chain Buffers
	for (int i = 0; i < SwapChainBufferCount; ++i) {
		SAFE_RELEASE(&m_SwapChainBuffer[i]);
	}

	// Release Depth Stencil Buffer
	SAFE_RELEASE(&m_DepthStencilBuffer);

	// Release Descriptor Heaps
	SAFE_RELEASE(&m_pRtvHeap);
	SAFE_RELEASE(&m_pDsvHeap);
	SAFE_RELEASE(&m_pCbvSrvUavDescriptorHeap);

	// Release Command List and Allocator
	SAFE_RELEASE(&m_CommandList);
	SAFE_RELEASE(&m_DirectCmdListAlloc);

	// Release Command Queue
	SAFE_RELEASE(&m_CommandQueue);

	// Release Fence
	SAFE_RELEASE(&m_Fence);

	// Release Device and Factory
	SAFE_RELEASE(&m_d3dDevice);
	SAFE_RELEASE(&m_dxgiFactory);
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