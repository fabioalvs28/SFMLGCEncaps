#pragma once
enum Extensions {
	obj,
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
	ModelInfos m_ParsedModel;
};

