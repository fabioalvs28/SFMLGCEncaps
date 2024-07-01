#pragma once
class GCUtils
{
public:
	static DirectX::XMFLOAT3 PixelToWorld(float x, float y, UINT windowWidth, UINT windowHeight, const DirectX::XMFLOAT4X4& proj, const DirectX::XMFLOAT4X4& view);
	//static DirectX::XMFLOAT3 GetNormal(const GCVEC3& v1, const GCVEC3& v2, const GCVEC3& v3, bool inverse);
	//static DirectX::XMFLOAT3 Gcvec3oXmfloat3(const GCVEC3& v1);
	//static GCVEC3 Xmfloat3ToGcvec3(const DirectX::XMFLOAT3& v1);
};

