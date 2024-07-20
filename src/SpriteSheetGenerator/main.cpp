#include "SpriteSheetGenerator.h"

int main()
{
	SpriteSheetGenerator Packer;
	fs::path importPath = "C:/Users/swann/source/repos/gce-dev/bin/SpriteSheetGenerator-py/randomImage/";
	fs::path outputPath = "C:/Users/swann/source/repos/gce-dev/bin/SpriteSheetGenerator-py/output/";

	Packer.setInputPath(importPath);
	Packer.setOutputPath(outputPath);
	Packer.setPadding(1);
	Packer.setAllowRotate(true);
	Packer.setTryRefillPrevRow(true);

	Packer.Packer();

	return 0;
}