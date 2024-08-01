#include "SpriteSheetGenerator.h"

int main(int argc, char** argv)
{
	SpriteSheetGenerator Packer;
	fs::path importPath = "C:/Users/ssanchez/Documents/GitHub/gce-dev/bin/SpriteSheetGenerator-py/randomImage/";
	fs::path outputPath = "C:/Users/ssanchez/Documents/GitHub/gce-dev/bin/SpriteSheetGenerator-py/output/";
	int padding = 1;
	bool allowRotate = false;
	bool allowTrimming = false;
	bool tryRefillPrevRow = false;

	std::string argument;
	std::string nextArgument;
	for (int i = 1; i < argc; ++i) {
		argument = argv[i];
		if (argument != "-R" && argument != "-T" && argument != "-F")
			nextArgument = argv[++i];

		if (argument == "-i" || argument == "--import")
			importPath = nextArgument + "/";
		if (argument == "-o" || argument == "--output")
			outputPath = nextArgument + "/";
		if (argument == "-p" || argument == "--padding")
			padding = std::stoi(nextArgument);
		if (argument == "-R")
			allowRotate = true;
		if (argument == "-T")
			allowTrimming = true;
		if (argument == "-F")
			tryRefillPrevRow = true;
	}
	Packer.setInputPath(importPath);
	Packer.setOutputPath(outputPath);
	Packer.setPadding(padding);
	Packer.setAllowRotate(allowRotate);
	Packer.setAllowTrimming(allowTrimming);
	Packer.setTryRefillPrevRow(tryRefillPrevRow);

	Packer.Packer();

	return 0;
}