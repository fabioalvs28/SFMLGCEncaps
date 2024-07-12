#pragma once
enum Extensions {
	obj,
};

struct ModelInfos
{
	std::vector<std::vector<float>> coords;
	std::vector<std::vector<uint16_t>> facesInfos;
	std::vector<std::vector<float>> uvs;
	std::vector<std::vector<float>> normals;
};

class GCModelParser
{
public:
	GCModelParser();
	virtual ~GCModelParser();

	GC_GRAPHICS_ERROR BuildModel(std::string filePath, DirectX::XMFLOAT4 color, Extensions fileExtension, GCGeometry* pGeometry);

	bool Parse(std::string filePath, Extensions fileExtension);

protected:
	std::string m_filePath;
	ModelInfos* m_ParsedModel;
	
};

