#include "pch.h"

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

//GCVEC3 GCUtils::Xmfloat3ToGcvec3(const DirectX::XMFLOAT3& v1)
//{
//	GCVEC3 vec;
//
//	vec.x = v1.x;
//	vec.y = v1.y;
//	vec.z = v1.z;
//
//	return vec;
//}
//
//DirectX::XMFLOAT3 GCUtils::Gcvec3oXmfloat3(const GCVEC3& v1)
//{
//	DirectX::XMFLOAT3 vec;
//
//	vec.x = v1.x;
//	vec.y = v1.y;
//	vec.z = v1.z;
//
//	return vec;
//}
//
//
//DirectX::XMFLOAT3 GCUtils::GetNormal(const GCVEC3& v1, const GCVEC3& v2, const GCVEC3& v3, bool inverse)
//{
//	GCVEC3 normal;
//	GCVEC3 vA, vB;
//	vA = v1 - v2;
//	vB = v3 - v2;
//
//	GCVEC3::CrossProduct(vA, vB, normal);
//	normal.Normalize();
//
//	DirectX::XMFLOAT3 norm;
//
//	norm.x = normal.x;
//	norm.y = normal.y;
//	norm.z = normal.z;
//
//	return norm;
//}
