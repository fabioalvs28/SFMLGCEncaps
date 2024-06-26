#pragma once

#include <map>
#include <string>
#include <variant>
#include <vector>
#include <DirectXMath.h> // Assurez-vous que DirectXMath.h est inclus si vous utilisez des types DirectX


class GCPrimitiveFactory
{
public:
	GCPrimitiveFactory();
	~GCPrimitiveFactory();
	void Initialize();

	void GenerateSphere(float radius, int numSegments, std::vector<DirectX::XMFLOAT3>& outVertices, std::vector<DirectX::XMFLOAT2>& outUvs, std::vector<uint16_t>& outIndices, std::vector<DirectX::XMFLOAT3>& outNormals);
	void GenerateCircle(float radius, int numSegments, std::vector<DirectX::XMFLOAT3>& outVertices, std::vector<DirectX::XMFLOAT2>& outUvs, std::vector<uint16_t>& outIndices);

	std::vector<DirectX::XMFLOAT3> GenerateNormal(const std::vector<uint16_t>& index, const std::vector<DirectX::XMFLOAT3>& pos);
	GCGeometry* BuildGeometry(std::string name, DirectX::XMFLOAT4 color, int& flagEnabledBits);

  
private:
	GCRender* m_pRender;
	std::map<std::wstring, std::map<std::wstring, std::variant<
		std::vector<uint16_t>,
		std::vector<DirectX::XMFLOAT2>,
		std::vector<DirectX::XMFLOAT3>,
		std::vector<DirectX::XMFLOAT4>
	>>> m_primitiveInfos;
};

