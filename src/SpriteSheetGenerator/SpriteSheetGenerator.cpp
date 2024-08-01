#include "SpriteSheetGenerator.h"

int SpriteSheetGenerator::getSmallestSheetSize(int remainingSize)
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


int SpriteSheetGenerator::getSizeOfImageList(std::vector<Sprite> Sprites, int padding)
{
    int total = 0;
    for (Sprite& sprite : Sprites)
    {
        total += (sprite.w + padding * 2) * (sprite.h + padding * 2);
    }
    return total;
}

int SpriteSheetGenerator::number_of_files_in_directory(fs::path path)
{
    int count = 0;

    for (auto& p : fs::directory_iterator(path))
    {
        ++count;
    }
    return count;
}

bool SpriteSheetGenerator::sortBySpriteHeight(const Sprite& a, const Sprite& b)
{
    return (a.h > b.h);
}

GCImage SpriteSheetGenerator::trimImage(GCImage image)
{
    int width = image.GetWidth();
    int height = image.GetHeight();
    int top = height;
    int bottom = 0;
    int left = width;
    int right = 0;
    for (int x = 0; x < width; x++) 
    {
        for (int y = 0; y < height; y++) 
        {
            if (image.GetPixelRGBA(x, y) != 0) 
            {
                top = std::min(top, y);
                bottom = std::max(bottom, y);
                left = std::min(left, x);
                right = std::max(right, x);
            }
        }
    }
    GCImage croppedImage(right - left + 1, bottom - top + 1);
    croppedImage.Copy(0, 0, &image, left, top, right - left + 1, bottom - top + 1);
    return croppedImage;
}

