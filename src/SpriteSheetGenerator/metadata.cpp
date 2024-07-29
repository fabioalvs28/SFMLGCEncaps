#include "metadata.h"

Metadata::Data Metadata::jsonToMetadataFile(json data)
{
    Metadata::Data metadata;
    unsigned char buffer[] = { 0x53, 0x53, 0x44, 0x47 };
    memcpy(&metadata.header.magic, buffer, 4);
    metadata.header.totalImageCount = data["totalImageCount"];
    metadata.header.texturesCount = data["totalTextureCount"];
    for (size_t i = 0; i < data["totalTextureCount"]; i++)
    {
        Metadata::Texture texture;
        texture.textureName = std::string(data["textures"][i]["textureName"]).c_str();
        texture.texturesize = data["textures"][i]["textureSize"];
        texture.imageCount = data["textures"][i]["imageCount"];
        for (size_t j = 0; j < data["textures"][i]["imageCount"]; j++)
        {
            Metadata::Image image;
            image.filename = std::string(data["textures"][i]["images"][j]["filename"]).c_str();
            image.x = data["textures"][i]["images"][j]["x"];
            image.y = data["textures"][i]["images"][j]["y"];
            image.w = data["textures"][i]["images"][j]["w"];
            image.h = data["textures"][i]["images"][j]["h"];
            image.rotated = data["textures"][i]["images"][j]["rotated"];
            texture.images.emplace_back(image);
        }
        metadata.textures.emplace_back(texture);
    }

    return metadata;
}

bool Metadata::MetadataFileToStruct(GCFile file, Metadata::Data& metadata)
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
    std::memcpy(&metadata.header.magic, buffer.data(), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    
    if (metadata.header.magic != 1195660115)
    {
        std::cerr << "Error : File isn't the right format" << std::endl;
        return false;
    }

    std::memcpy(&metadata.header.totalImageCount, buffer.data() + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);
    std::memcpy(&metadata.header.texturesCount, buffer.data() + offset, sizeof(uint32_t));
    offset += sizeof(uint32_t);

    for (int i = 0; i < metadata.header.texturesCount; i++)
    {
        Metadata::Texture tex;

        std::vector<uint8_t> strBuffer;
        uint8_t lastVar = 0;
        while (strBuffer.size() == 0 || lastVar != 0x00)
        {
            strBuffer.emplace_back(buffer[offset]);
            offset += 1;
            lastVar = strBuffer.back();
        }
        std::memcpy(tex.textureName.data(), strBuffer.data(), strBuffer.size());

        std::memcpy(&tex.texturesize, buffer.data() + offset, sizeof(uint16_t));
        offset += sizeof(uint16_t);

        std::memcpy(&tex.imageCount, buffer.data() + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);

        for (int j = 0; j < tex.imageCount; j++)
        {
            Metadata::Image img;

            std::vector<uint8_t> strBuffer;
            uint8_t lastVar = 0;
            while (strBuffer.size() == 0 || lastVar != 0x00)
            {
                strBuffer.emplace_back(buffer[offset]);
                offset += 1;
                lastVar = strBuffer.back();
            }
            std::memcpy(img.filename.data(), strBuffer.data(), strBuffer.size());

            std::memcpy(&img.x, buffer.data() + offset, sizeof(uint16_t));
            offset += sizeof(uint16_t);
            std::memcpy(&img.y, buffer.data() + offset, sizeof(uint16_t));
            offset += sizeof(uint16_t);
            std::memcpy(&img.w, buffer.data() + offset, sizeof(uint16_t));
            offset += sizeof(uint16_t);
            std::memcpy(&img.h, buffer.data() + offset, sizeof(uint16_t));
            offset += sizeof(uint16_t);

            std::memcpy(&img.rotated, buffer.data() + offset, sizeof(uint8_t));
            offset += sizeof(uint8_t);
            tex.images.emplace_back(img);
        }
        metadata.textures.emplace_back(tex);
    }

    return true;
}