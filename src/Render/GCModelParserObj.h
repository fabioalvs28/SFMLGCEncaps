#pragma once

struct ObjInfo {
	std::vector<std::vector<float>> coords;
	std::vector<std::vector<uint16_t>> facesInfos;
	std::vector<std::vector<float>> uvs;
};

class GCModelParserObj
{
public:
	GCModelParserObj();
	~GCModelParserObj();
	void Initialize();
	bool ParseObj(std::string fileName);

	GCGeometry* BuildObjColor(std::string fileName);
	GCGeometry* BuildObjTexture(std::string fileName);


private:
	ObjInfo m_ParsedObj;


	std::string m_fileName;

};

