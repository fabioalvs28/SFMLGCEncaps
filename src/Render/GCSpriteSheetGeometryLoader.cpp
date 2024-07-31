#include "pch.h"

GC_SPRITESHEET_DATA GCSpriteSheetGeometryLoader::LoadSpriteSheet(const char* filename) {

    Metadata::Data data;
    GCFile file(filename, "rb");
    Metadata::MetadataFileToStruct(file, data);

    GCGraphicsLogger& logger = GCGraphicsLogger::GetInstance();

    GC_SPRITESHEET_DATA spriteSheetData;

    for (const auto& texture : data.textures) {

        GC_SPRITESHEET spritesheet;
        spritesheet.size = texture.texturesize;

        CalculateUVs(spritesheet, texture.images);

        spriteSheetData.spritesheets.push_back(spritesheet);
    }
    return spriteSheetData;
}

void GCSpriteSheetGeometryLoader::CalculateUVs(GC_SPRITESHEET& spritesheet, const std::vector<Metadata::Image>& images) {
    spritesheet.sprites.clear();  
    int index = 0;  

    for (const auto& image : images) {
        GC_SPRITE spriteInfo;

        float u1 = static_cast<float>(image.x) / spritesheet.size;
        float v1 = static_cast<float>(image.y + image.h) / spritesheet.size;

        float u2 = static_cast<float>(image.x) / spritesheet.size;
        float v2 = static_cast<float>(image.y) / spritesheet.size;

        float u3 = static_cast<float>(image.x + image.w) / spritesheet.size;
        float v3 = static_cast<float>(image.y) / spritesheet.size;

        float u4 = static_cast<float>(image.x + image.w) / spritesheet.size;
        float v4 = static_cast<float>(image.y + image.h) / spritesheet.size;

        spriteInfo.uvs[0] = GCVEC2(u1, v1); // Bottom-left
        spriteInfo.uvs[1] = GCVEC2(u2, v2); // Top-left
        spriteInfo.uvs[2] = GCVEC2(u3, v3); // Top-right
        spriteInfo.uvs[3] = GCVEC2(u4, v4); // Bottom-right

        spriteInfo.width = image.w;
        spriteInfo.height = image.h;


        spritesheet.sprites[index++] = spriteInfo;
    }
}

// Utils function to set sprite sheet info uv on 4 vertex plane
void GCSpriteSheetGeometryLoader::SetSpriteUVs(GCGeometry* geometry, int spriteSheetID, int spriteID, const GC_SPRITESHEET_DATA& spriteSheetInfo) {

    // Get wanted texture in spritesheet vector info
    const GC_SPRITESHEET spriteSheet = spriteSheetInfo.spritesheets[spriteSheetID];

    // Search and find sprite
    auto it = spriteSheet.sprites.find(spriteID);
    if (it != spriteSheet.sprites.end()) {
        const GC_SPRITE& spriteInfo = it->second;

        // Update uv
        geometry->uv.resize(4);
        for (int i = 0; i < 4; ++i) {
            geometry->uv[i] = DirectX::XMFLOAT2(spriteInfo.uvs[i].x, spriteInfo.uvs[i].y);
        }
    }
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