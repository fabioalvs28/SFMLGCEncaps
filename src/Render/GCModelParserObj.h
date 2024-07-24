#pragma once

class GCModelParserObj :public GCModelParserFactory
{
public:
	GCModelParserObj();
	~GCModelParserObj();

	GC_MODELINFOS* Parse(std::string filePath);
};