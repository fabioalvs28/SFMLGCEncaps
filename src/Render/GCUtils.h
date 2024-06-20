#pragma once
class GCUtils
{
public:
	static DirectX::XMFLOAT3 PixelToWorld(float x, float y, UINT windowWidth, UINT windowHeight, const DirectX::XMFLOAT4X4& proj, const DirectX::XMFLOAT4X4& view);
};

