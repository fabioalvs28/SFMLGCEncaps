#include "framework.h"

//converts pixels coordinates to ndc ones
DirectX::XMFLOAT3 GCUtils::PixelToWorld(float x, float y, UINT windowWidth, UINT windowHeight, const DirectX::XMFLOAT4X4& proj, const DirectX::XMFLOAT4X4& view)
{
	float ndcX = (x / windowWidth) * 2.0f - 1.0f;
	float ndcY = 1.0f - (y / windowHeight) * 2.0f;

	float ndcZ = 0.9f;

	DirectX::XMVECTOR ndcSpacePos = DirectX::XMVectorSet(ndcX, ndcY, ndcZ, 1.0f);

	DirectX::XMMATRIX projMatrix = DirectX::XMLoadFloat4x4(&proj);
	DirectX::XMMATRIX viewMatrix = DirectX::XMLoadFloat4x4(&view);

	DirectX::XMMATRIX invViewProj = DirectX::XMMatrixInverse(nullptr, DirectX::XMMatrixMultiply(DirectX::XMMatrixTranspose(viewMatrix), DirectX::XMMatrixTranspose(projMatrix)));

	DirectX::XMVECTOR worldSpacePos = DirectX::XMVector3TransformCoord(ndcSpacePos, invViewProj);

	DirectX::XMFLOAT3 worldPos;
	DirectX::XMStoreFloat3(&worldPos, worldSpacePos);
	return worldPos;
}