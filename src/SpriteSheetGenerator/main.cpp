#include "math.h"
#include "GCImage.h"
#include "GCFile.h"
#include "string"
#include "nlohmann/json.hpp"
using json = nlohmann::json;

struct Sprite
{
    GCImage image;
    int x;
    int y;
    int w;
    int h;
    bool rotated;
    std::string name;
};

static int getSmallestSheetSize(int remainingSize)
{
    int sqrtTotal = sqrt(remainingSize);
    int spriteSheetSize;
    if (sqrtTotal >= 1024)
        spriteSheetSize = 2048;
    else if (sqrtTotal >= 512)
        spriteSheetSize = 1024;
    else if (sqrtTotal >= 256)
        spriteSheetSize = 512;
    else if (sqrtTotal >= 128)
        spriteSheetSize = 256;
    else if (sqrtTotal >= 64)
        spriteSheetSize = 128;
    else if (sqrtTotal >= 32)
        spriteSheetSize = 64;
    else if (sqrtTotal >= 16)
        spriteSheetSize = 32;
    else if (sqrtTotal >= 8)
        spriteSheetSize = 16;
    else if (sqrtTotal >= 4)
        spriteSheetSize = 8;
    else if (sqrtTotal >= 2)
        spriteSheetSize = 4;
    else if (sqrtTotal >= 1)
        spriteSheetSize = 2;
    else
        spriteSheetSize = 1;
    return spriteSheetSize;
}


static int getSizeOfImageList(std::vector<Sprite> Sprites, int padding)
{
    int total = 0;
    for (Sprite& sprite : Sprites)
    {
        total += (sprite.w + padding * 2) * (sprite.h + padding * 2);
    }
    return total;
}

int number_of_files_in_directory(std::filesystem::path path)
{
    int count = 0;

    for (auto& p : std::filesystem::directory_iterator(path))
    {
        ++count;
    }
    return count;
}

int main()
{
    std::filesystem::path importPath = "C:\\Users\\ssanchez\\Downloads\\SpriteSheetGenerator-py\\randomImage\\";
    std::filesystem::path outputPath = "C:\\Users\\ssanchez\\Downloads\\SpriteSheetGenerator-py\\output\\";

    json data = json::parse(R"({})");
    data["totalImageCount"] = 0;
    data["totalTextureCount"] = 0;
    data["textures"] = json::array();
    data["spriteIndex"] = json::object();

    bool importPathExists = std::filesystem::is_directory(importPath.parent_path());
    if (!importPathExists)
    {
        std::printf("Error : Import Path isn't a folder\n");
        //sys.exit(1);
    }

    bool outputPathExists = std::filesystem::exists(outputPath.parent_path());
    if (outputPathExists)
    {
        if (!std::filesystem::is_directory(outputPath.parent_path()))
        {
            std::printf("Error : Output Path isn't a folder\n");
            //sys.exit(1);
        }
    }
    else
    {
        std::filesystem::create_directory(outputPath);
    }

    
    if (number_of_files_in_directory(outputPath) > 0)
    {
        std::printf("Error : Output Path isn't empty\n");
        //sys.exit(1);
    }

    //fileList = sorted_alphanumeric(os.listdir(importPath));
    
    std::vector<Sprite> imageList;
    int totalSize = 0;

    return 1;
}
