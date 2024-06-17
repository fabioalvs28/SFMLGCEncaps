#pragma once

struct GCGeometry 
{
public:
	virtual ~GCGeometry() {}

	std::vector<std::uint16_t> indices;

	std::size_t vertexNumber;
	std::size_t indiceNumber;

	std::vector<DirectX::XMFLOAT3> pos;
	std::vector<DirectX::XMFLOAT4> color;
	std::vector<DirectX::XMFLOAT2> texC;
};

