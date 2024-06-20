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

	GCGeometry* BuildGeometryColor(int index, DirectX::XMFLOAT4 color);
	GCGeometry* BuildGeometryTexture(int index);
  
private:
	GCRender* m_pRender;
	std::map<int, std::map<std::wstring, std::variant<
		std::vector<uint16_t>,
		std::vector<DirectX::XMFLOAT2>,
		std::vector<DirectX::XMFLOAT3>,
		std::vector<DirectX::XMFLOAT4>
	>>> m_primitiveInfos;
};

