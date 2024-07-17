#include "math.h"
#include "GCImage.h"
#include "string"
#include <iostream>
#include "nlohmann/json.hpp"
using json = nlohmann::json;
namespace fs = std::filesystem;

struct Sprite
{
    //GCImage image;
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

static int number_of_files_in_directory(fs::path path)
{
    int count = 0;

    for (auto& p : fs::directory_iterator(path))
    {
        ++count;
    }
    return count;
}


#undef main


int main()
{
    fs::path importPath = "C:\\Users\\ssanchez\\Downloads\\SpriteSheetGenerator-py\\randomImage\\";
    fs::path outputPath = "C:\\Users\\ssanchez\\Downloads\\SpriteSheetGenerator-py\\output\\";

    json data = json::parse(R"({})");
    data["totalImageCount"] = 0;
    data["totalTextureCount"] = 0;
    data["textures"] = json::array();
    data["spriteIndex"] = json::object();

    bool importPathExists = fs::is_directory(importPath.parent_path());
    if (!importPathExists)
    {
        std::printf("Error : Import Path isn't a folder\n");
        return 1;
    }

    bool outputPathExists = fs::exists(outputPath.parent_path());
    if (outputPathExists)
    {
        if (!fs::is_directory(outputPath.parent_path()))
        {
            std::printf("Error : Output Path isn't a folder\n");
            return 1;
        }
    }
    else
    {
        fs::create_directory(outputPath);
    }


    if (number_of_files_in_directory(importPath) == 0)
    {
        std::printf("Error : Input Path is empty\n");
        return 1;
    }
    
    if (number_of_files_in_directory(outputPath) > 0)
    {
        std::printf("Error : Output Path isn't empty\n");
        return 1;
    }

    std::vector<std::string> fileList;

    //fileList = sorted_alphanumeric(os.listdir(importPath));
    for (const auto& entry : fs::directory_iterator(importPath))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".png")
        {
            std::string filename = entry.path().filename().string();
            data["spriteIndex"][filename] = "";
            auto imagefile = GCFile(entry.path().string().c_str(), "r");
            //GCImage::Load(imagefile, sizeof(imagefile))
            /*image = Image.open(importPath + filename)
            w, h = image.size
            rotated = False
            if w > h and allowRotate:
                image = image.rotate(90, expand = True)
                w, h = image.size
                rotated = True
            imageList.append((image.copy(), w, h, filename, rotated)) #filename.split('.')[0]
            totalSize += (w + padding * 2) * (h + padding * 2)*/
        }
    }


    std::vector<Sprite> imageList;
    int totalSize = 0;

    return 0;
}
