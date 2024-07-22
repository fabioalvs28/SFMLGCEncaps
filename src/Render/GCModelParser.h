#pragma once
enum GC_EXTENSIONS {
	obj,
};

struct GC_MODELINFOS
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

	GC_GRAPHICS_ERROR BuildModel(std::string filePath, DirectX::XMFLOAT4 color, GC_EXTENSIONS fileExtension, GCGeometry* pGeometry);

	bool Parse(std::string filePath, GC_EXTENSIONS fileExtension);

protected:
	std::string m_filePath;
	GC_MODELINFOS* m_ParsedModel;
};

