#pragma once

class GCPrimitiveFactory
{
public:
	GCPrimitiveFactory();
	~GCPrimitiveFactory();
	bool Initialize();

	void GenerateSphere(float radius, int numSegments, std::vector<DirectX::XMFLOAT3>& outVertices, std::vector<DirectX::XMFLOAT2>& outUvs, std::vector<uint16_t>& outIndices, std::vector<DirectX::XMFLOAT3>& outNormals);
	void GenerateCircle(float radius, int numSegments, std::vector<DirectX::XMFLOAT3>& outVertices, std::vector<DirectX::XMFLOAT2>& outUvs, std::vector<uint16_t>& outIndices, std::vector<DirectX::XMFLOAT3>& outNormals);

	void GenerateCube(std::vector<DirectX::XMFLOAT3>& vertices,
		std::vector<uint16_t>& indices,
		std::vector<DirectX::XMFLOAT2>& uvs,
		std::vector<DirectX::XMFLOAT3>& normals);

	void GenerateCubeSkybox(std::vector<DirectX::XMFLOAT3>& vertices,
		std::vector<uint16_t>& indices,
		std::vector<DirectX::XMFLOAT2>& uvs,
		std::vector<DirectX::XMFLOAT3>& normals);

	//std::vector<DirectX::XMFLOAT3> GenerateNormal(const std::vector<uint16_t>& index, const std::vector<DirectX::XMFLOAT3>& pos);
	GC_GRAPHICS_ERROR BuildGeometry(GC_PRIMITIVE_ID index, DirectX::XMFLOAT4 color, GCGeometry* pGeometry);

private:
	GCRenderContext* m_pRender;

	std::vector<std::map<std::wstring, std::variant<
		std::vector<uint16_t>,
		std::vector<DirectX::XMFLOAT2>,
		std::vector<DirectX::XMFLOAT3>,
		std::vector<DirectX::XMFLOAT4>
	>>> m_primitiveInfos;
};

