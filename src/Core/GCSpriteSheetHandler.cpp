#include "GCSpriteSheetHandler.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include "GCFile.h"
#include "pch.h"

GCSpriteSheetHandler::GCSpriteSheetHandler(const std::string& outputPath, const std::string& entryPath)
    : m_outputPath(outputPath), 
    m_importPath(entryPath),
    m_padding(2), 
    m_allowTrimming(true), 
    m_allowRotate(true), 
    m_tryRefillPrevRow(true)
{
    for (const auto& entry : std::filesystem::directory_iterator(m_outputPath))
        std::filesystem::remove_all(entry.path());

    std::vector<fs::path> paths;
    for (const auto& path : std::filesystem::directory_iterator(entryPath))
        paths.push_back(path);

    packer(paths);
}

int GCSpriteSheetHandler::getSmallestSheetSize(int remainingSize) {
    int size = static_cast<int>(std::sqrt(remainingSize));
    for (int i = 1024; i > 1 ; i /= 2)
        if (size >= i)
            return i * 2;
    return 1;
}

int GCSpriteSheetHandler::getSizeOfImageList(const std::vector<Sprite>& sprites, int padding) {
    int totalSize = 0;
    for (const auto& sprite : sprites) {
        totalSize += (sprite.w + padding * 2) * (sprite.h + padding * 2);
    }
    return totalSize;
}

int GCSpriteSheetHandler::numberOfFilesInDirectory(const fs::path& path) {
    if (!fs::is_directory(path)) {
        std::cerr << "Error: Path is not a directory.\n";
        return 0;
    }
    return std::distance(fs::directory_iterator(path), fs::directory_iterator{});
}

bool GCSpriteSheetHandler::sortBySpriteHeight(const Sprite& a, const Sprite& b) {
    return a.h > b.h;
}

GCImage GCSpriteSheetHandler::trimImage(GCImage image, REC2& outCropRect) {
    // Dummy implementation for trimming
    // This should be replaced with actual logic
    outCropRect = REC2(0, 0, image.GetWidth(), image.GetHeight());
    return image;
}

int GCSpriteSheetHandler::packer(const std::vector<fs::path>& inputPaths,
    int padding,
    bool allowTrimming,
    bool allowRotate,
    bool tryRefillPrevRow) 