int SpriteSheetGenerator::Packer()
{
    bool logging = false;

    SaveFormat saveFormat = SaveFormat::DDS;

    json data = json::parse(R"({})");
    data["totalImageCount"] = 0;
    data["totalTextureCount"] = 0;
    data["textures"] = json::array();
    data["spriteIndex"] = json::object();

    bool importPathExists = fs::is_directory(m_importPath.parent_path());
    if (!importPathExists)
    {
        std::printf("Error : Import Path isn't a folder\n");
        return 1;
    }

    bool outputPathExists = fs::exists(m_outputPath.parent_path());
    if (outputPathExists)
    {
        if (!fs::is_directory(m_outputPath.parent_path()))
        {
            std::printf("Error : Output Path isn't a folder\n");
            return 1;
        }
        fs::remove_all(m_outputPath);
    }
    fs::create_directory(m_outputPath);


    if (number_of_files_in_directory(m_importPath) == 0)
    {
        std::printf("Error : Input Path is empty\n");
        return 1;
    }
    
    if (number_of_files_in_directory(m_outputPath) > 0)
    {
        std::printf("Error : Output Path isn't empty\n");
        //return 1;
    }

    std::vector<std::string> fileList;
    std::vector<Sprite> imageList;
    int totalSize = 0;

    //fileList = sorted_alphanumeric(os.listdir(importPath)); // Sort input maybe 1, 2, 3, 10 insead of 1, 10, 2, 3

    for (const auto& entry : fs::directory_iterator(m_importPath))
    {
        if (entry.is_regular_file())
        {
            std::string filename = entry.path().filename().string(); 
            data["spriteIndex"][filename] = ""; 
            GCImage image = GCImage(0, 0); 
            if (entry.path().extension() == ".bmp")
            {
                if (!image.LoadBMP(entry.path().string()))
                {
                    std::printf("Error loading image\n");
                    return 1;
                }
            }

            if (entry.path().extension() == ".png")
            {
                if (!image.LoadPNG(entry.path().string()))
                {
                    std::printf("Error loading image\n");
                    return 1;
                }
            }

            if (entry.path().extension() == ".dds")
            {
                if (!image.LoadDDS(entry.path().string()))
                {
                    std::printf("Error loading image\n");
                    return 1;
                }
            }

            if (m_allowTrimming)
            {
                image = trimImage(image);
            }

            int h = image.GetHeight();
            int w = image.GetWidth();

            bool rotated = false;
            if (w > h and m_allowRotate)
            {
                image.Rotate(90);
                int temp = h;
                h = w;
                w = temp;
                rotated = true;
            }
            Sprite sprite = Sprite(image, w, h, rotated, filename.substr(0, filename.find('.')));
            imageList.emplace_back(sprite);
            totalSize += (w + m_padding * 2) * (h + m_padding * 2);
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

            if (m_tryRefillPrevRow) // This part is very upgradable, trying to put only the largest image that fit in a space, remembering vertical size too, ect
            {
                bool foundPrevEmplacement = false;
                for (const auto& emplacement : remainingRows)
                {
                    if (w <= emplacement.w and h <= emplacement.h)
                    {
                        //spritSheet.paste(image, (emplacement[2][0] + padding, emplacement[2][1] + padding))
                        spritSheet.Copy(emplacement.pos.x + m_padding, emplacement.pos.y + m_padding, &image, 0, 0, w, h);
                        data["spriteIndex"][filename] = std::to_string((textureIndex, imageIndex));
                        data["textures"][textureIndex]["imageCount"] = data["textures"][textureIndex]["imageCount"] + 1;
                        data["textures"][textureIndex]["images"].push_back({ {"filename", filename},
                                                                             {"x", emplacement.pos.x + m_padding},
                                                                             {"y", emplacement.pos.y + m_padding},
                                                                             {"w", w},
                                                                             {"h", h},
                                                                             {"rotated", rotated}});
                        remainingRows.erase(std::find(remainingRows.begin(), remainingRows.end(), emplacement));
                        
                        if (spriteSheetSize - emplacement.pos.x - m_padding * 2 > 0 and emplacement.pos.x + w + m_padding * 2 < spriteSheetSize)
                        {
                            remainingRows.emplace_back(Emplacement(spriteSheetSize - emplacement.pos.x - w - m_padding * 4,
                                                        emplacement.h,
                                                        Pos(emplacement.pos.x + w + m_padding * 2, emplacement.pos.y)));
                        }
                        foundPrevEmplacement = true;
                        break;
                    }
                }
                if (foundPrevEmplacement)
                    continue;
            }

            if (pos.x + w + m_padding * 2 > spriteSheetSize)
            {
                int maxYs = *std::max_element(Ys.begin(), Ys.end());
                if (spriteSheetSize - pos.x - m_padding * 2 > 0)
                {
                    remainingRows.emplace_back(Emplacement(spriteSheetSize - pos.x - m_padding * 2, maxYs - m_padding * 2, pos));
                }
                pos = Pos(0, pos.y + maxYs);
                Ys.clear();
            }

            if (pos.y + h + m_padding * 2 > spriteSheetSize or w > spriteSheetSize)
            {
                completed = false;
                spriteSheetSize *= 2;
                if (spriteSheetSize == 4096)
                {
                    std::string textureName;
                    std::string outfilePath;
                    switch (saveFormat)
                    {
                    case SpriteSheetGenerator::BMP:
                        outfilePath = m_outputPath.string() + std::to_string(textureIndex) + ".bmp";
                        break;
                    case SpriteSheetGenerator::PNG:
                        outfilePath = m_outputPath.string() + std::to_string(textureIndex) + ".png";
                        break;
                    case SpriteSheetGenerator::DDS:
                        outfilePath = m_outputPath.string() + std::to_string(textureIndex) + ".dds";
                        break;
                    default:
                        break;
                    }
                    GCFile outfile = GCFile(outfilePath.c_str(), "wb");
                    switch (saveFormat)
                    {
                    case SpriteSheetGenerator::BMP:
                        spritSheet.SaveBMP(&outfile);
                        break;
                    case SpriteSheetGenerator::PNG:
                        spritSheet.SavePNG(&outfile);
                        break;
                    case SpriteSheetGenerator::DDS:
                        spritSheet.SaveDDS(&outfile);
                        break;
                    default:
                        break;
                    }
                    data["textures"][textureIndex]["textureName"] = std::to_string(textureIndex);
                    textureIndex++;
                    startImageIndex = imageIndex;
                    auto start = imageList.begin() + startImageIndex;
                    auto end = imageList.end();
                    int total = getSizeOfImageList(std::vector(start, end), m_padding);
                    spriteSheetSize = getSmallestSheetSize(total);
                }
                else
                    data["textures"].erase(data["textures"].size() - 1);
                spritSheet = GCImage(spriteSheetSize, spriteSheetSize);
                break;
            }

            //spritSheet.paste(image, (pos[0] + padding, pos[1] + padding))
            spritSheet.Copy(pos.x + m_padding, pos.y + m_padding, &image, 0, 0, w, h);
            data["spriteIndex"][filename] = '(' + std::to_string(textureIndex) + ", " + std::to_string(imageIndex) + ')';
            data["textures"][textureIndex]["imageCount"] = data["textures"][textureIndex]["imageCount"] + 1;
            data["textures"][textureIndex]["images"].push_back({{"filename", filename},
                                                                {"x", pos.x + m_padding},
                                                                {"y", pos.y + m_padding},
                                                                {"w", w},
                                                                {"h", h},
                                                                {"rotated", rotated}});
            maxX = std::max(maxX, pos.x + w + m_padding * 2);
            maxY = std::max(maxY, pos.y + h + m_padding * 2);
            pos = Pos(pos.x + w + m_padding * 2, pos.y);
            Ys.emplace_back(h + m_padding * 2);
        }
    }

    data["totalTextureCount"] = textureIndex + 1;
    std::string textureName;
    std::string outfilePath;
    switch (saveFormat)
    {
    case SpriteSheetGenerator::BMP:
        outfilePath = m_outputPath.string() + std::to_string(textureIndex) + ".bmp";
        break;
    case SpriteSheetGenerator::PNG:
        outfilePath = m_outputPath.string() + std::to_string(textureIndex) + ".png";
        break;
    case SpriteSheetGenerator::DDS:
        outfilePath = m_outputPath.string() + std::to_string(textureIndex) + ".dds";
        break;
    default:
        break;
    }
    GCFile outfile = GCFile(outfilePath.c_str(), "wb");
    switch (saveFormat)
    {
    case SpriteSheetGenerator::BMP:
        spritSheet.SaveBMP(&outfile);
        break;
    case SpriteSheetGenerator::PNG:
        spritSheet.SavePNG(&outfile);
        break;
    case SpriteSheetGenerator::DDS:
        spritSheet.SaveDDS(&outfile);
        break;
    default:
        break;
    }
    data["textures"][textureIndex]["textureName"] = std::to_string(textureIndex);

    std::ofstream jsonFile;
    jsonFile.open(m_outputPath.string() + "spritSheetData.json");
    jsonFile << data.dump(4);
    jsonFile.close();


    Metadata::Data metadata = Metadata::jsonToMetadataFile(data);

    GCFile metadataFile((m_outputPath.string() + "spritSheetData.ssdg").c_str(), "wb");

    std::vector<uint8_t> buffer;

    buffer.resize(sizeof(Metadata::Header));
    std::memcpy(buffer.data(), &metadata.header, buffer.size());
    metadataFile.Write(buffer);

    for (int i = 0; i < metadata.textures.size(); i++)
    {
        Metadata::Texture tex = metadata.textures[i];
        buffer.resize(tex.textureName.length() + 1);
        std::memcpy(buffer.data(), tex.textureName.c_str(), buffer.size());
        buffer[tex.textureName.length()] = 0x00;
        metadataFile.Write(buffer);

        buffer.resize(sizeof(uint16_t));
        std::memcpy(buffer.data(), &tex.texturesize, buffer.size());
        metadataFile.Write(buffer);

        buffer.resize(sizeof(uint32_t));
        std::memcpy(buffer.data(), &tex.imageCount, buffer.size());
        metadataFile.Write(buffer);

        for (size_t j = 0; j < tex.imageCount; j++)
        {
            Metadata::Image img = tex.images[j];

            buffer.resize(img.filename.length() + 1 + 4 * sizeof(uint16_t) + sizeof(uint8_t));
            std::memcpy(buffer.data(), img.filename.c_str(), img.filename.length());
            buffer[img.filename.length()] = 0x00;
            std::memcpy(buffer.data() + img.filename.length() + 1 + 0 * sizeof(uint16_t), &img.x, sizeof(uint16_t));
            std::memcpy(buffer.data() + img.filename.length() + 1 + 1 * sizeof(uint16_t), &img.y, sizeof(uint16_t));
            std::memcpy(buffer.data() + img.filename.length() + 1 + 2 * sizeof(uint16_t), &img.w, sizeof(uint16_t));
            std::memcpy(buffer.data() + img.filename.length() + 1 + 3 * sizeof(uint16_t), &img.h, sizeof(uint16_t));
            std::memcpy(buffer.data() + img.filename.length() + 1 + 4 * sizeof(uint16_t), &img.rotated, sizeof(uint8_t));
            metadataFile.Write(buffer);
        }
    }
    metadataFile.Close();

    GCFile metadataFile2 = GCFile((m_outputPath.string() + "spritSheetData.ssdg").c_str(), "rb");
    Metadata::Data metadata2;
    Metadata::MetadataFileToStruct(metadataFile2, metadata2);

    if (logging)
    {
        std::cout << "Header Magic : " << metadata2.header.magic << std::endl;
        std::cout << "Header Textures Count : " << metadata2.header.texturesCount << std::endl;
        std::cout << "Header Total Image Count : " << metadata2.header.totalImageCount << std::endl;

        std::cout << std::endl;
    }
    for (size_t i = 0; i < metadata2.header.texturesCount; i++)
    {
        Metadata::Texture tex = metadata2.textures[i];
        if (logging)
        {
            std::cout << "Texture " << i << " Texture Name : " << tex.textureName.c_str() << std::endl;
            std::cout << "Texture " << i << " Texture Size : " << tex.texturesize << std::endl;
            std::cout << "Texture " << i << " Image Count : " << tex.imageCount << std::endl;

            std::cout << std::endl;
        }

        for (size_t j = 0; j < tex.imageCount; j++)
        {
            Metadata::Image img = tex.images[j];
            if (logging)
            {
                std::cout << "Image " << j << " Image Name : " << img.filename.c_str() << std::endl;
                std::cout << "Image " << j << " Image X pos : " << img.x << std::endl;
                std::cout << "Image " << j << " Image Y pos : " << img.y << std::endl;
                std::cout << "Image " << j << " Image Width : " << img.w << std::endl;
                std::cout << "Image " << j << " Image Height : " << img.h << std::endl;
                std::cout << "Image " << j << " Rotated : " << bool(img.rotated) << std::endl;
                std::cout << std::endl;
            }
        }
    }

    return 0;
}
