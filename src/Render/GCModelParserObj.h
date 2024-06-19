#pragma once

struct ModelInfos;

class GCModelParserObj :public GCModelParser
{
public:
	GCModelParserObj();
	~GCModelParserObj();

	ModelInfos* Parse(std::string fileName);
};