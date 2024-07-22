#pragma once

struct GC_MODELINFOS;

class GCModelParserObj :public GCModelParser
{
public:
	GCModelParserObj();
	~GCModelParserObj();

	GC_MODELINFOS* Parse(std::string filePath);
};