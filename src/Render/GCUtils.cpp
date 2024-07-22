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

DirectX::XMFLOAT4X4 GCUtils::GCMATRIXToXMFLOAT4x4(const GCMATRIX& mat)
{
	DirectX::XMFLOAT4X4 result;
	result._11 = mat._11; result._12 = mat._12; result._13 = mat._13; result._14 = mat._14;
	result._21 = mat._21; result._22 = mat._22; result._23 = mat._23; result._24 = mat._24;
	result._31 = mat._31; result._32 = mat._32; result._33 = mat._33; result._34 = mat._34;
	result._41 = mat._41; result._42 = mat._42; result._43 = mat._43; result._44 = mat._44;
	return result;
}

GCMATRIX GCUtils::XMMATRIXToGCMATRIX(const DirectX::XMMATRIX& mat)
{
	GCMATRIX result;

	result._11 = mat.r[0].m128_f32[0]; result._12 = mat.r[0].m128_f32[1]; result._13 = mat.r[0].m128_f32[2]; result._14 = mat.r[0].m128_f32[3];
	result._21 = mat.r[1].m128_f32[0]; result._22 = mat.r[1].m128_f32[1]; result._23 = mat.r[1].m128_f32[2]; result._24 = mat.r[1].m128_f32[3];
	result._31 = mat.r[2].m128_f32[0]; result._32 = mat.r[2].m128_f32[1]; result._33 = mat.r[2].m128_f32[2]; result._34 = mat.r[2].m128_f32[3];
	result._41 = mat.r[3].m128_f32[0]; result._42 = mat.r[3].m128_f32[1]; result._43 = mat.r[3].m128_f32[2]; result._44 = mat.r[3].m128_f32[3];

	return result;
}

DirectX::XMMATRIX GCUtils::CreateBillboardMatrix(DirectX::XMVECTOR objectPosition, DirectX::XMVECTOR cameraPosition, DirectX::XMVECTOR cameraUp)
{
	DirectX::XMVECTOR lookAt = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(cameraPosition, objectPosition));
	DirectX::XMVECTOR right = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(cameraUp, lookAt));
	DirectX::XMVECTOR up = DirectX::XMVector3Cross(lookAt, right);

	DirectX::XMMATRIX billboardMatrix = DirectX::XMMatrixIdentity();
	billboardMatrix.r[0] = right;
	billboardMatrix.r[1] = up;
	billboardMatrix.r[2] = lookAt;
	billboardMatrix.r[3] = objectPosition;

	return DirectX::XMMatrixTranspose(billboardMatrix);
}