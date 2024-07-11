#include "pch.h"

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
	GC_DESCRIPTOR_RESOURCE* descriptorResource = new GC_DESCRIPTOR_RESOURCE;
	descriptorResource->resource = renderTargetTexture;
	descriptorResource->cpuHandle = rtvHeapHandle;
	m_lRenderTargets.push_back(descriptorResource);

	m_rtvOffsetCount++;

	return descriptorResource;
}

GC_DESCRIPTOR_RESOURCE* GCRenderResources::CreateRTV(ID3D12Resource* pResource)
{
	//Handle Cpu
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_pRtvHeap->GetCPUDescriptorHandleForHeapStart(), m_rtvOffsetCount, m_rtvDescriptorSize);
	rtvHeapHandle.Offset(m_rtvOffsetCount, m_rtvDescriptorSize);

	m_d3dDevice->CreateRenderTargetView(pResource, nullptr, rtvHeapHandle);

	// Manager Rtv
	GC_DESCRIPTOR_RESOURCE* descriptorResource = new GC_DESCRIPTOR_RESOURCE;
	descriptorResource->resource = pResource;
	descriptorResource->cpuHandle = rtvHeapHandle;
	m_lRenderTargets.push_back(descriptorResource);

	m_rtvOffsetCount++;

	return descriptorResource;
}

GC_DESCRIPTOR_RESOURCE* GCRenderResources::CreateDepthStencilBufferAndView(DXGI_FORMAT depthStencilFormat)
{
	HRESULT hr;

	D3D12_RESOURCE_DESC depthStencilDesc = {};
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = m_renderWidth;
	depthStencilDesc.Height = m_renderHeight;
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = m_4xMsaaState ? 4 : 1;
	depthStencilDesc.SampleDesc.Quality = m_4xMsaaState ? (m_4xMsaaQuality - 1) : 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear = {};
	optClear.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;

	CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_DEFAULT);
	ID3D12Resource* depthStencilBuffer = nullptr;
	hr = m_d3dDevice->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&depthStencilDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&optClear,
		IID_PPV_ARGS(&depthStencilBuffer)
	);
	CHECK_HRESULT(hr, "ERROR DS PAS BON");

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.Texture2D.MipSlice = 0;

	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(m_pDsvHeap->GetCPUDescriptorHandleForHeapStart());
	dsvHandle.Offset(m_dsvOffsetCount, m_dsvDescriptorSize);

	m_d3dDevice->CreateDepthStencilView(depthStencilBuffer, &dsvDesc, dsvHandle);

	GC_DESCRIPTOR_RESOURCE* dsv = new GC_DESCRIPTOR_RESOURCE{ depthStencilBuffer, dsvHandle };

	m_dsvOffsetCount++;
	m_lDepthStencilView.push_back(dsv);

	return dsv;
}

//ID3D12Resource* GCRenderResources::CreateRTT(bool isStateRenderTarget)
//{
//	if (isStateRenderTarget)
//	{
//		ID3D12Resource* renderTargetTexture = nullptr;
//		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_pRtvHeap->GetCPUDescriptorHandleForHeapStart(), 2 + testCount, m_rtvDescriptorSize);
//		// Define the texture description
//		D3D12_RESOURCE_DESC textureDesc = {};
//		textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
//		textureDesc.Width = m_pWindow->GetClientWidth(); // Example width
//		textureDesc.Height = m_pWindow->GetClientHeight(); // Example height
//		textureDesc.DepthOrArraySize = 1;
//		textureDesc.MipLevels = 1;
//		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//		textureDesc.SampleDesc.Count = 1;
//		textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
//		// Define the clear value
//		D3D12_CLEAR_VALUE clearValue = {};
//		clearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//		clearValue.Color[0] = 0.0f;
//		clearValue.Color[1] = 0.0f;
//		clearValue.Color[2] = 0.0f;
//		clearValue.Color[3] = 1.0f;
//		// Create the render target texture
//		CD3DX12_HEAP_PROPERTIES test2 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
//		HRESULT hr = m_d3dDevice->CreateCommittedResource(
//			&test2,
//			D3D12_HEAP_FLAG_NONE,
//			&textureDesc,
//			D3D12_RESOURCE_STATE_RENDER_TARGET,
//			&clearValue,
//			IID_PPV_ARGS(&renderTargetTexture)
//		);
//		if (FAILED(hr)) {
//			// Handle error
//			MessageBoxA(nullptr, "Failed to create render target texture", "Error", MB_OK);
//		}
//		m_d3dDevice->CreateRenderTargetView(renderTargetTexture, nullptr, rtvHeapHandle);
//		rtvHeapHandle.Offset(1, m_rtvDescriptorSize);
//		m_renderTargets.push_back(renderTargetTexture);
//		testCount += 1;
//		return renderTargetTexture;
//	}
//	else {
//		ID3D12Resource* renderTargetTexture = nullptr;
//		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_pRtvHeap->GetCPUDescriptorHandleForHeapStart(), 2 + testCount, m_rtvDescriptorSize);
//		D3D12_RESOURCE_DESC textureDesc2 = {};
//		textureDesc2.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
//		textureDesc2.Width = m_pWindow->GetClientWidth();
//		textureDesc2.Height = m_pWindow->GetClientHeight();
//		textureDesc2.DepthOrArraySize = 1;
//		textureDesc2.MipLevels = 1;
//		textureDesc2.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
//		textureDesc2.SampleDesc.Count = 1;
//		textureDesc2.SampleDesc.Quality = 0;
//		textureDesc2.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
//		textureDesc2.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
//
//		CD3DX12_HEAP_PROPERTIES test2 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
//		HRESULT hr = m_d3dDevice->CreateCommittedResource(
//			&test2,
//			D3D12_HEAP_FLAG_NONE,
//			&textureDesc2,
//			D3D12_RESOURCE_STATE_COMMON,
//			nullptr,
//			IID_PPV_ARGS(&renderTargetTexture));
//		if (FAILED(hr)) {
//			// Handle error
//			MessageBoxA(nullptr, "Failed to create render target texture", "Error", MB_OK);
//		}
//		m_d3dDevice->CreateRenderTargetView(renderTargetTexture, nullptr, rtvHeapHandle);
//		rtvHeapHandle.Offset(1, m_rtvDescriptorSize);
//		m_renderTargets.push_back(renderTargetTexture);
//		testCount += 1;
//		return renderTargetTexture;
//	}
//}
//
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