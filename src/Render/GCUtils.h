#pragma once
class GCUtils
{
public:
	static DirectX::XMFLOAT3 PixelToWorld(float x, float y, UINT windowWidth, UINT windowHeight, const DirectX::XMMATRIX& proj, const DirectX::XMMATRIX& view);
};

