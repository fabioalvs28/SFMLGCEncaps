#include <math.h>
#include <iostream>
#include <fstream>
#include <tuple>
#include <iostream>
#include "nlohmann/json.hpp"
using json = nlohmann::ordered_json;
namespace fs = std::filesystem;

#include "GCImage.h"

struct Sprite
{
    GCImage image;
    int w;
    int h;
    bool rotated;
    std::string filename;
};
struct Pos
{
    Pos(int x, int y) : x(x), y(y) {};
    bool operator==(const Pos& other)
    {
        return x == other.x && y == other.y;
    }
    int x;
    int y;
};
struct Emplacement
{
    Emplacement(int w, int h, Pos pos) : w(w), h(h), pos(pos) {};
    bool operator==(const Emplacement& other)
    {
        return w == other.w && h == other.h && pos == other.pos;
    }
    int w;
    int h;
    Pos pos;
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

static bool sortBySpriteHeight(const Sprite& a, const Sprite& b)
{
    return (a.h > b.h);
}

int main()
{
    bool allowRotate = true;
    bool tryRefillPrevRow = true;
    int padding = 1;
    fs::path importPath = "C:/Users/ssanchez/Downloads/SpriteSheetGenerator-py/randomImage/";
    fs::path outputPath = "C:/Users/ssanchez/Downloads/SpriteSheetGenerator-py/output/";

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
    std::vector<Sprite> imageList;
    std::vector<uint8_t> buffer;
    int totalSize = 0;

    //fileList = sorted_alphanumeric(os.listdir(importPath)); // Sort input maybe 1, 2, 3, 10 insead of 1, 10, 2, 3

    for (const auto& entry : fs::directory_iterator(importPath))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".bmp")
        {
            std::string filename = entry.path().filename().string();
            data["spriteIndex"][filename] = "";
            GCImage image = GCImage(0, 0);
            
            if (!image.LoadBMP(entry.path().string()))
            {
                std::printf("Error opening image\n");
                return 1;
            }

            int h = image.GetHeight();
            int w = image.GetWidth();

            bool rotated = false;
            if (w > h and allowRotate)
            {
                image.Rotate(90);
                int temp = h;
                h = w;
                w = temp;
                rotated = true;
            }
            Sprite sprite = Sprite(image, w, h, rotated, filename); //filename.split('.')[0]
            imageList.emplace_back(sprite);
            totalSize += (w + padding * 2) * (h + padding * 2);
        }
    }

    sort(imageList.begin(), imageList.end(), sortBySpriteHeight);

    data["totalImageCount"] = imageList.size();

    int spriteSheetSize = getSmallestSheetSize(totalSize);
    bool completed = false;
    int textureIndex = 0;
    int startImageIndex = 0;
    GCImage spritSheet;

    while (not completed)
    {
        completed = true;
        std::vector<Emplacement> remainingRows;
        std::vector<int> Ys = { 0 };
        spritSheet = GCImage(spriteSheetSize, spriteSheetSize);
        json elem = {{"textureName", ""}, {"textureSize", spriteSheetSize}, {"imageCount", 0}, {"images", json::array()}};
        data["textures"].push_back(elem);
        int maxX = 0;
        int maxY = 0;
        Pos pos = Pos(0, 0);

        for (int imageIndex = startImageIndex; imageIndex < imageList.size(); imageIndex++)
        {
            GCImage image = imageList[imageIndex].image;
            int w = imageList[imageIndex].w;
            int h = imageList[imageIndex].h;
            std::string filename = imageList[imageIndex].filename;
            bool rotated = imageList[imageIndex].rotated;

            if (tryRefillPrevRow)
            {
                bool foundPrevEmplacement = false;
                for (const auto& emplacement : remainingRows)
                {
                    if (w <= emplacement.w and h <= emplacement.h)
                    {
                        //spritSheet.paste(image, (emplacement[2][0] + padding, emplacement[2][1] + padding))
                        spritSheet.Copy(emplacement.pos.x + padding, emplacement.pos.y + padding, &image, 0, 0, w, h);
                        data["spriteIndex"][filename] = std::to_string((textureIndex, imageIndex));
                        data["textures"][textureIndex]["imageCount"] = data["textures"][textureIndex]["imageCount"] + 1;
                        data["textures"][textureIndex]["images"].push_back({ {"filename", filename},
                                                                             {"x", emplacement.pos.x + padding},
                                                                             {"y", emplacement.pos.y + padding},
                                                                             {"w", w},
                                                                             {"h", h},
                                                                             {"rotated", rotated}});
                        remainingRows.erase(std::find(remainingRows.begin(), remainingRows.end(), emplacement));
                        
                        if (spriteSheetSize - emplacement.pos.x - padding * 2 > 0 and emplacement.pos.x + w + padding * 2 < spriteSheetSize)
                        {
                            remainingRows.emplace_back(Emplacement(spriteSheetSize - emplacement.pos.x - w - padding * 4,
                                                        emplacement.h,
                                                        Pos(emplacement.pos.x + w + padding * 2, emplacement.pos.y)));
                        }
                        foundPrevEmplacement = true;
                        break;
                    }
                }
                if (foundPrevEmplacement)
                    continue;
            }

            if (pos.x + w + padding * 2 > spriteSheetSize)
            {
                int maxYs = *std::max_element(Ys.begin(), Ys.end());
                if (spriteSheetSize - pos.x - padding * 2 > 0)
                {
                    remainingRows.emplace_back(Emplacement(spriteSheetSize - pos.x - padding * 2, maxYs - padding * 2, pos));
                }
                pos = Pos(0, pos.y + maxYs);
                Ys.clear();
            }

            if (pos.y + h + padding * 2 > spriteSheetSize or w > spriteSheetSize)
            {
                completed = false;
                spriteSheetSize *= 2;
                if (spriteSheetSize == 4096)
                {
                    std::string textureName = std::to_string(textureIndex) + ".png";
                    std::string outfilePath = outputPath.string() + textureName;
                    GCFile outfile = GCFile(outfilePath.c_str(), "wb");
                    spritSheet.SavePNG(&outfile);
                    data["textures"][textureIndex]["textureName"] = textureName;
                    textureIndex++;
                    startImageIndex = imageIndex;
                    auto start = imageList.begin() + startImageIndex;
                    auto end = imageList.end();
                    int total = getSizeOfImageList(std::vector(start, end), padding);
                    spriteSheetSize = getSmallestSheetSize(total);
                }
                else
                    data["textures"].erase(data["textures"].size());
                spritSheet = GCImage(spriteSheetSize, spriteSheetSize);
                break;
            }

            //spritSheet.paste(image, (pos[0] + padding, pos[1] + padding))
            spritSheet.Copy(pos.x + padding, pos.y + padding, &image, 0, 0, w, h);
            data["spriteIndex"][filename] = '(' + std::to_string(textureIndex) + ", " + std::to_string(imageIndex) + ')';
            data["textures"][textureIndex]["imageCount"] = data["textures"][textureIndex]["imageCount"] + 1;
            data["textures"][textureIndex]["images"].push_back({{"filename", filename},
                                                                {"x", pos.x + padding},
                                                                {"y", pos.y + padding},
                                                                {"w", w},
                                                                {"h", h},
                                                                {"rotated", rotated}});
            maxX = std::max(maxX, pos.x + w + padding * 2);
            maxY = std::max(maxY, pos.y + h + padding * 2);
            pos = Pos(pos.x + w + padding * 2, pos.y);
            Ys.emplace_back(h + padding * 2);
        }
    }



    data["totalTextureCount"] = textureIndex + 1;
    std::string textureName = std::to_string(textureIndex) + ".png";
    std::string outfilePath = outputPath.string() + textureName;
    GCFile outfile = GCFile(outfilePath.c_str(), "wb");
    spritSheet.SavePNG(&outfile);
    data["textures"][textureIndex]["textureName"] = textureName;

    std::ofstream myfile;
    myfile.open(outputPath.string() + "spritSheetData.json");
    myfile << data.dump(4);
    myfile.close();

    return 0;
}
