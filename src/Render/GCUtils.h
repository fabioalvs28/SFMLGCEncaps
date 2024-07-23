#pragma once
class GCUtils
{
public:
	static DirectX::XMFLOAT3 PixelToWorld(float x, float y, UINT windowWidth, UINT windowHeight, const DirectX::XMFLOAT4X4& proj, const DirectX::XMFLOAT4X4& view);
	static DirectX::XMFLOAT4X4 GCMATRIXToXMFLOAT4x4(const GCMATRIX& mat);
	static GCMATRIX XMMATRIXToGCMATRIX(const DirectX::XMMATRIX& mat);
	static DirectX::XMMATRIX GCMATRIXToXMMATRIX(const GCMATRIX& mat);
	static DirectX::XMMATRIX CreateBillboardMatrix(DirectX::XMVECTOR objectPosition,
		DirectX::XMVECTOR cameraPosition,
		DirectX::XMVECTOR cameraUp);
};

