#pragma once
class GCUtils
{
public:
	static DirectX::XMFLOAT3 PixelToWorld(float x, float y, UINT windowWidth, UINT windowHeight, const DirectX::XMFLOAT4X4& proj, const DirectX::XMFLOAT4X4& view);
	//static DirectX::XMFLOAT4X4 DirectX::XMMATRIXToXMFLOAT4x4(const DirectX::XMMATRIX& mat);
	//static DirectX::XMMATRIX XMMATRIXToDirectX::XMMATRIX(const DirectX::XMMATRIX& mat);
	//static DirectX::XMMATRIX DirectX::XMMATRIXToXMMATRIX(const DirectX::XMMATRIX& mat);
	//static DirectX::XMMATRIX CreateBillboardMatrix(DirectX::XMVECTOR objectPosition, DirectX::XMVECTOR cameraPosition, DirectX::XMVECTOR cameraUp);
	//static DirectX::XMVECTOR GCVEC3ToXMVECTOR(const GCVEC3& mat);
	//static DirectX::XMFLOAT4 GCColorToXMFLOAT4(const GCColor& color);
};

