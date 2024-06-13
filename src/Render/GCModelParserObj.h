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
	void Initialize(GCRender* pRender);
	bool ParseObj(std::string fileName);

	GCGeometry* BuildObjColor(std::string fileName);
	GCGeometry* BuildObjTexture(std::string fileName);


private:
	ObjInfo m_ParsedObj;


	GCRender* m_pRender;
	std::string m_fileName;

};

