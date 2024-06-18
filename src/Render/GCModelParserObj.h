#pragma once

struct ModelInfos
{
	std::vector<std::vector<float>> coords;
	std::vector<std::vector<uint16_t>> facesInfos;
	std::vector<std::vector<float>> uvs;
};

class GCModelParserObj :public GCModelParser
{
public:
	GCModelParserObj();
	~GCModelParserObj();

	bool Parse(std::string fileName);
};