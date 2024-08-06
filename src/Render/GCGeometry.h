#pragma once

// Tous passer en GCVector

struct GCGeometry 
{
public:
	virtual ~GCGeometry() {}

	std::vector<std::uint16_t> indices;

	std::size_t vertexNumber;
	std::size_t indiceNumber;

	std::vector<DirectX::XMFLOAT3> pos;
	std::vector<DirectX::XMFLOAT4> color;
	std::vector<DirectX::XMFLOAT2> uv;

	std::vector<DirectX::XMFLOAT3> normals; 
	std::vector<DirectX::XMFLOAT3> tangents;   
	std::vector<DirectX::XMFLOAT3> binormals;
};