{
    bool logging = false;

    m_padding = padding;
    m_allowTrimming = allowTrimming;
    m_allowRotate = allowRotate;
    m_tryRefillPrevRow = tryRefillPrevRow;

    int spritesheetCount = -1;
    nlohmann::json data;
    data["totalImageCount"] = 0;
    data["totalSpritesheetCount"] = 0;
    data["Spritesheets"] = nlohmann::json::array();
    data["spriteIndex"] = nlohmann::json::object();

    for (const auto& dirPath : inputPaths) {

        ++spritesheetCount;

        std::filesystem::path dir(dirPath);
        if (!std::filesystem::is_directory(dir)) {
            std::cerr << "Error: Directory " << dir.string() << " isn't a folder\n";
            continue;
        }

        std::vector<Sprite> imageList;
        int totalSize = 0;

        for (const auto& entry : std::filesystem::directory_iterator(dir)) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                data["spriteIndex"][filename] = "";

                GCImage image = GCImage(0,0);
                if (entry.path().extension() == ".bmp" && !image.LoadBMP(entry.path().string())) {
                    std::cerr << "Error loading BMP image\n";
                    return 1;
                }
                else if (entry.path().extension() == ".png" && !image.LoadPNG(entry.path().string())) {
                    std::cerr << "Error loading PNG image\n";
                    return 1;
                }
                else if (entry.path().extension() == ".dds" && !image.LoadDDS(entry.path().string())) {
                    std::cerr << "Error loading DDS image\n";
                    return 1;
                }

                int original_w = image.GetWidth();
                int original_h = image.GetHeight();

                REC2 outCropRect;
                if (m_allowTrimming) {
                    image = trimImage(image, outCropRect);
                }

                bool usefulCrop = outCropRect != REC2(0, 0, original_w, original_h);
                bool rotated = false;
                if (original_w > original_h && m_allowRotate) {
                    image.Rotate(90);
                    std::swap(original_w, original_h);
                    rotated = true;
                }

                Sprite sprite;
                if (usefulCrop) {
                    sprite = Sprite(image, original_w, original_h, rotated, filename, true, outCropRect);
                    totalSize += (outCropRect.width + m_padding * 2) * (outCropRect.height + m_padding * 2);
                }
                else {
                    sprite = Sprite(image, original_w, original_h, rotated, filename);
                    totalSize += (original_w + m_padding * 2) * (original_h + m_padding * 2);
                }
                imageList.emplace_back(sprite);
            }
        }

        std::sort(imageList.begin(), imageList.end(), sortBySpriteHeight);

        data["totalImageCount"] = data["totalImageCount"] + imageList.size();
        int spriteSheetSize = getSmallestSheetSize(totalSize);
        bool completed = false;
        int startImageIndex = 0;
        GCImage spritSheet;

        while (!completed) {
            completed = true;
            std::vector<Emplacement> remainingRows;
            std::vector<int> Ys = { 0 };
            spritSheet = GCImage(spriteSheetSize, spriteSheetSize);
            data["totalSpritesheetCount"] = data["totalSpritesheetCount"] + 1;
            nlohmann::json elem = { {"Spritesheet_ID", std::to_string(spritesheetCount)}, {"SpritesheetSize", spriteSheetSize}, {"imageCount", 0}, {"images", nlohmann::json::array()} };
            data["Spritesheets"].push_back(elem);
            int maxX = 0;
            int maxY = 0;
            Pos pos = Pos(0, 0);

            data["Spritesheets"][spritesheetCount]["images"] = nlohmann::json::array();

            for (int imageIndex = startImageIndex; imageIndex < imageList.size(); imageIndex++) {

                GCImage image = imageList[imageIndex].image;
                int w = imageList[imageIndex].w;
                int h = imageList[imageIndex].h;
                bool rotated = imageList[imageIndex].rotated;
                std::string filename = imageList[imageIndex].filename;
                bool cropped = imageList[imageIndex].cropped;
                REC2 cropRect = imageList[imageIndex].cropRect;

                if (m_tryRefillPrevRow) {
                    bool foundPrevEmplacement = false;
                    for (const auto& emplacement : remainingRows) {
                        if (w <= emplacement.w && h <= emplacement.h) {
                            spritSheet.Copy(emplacement.pos.x + m_padding, emplacement.pos.y + m_padding, &image, 0, 0, w, h);
                            data["spriteIndex"][filename] = std::to_string(spritesheetCount) + ", " + std::to_string(imageIndex);
                            data["Spritesheets"][spritesheetCount]["imageCount"] = data["Spritesheets"][spritesheetCount]["imageCount"] + 1;
                            data["Spritesheets"][spritesheetCount]["images"].push_back({ {"filename", filename},
                                                                                     {"x", emplacement.pos.x + m_padding},
                                                                                     {"y", emplacement.pos.y + m_padding},
                                                                                     {"w", w},
                                                                                     {"h", h},
                                                                                     {"rotated", rotated},
                                                                                     {"cropped", cropped},
                                                                                     {"crop_x", cropRect.x},
                                                                                     {"crop_y", cropRect.y},
                                                                                     {"original_w", cropRect.width},
                                                                                     {"original_h", cropRect.height} });

                            remainingRows.erase(std::find(remainingRows.begin(), remainingRows.end(), emplacement));

                            if (spriteSheetSize - emplacement.pos.x - m_padding * 2 > 0 && emplacement.pos.x + w + m_padding * 2 < spriteSheetSize) {
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

                if (pos.x + w + m_padding * 2 > spriteSheetSize) {
                    int maxYs = *std::max_element(Ys.begin(), Ys.end());
                    if (spriteSheetSize - pos.x - m_padding * 2 > 0) {
                        remainingRows.emplace_back(Emplacement(spriteSheetSize - pos.x - m_padding * 2, maxYs - m_padding * 2, pos));
                    }
                    pos = Pos(0, maxYs);
                    Ys.clear();
                }

                if ( ( pos.y + h + m_padding * 2 > spriteSheetSize || w > spriteSheetSize ) && spriteSheetSize == 4096 ) {
                    completed = false;
                    if (spriteSheetSize == 4096)
                    {
                        saveSpritesheet(spritSheet, spritesheetCount);
                        spritesheetCount++;
                        nlohmann::json newSpriteSheet = { {"Spritesheet_ID", std::to_string(spritesheetCount)}, {"SpritesheetSize", spriteSheetSize}, {"imageCount", 0}, {"images", nlohmann::json::array()} };
                        data["Spritesheets"].push_back(newSpriteSheet);
                        data["totalSpritesheetCount"] = data["totalSpritesheetCount"] + 1;
                        startImageIndex = imageIndex;
                        auto start = imageList.begin() + startImageIndex;
                        auto end = imageList.end();
                        int total = getSizeOfImageList(std::vector<Sprite>(start, end), m_padding);
                        spriteSheetSize = getSmallestSheetSize(total);
                    }
                    else
                        data["Spritesheets"].erase(data["Spritesheets"].size() - 1);
                    spriteSheetSize *= 2;
                    spritSheet = GCImage(spriteSheetSize, spriteSheetSize);
                    break;
                }

                spritSheet.Copy(pos.x + m_padding, pos.y + m_padding, &image, 0, 0, w, h);
                data["spriteIndex"][filename] = std::to_string(spritesheetCount) + ", " + std::to_string(imageIndex);
                data["Spritesheets"][spritesheetCount]["imageCount"] = data["Spritesheets"][spritesheetCount]["imageCount"] + 1;
                data["Spritesheets"][spritesheetCount]["images"].push_back({{"filename", filename},
                                                                                {"x", pos.x + m_padding},
                                                                                {"y", pos.y + m_padding},
                                                                                {"w", w},
                                                                                {"h", h},
                                                                                {"rotated", rotated},
                                                                                {"cropped", cropped},
                                                                                {"crop_x", cropRect.x},
                                                                                {"crop_y", cropRect.y},
                                                                                {"original_w", cropRect.width},
                                                                                {"original_h", cropRect.height} });
                maxX = max(maxX, pos.x + w + m_padding * 2);
                maxY = max(maxY, pos.y + h + m_padding * 2);
                pos.x += w + m_padding * 2;
                Ys.emplace_back(pos.y + h + m_padding * 2);
            }
            saveSpritesheet(spritSheet, spritesheetCount);
        }   
    }
        std::ofstream outFile(m_outputPath.string() + "/"+ +"spritesheets_data.json");
        outFile << data.dump(4);
        outFile.close();

        Metadata::Data* pData = Metadata::jsonToMetadataStruct(data);

        GCFile metadataFile((m_outputPath.string() + "/SpriteSheetData.ssdg").c_str(), "wb");

        std::vector<uint8_t> buffer;

        buffer.resize(sizeof(Metadata::Header));
        std::memcpy(buffer.data(), &pData->header, buffer.size());
        metadataFile.Write(buffer);

        for (int i = 0; i < pData->textures.size(); i++)
        {
            Metadata::Texture* pTexture = pData->textures[i];
            buffer.resize(pTexture->textureName.length() + 1);
            std::memcpy(buffer.data(), pTexture->textureName.c_str(), buffer.size());
            buffer[pTexture->textureName.length()] = 0x00;
            metadataFile.Write(buffer);

            buffer.resize(sizeof(uint64_t));
            std::memcpy(buffer.data(), &pTexture->texturesize, buffer.size());
            metadataFile.Write(buffer);

            buffer.resize(sizeof(uint64_t));
            std::memcpy(buffer.data(), &pTexture->imageCount, buffer.size());
            metadataFile.Write(buffer);

            for (size_t j = 0; j < pTexture->imageCount; j++)
            {
                Metadata::Image* pImage = pTexture->images[j];

                buffer.resize(pImage->filename.length() + 1 + 8 * sizeof(uint16_t) + 2 * sizeof(uint8_t));
                std::memcpy(buffer.data(), pImage->filename.c_str(), pImage->filename.length());
                buffer[pImage->filename.length()] = 0x00;

                int offset = pImage->filename.length() + 1;
                std::memcpy(buffer.data() + offset, &pImage->x, sizeof(uint16_t));
                offset += sizeof(uint16_t);
                std::memcpy(buffer.data() + offset, &pImage->y, sizeof(uint16_t));
                offset += sizeof(uint16_t);
                std::memcpy(buffer.data() + offset, &pImage->w, sizeof(uint16_t));
                offset += sizeof(uint16_t);
                std::memcpy(buffer.data() + offset, &pImage->h, sizeof(uint16_t));
                offset += sizeof(uint16_t);
                std::memcpy(buffer.data() + offset, &pImage->rotated, sizeof(uint8_t));
                offset += sizeof(uint8_t);
                std::memcpy(buffer.data() + offset, &pImage->cropped, sizeof(uint8_t));
                offset += sizeof(uint8_t);
                std::memcpy(buffer.data() + offset, &pImage->crop_x, sizeof(uint16_t));
                offset += sizeof(uint16_t);
                std::memcpy(buffer.data() + offset, &pImage->crop_y, sizeof(uint16_t));
                offset += sizeof(uint16_t);
                std::memcpy(buffer.data() + offset, &pImage->original_w, sizeof(uint16_t));
                offset += sizeof(uint16_t);
                std::memcpy(buffer.data() + offset, &pImage->original_h, sizeof(uint16_t));
                offset += sizeof(uint16_t);
                metadataFile.Write(buffer);
            }
        }
        metadataFile.Close();

        GCFile metadataFile2 = GCFile((m_outputPath.string() + "/SpriteSheetData.ssdg").c_str(), "rb");
        Metadata::Data* pData2 = new Metadata::Data{};
        Metadata::MetadataFileToMetadataStruct(metadataFile2, pData2);

        if (logging)
        {
            std::cout << "Header Magic : " << pData2->header.magic << std::endl;
            std::cout << "Header Textures Count : " << pData2->header.texturesCount << std::endl;
            std::cout << "Header Total Image Count : " << pData2->header.totalImageCount << std::endl;

            std::cout << std::endl;
        }
        for (size_t i = 0; i < pData2->header.texturesCount; i++)
        {
            Metadata::Texture* tex = pData2->textures[i];
            if (logging)
            {
                std::cout << "Texture " << i << " Texture Name : " << tex->textureName.c_str() << std::endl;
                std::cout << "Texture " << i << " Texture Size : " << tex->texturesize << std::endl;
                std::cout << "Texture " << i << " Image Count : " << tex->imageCount << std::endl;

                std::cout << std::endl;
            }

            for (size_t j = 0; j < tex->imageCount; j++)
            {
                Metadata::Image* img = tex->images[j];
                if (logging)
                {
                    std::cout << "Image " << j << " Image Name : " << img->filename.c_str() << std::endl;
                    std::cout << "Image " << j << " Image X pos : " << img->x << std::endl;
                    std::cout << "Image " << j << " Image Y pos : " << img->y << std::endl;
                    std::cout << "Image " << j << " Image Width : " << img->w << std::endl;
                    std::cout << "Image " << j << " Image Height : " << img->h << std::endl;
                    std::cout << "Image " << j << " Rotated : " << bool(img->rotated) << std::endl;
                    std::cout << "Image " << j << " Image Crop X pos : " << img->crop_x << std::endl;
                    std::cout << "Image " << j << " Image Crop Y pos : " << img->crop_y << std::endl;
                    std::cout << "Image " << j << " Image Original Width : " << img->original_w << std::endl;
                    std::cout << "Image " << j << " Image Original Height : " << img->original_h << std::endl;
                    std::cout << "Image " << j << " Cropped : " << bool(img->cropped) << std::endl;
                    std::cout << std::endl;
                }
            }
        }

    return 0;
}

// Implementation details for getSmallestSheetSize, getSizeOfImageList, trimImage, and saveSpritesheet should be added here.
#include "GCSpriteSheetHandler.h"

// Function to save spritesheet image to file
void GCSpriteSheetHandler::saveSpritesheet(GCImage& image, int index) {

    std::string filename = "spritesheet_" + std::to_string(index);

    // Create a full path with output directory and file extension
    fs::path fullPath;

    SaveFormat saveFormat = DDS;
    switch (saveFormat)
    {
    case GCSpriteSheetHandler::BMP:
        fullPath = m_outputPath / (filename + ".bmp ");
        break;
    case GCSpriteSheetHandler::PNG:
        fullPath = m_outputPath / (filename + ".png");
        break;
    case GCSpriteSheetHandler::DDS:
        fullPath = m_outputPath / (filename + ".dds");
        break;
    default:
        break;
    }

    GCFile outFile = GCFile(fullPath.string().c_str(), "wb");
    switch (saveFormat)
    {
    case GCSpriteSheetHandler::BMP:
        image.SaveBMP(&outFile);
        break;
    case GCSpriteSheetHandler::PNG:
        image.SavePNG(&outFile);
        break;
    case GCSpriteSheetHandler::DDS:
        image.SaveDDS(&outFile);
        break;
    default:
        break;
    }
    
}

