#pragma once
enum Extensions {
	obj,
};

struct ModelInfos
{
	std::vector<std::vector<float>> coords;
	std::vector<std::vector<uint16_t>> facesInfos;
	std::vector<std::vector<float>> uvs;
};

class GCModelParser
{
public:
	GCModelParser();
	virtual ~GCModelParser();

	GCGeometry* BuildModelColor(std::string fileName, Extensions fileExtension);
	GCGeometry* BuildModelTexture(std::string fileName, Extensions fileExtension);

	bool Parse(std::string fileName, Extensions fileExtension);

protected:
	std::string m_fileName;
	ModelInfos* m_ParsedModel;
	
};

