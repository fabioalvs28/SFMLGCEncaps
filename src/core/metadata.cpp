#include "pch.h"
#include "metadata.h"

Metadata::Data* Metadata::jsonToMetadataStruct(json data)
{
    Metadata::Data* pData = new Metadata::Data{};
    unsigned char buffer[] = { 0x53, 0x53, 0x44, 0x47 };
    memcpy(&pData->header.magic, buffer, 4);
    pData->header.totalImageCount = data["totalImageCount"];
    pData->header.texturesCount = data["totalSpritesheetCount"];
    pData->textures = std::vector<Metadata::Texture*>(pData->header.texturesCount);

    for (size_t i = 0; i < data["totalSpritesheetCount"]; i++)
    {
        Metadata::Texture* pTexture = new Metadata::Texture();
        pTexture->textureName = std::string(data["Spritesheets"][i]["Spritesheet_ID"]).c_str();
        pTexture->texturesize = data["Spritesheets"][i]["SpritesheetSize"];
        pTexture->imageCount = data["Spritesheets"][i]["imageCount"];
        pTexture->images = std::vector<Metadata::Image*>(pTexture->imageCount);

        for (size_t j = 0; j < data["Spritesheets"][i]["imageCount"]; j++)
        {
            Metadata::Image* pImage = new Metadata::Image();
            pImage->filename = std::string(data["Spritesheets"][i]["images"][j]["filename"]);
            pImage->x = data["Spritesheets"][i]["images"][j]["x"];
            pImage->y = data["Spritesheets"][i]["images"][j]["y"];
            pImage->w = data["Spritesheets"][i]["images"][j]["w"];
            pImage->h = data["Spritesheets"][i]["images"][j]["h"];
            pImage->rotated = data["Spritesheets"][i]["images"][j]["rotated"];
            pImage->crop_x = data["Spritesheets"][i]["images"][j]["crop_x"];
            pImage->crop_y = data["Spritesheets"][i]["images"][j]["crop_y"];
            pImage->original_w = data["Spritesheets"][i]["images"][j]["original_w"];
            pImage->original_h = data["Spritesheets"][i]["images"][j]["original_h"];
            pImage->cropped = data["Spritesheets"][i]["images"][j]["cropped"];
            pTexture->images[j] = pImage;
        }
        pData->textures[i] = pTexture;
    }

    return pData;
}

bool Metadata::MetadataFileToMetadataStruct(GCFile file, Metadata::Data* pMetadata)
{
    size_t offset = 0;

    if (file.mode != "rb")
    {
        std::cerr << "Error : File must be open in ReadBinary mode" << std::endl;
        return false;
    }
    std::vector<uint8_t> buffer(file.size);
    if (!file.Read(buffer, file.size))
    {
        std::cerr << "Error reading metadata file" << std::endl;
        return false;
    }
    std::memcpy(&pMetadata->header.magic, buffer.data(), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    
    if (pMetadata->header.magic != 1195660115)
    {
        std::cerr << "Error : File isn't the right format" << std::endl;
        return false;
    }

    std::memcpy(&pMetadata->header.totalImageCount, buffer.data() + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    std::memcpy(&pMetadata->header.texturesCount, buffer.data() + offset, sizeof(uint64_t));
    offset += sizeof(uint64_t);

    pMetadata->textures = std::vector<Metadata::Texture*>(pMetadata->header.texturesCount);

    for (uint64_t i = 0; i < pMetadata->header.texturesCount; i++)
    {
        Metadata::Texture* pTexture = new Metadata::Texture();

        std::vector<uint8_t> strBuffer;
        uint8_t lastVar = 0;
        while (strBuffer.size() == 0 || lastVar != 0x00)
        {
            strBuffer.emplace_back(buffer[offset]);
            offset += 1;
            lastVar = strBuffer.back();
        }
        std::memcpy(pTexture->textureName.data(), strBuffer.data(), strBuffer.size());

        std::memcpy(&pTexture->texturesize, buffer.data() + offset, sizeof(uint64_t));
        offset += sizeof(uint64_t);

        std::memcpy(&pTexture->imageCount, buffer.data() + offset, sizeof(uint64_t));
        offset += sizeof(uint64_t);

        for (uint64_t j = 0; j < pTexture->imageCount; j++)
        {
            Metadata::Image* pImage = new Metadata::Image();

            std::vector<uint8_t> strBuffer;
            uint8_t lastVar = 0;
            while (strBuffer.size() == 0 || lastVar != 0x00)
            {
                strBuffer.emplace_back(buffer[offset]);
                offset += 1;
                lastVar = strBuffer.back();
            }
            std::memcpy(pImage->filename.data(), strBuffer.data(), strBuffer.size());

            std::memcpy(&pImage->x, buffer.data() + offset, sizeof(uint16_t));
            offset += sizeof(uint16_t);
            std::memcpy(&pImage->y, buffer.data() + offset, sizeof(uint16_t));
            offset += sizeof(uint16_t);
            std::memcpy(&pImage->w, buffer.data() + offset, sizeof(uint16_t));
            offset += sizeof(uint16_t);
            std::memcpy(&pImage->h, buffer.data() + offset, sizeof(uint16_t));
            offset += sizeof(uint16_t);
            std::memcpy(&pImage->rotated, buffer.data() + offset, sizeof(uint8_t));
            offset += sizeof(uint8_t);

            std::memcpy(&pImage->cropped, buffer.data() + offset, sizeof(uint8_t));
            offset += sizeof(uint8_t);
            std::memcpy(&pImage->crop_x, buffer.data() + offset, sizeof(uint16_t));
            offset += sizeof(uint16_t);
            std::memcpy(&pImage->crop_y, buffer.data() + offset, sizeof(uint16_t));
            offset += sizeof(uint16_t);
            std::memcpy(&pImage->original_w, buffer.data() + offset, sizeof(uint16_t));
            offset += sizeof(uint16_t);
            std::memcpy(&pImage->original_h, buffer.data() + offset, sizeof(uint16_t));
            offset += sizeof(uint16_t);

            pTexture->images.emplace_back(pImage);
        }
        pMetadata->textures[i] = pTexture;
    }

    return true;
}
