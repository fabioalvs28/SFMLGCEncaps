#include "pch.h"


ID3D12Resource* GCRenderResources::CreateRTT(bool isStateRenderTarget)
{
	if (isStateRenderTarget)
	{
		ID3D12Resource* renderTargetTexture = nullptr;
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_pRtvHeap->GetCPUDescriptorHandleForHeapStart(), 2 + testCount, m_rtvDescriptorSize);
		// Define the texture description
		D3D12_RESOURCE_DESC textureDesc = {};
		textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		textureDesc.Width = m_pWindow->GetClientWidth(); // Example width
		textureDesc.Height = m_pWindow->GetClientHeight(); // Example height
		textureDesc.DepthOrArraySize = 1;
		textureDesc.MipLevels = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
		// Define the clear value
		D3D12_CLEAR_VALUE clearValue = {};
		clearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		clearValue.Color[0] = 0.0f;
		clearValue.Color[1] = 0.0f;
		clearValue.Color[2] = 0.0f;
		clearValue.Color[3] = 1.0f;
		// Create the render target texture
		CD3DX12_HEAP_PROPERTIES test2 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		HRESULT hr = m_d3dDevice->CreateCommittedResource(
			&test2,
			D3D12_HEAP_FLAG_NONE,
			&textureDesc,
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			&clearValue,
			IID_PPV_ARGS(&renderTargetTexture)
		);
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