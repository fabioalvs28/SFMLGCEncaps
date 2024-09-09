#include "pch.h"
#include "../../src/core/framework.h"

GC_SPRITESHEET_DATA GCSpriteSheetGeometryLoader::LoadSpriteSheets(const char* filename) {

    Metadata::Data* pData = new Metadata::Data{};
    GCFile file(filename, "rb");
    Metadata::MetadataFileToMetadataStruct(file, pData);

    GCGraphicsLogger& logger = GCGraphicsLogger::GetInstance();

    GC_SPRITESHEET_DATA spriteSheetData;

    for (const auto& texture : pData->textures) {

        GC_SPRITESHEET spritesheet;
        spritesheet.size = texture->texturesize;

        CalculateUVs(spritesheet, texture->images);

        spriteSheetData.spritesheets.push_back(spritesheet);
    }
    return spriteSheetData;
}

void GCSpriteSheetGeometryLoader::CalculateUVs(GC_SPRITESHEET& spritesheet, const std::vector<Metadata::Image*>& images) {
    spritesheet.sprites.clear();  
    int index = 0;  

    for (const auto& image : images) {
        GC_SPRITE spriteInfo;

        float u1 = static_cast<float>(image->x) / spritesheet.size;
        float v1 = static_cast<float>(image->y + image->h) / spritesheet.size;

        float u2 = static_cast<float>(image->x) / spritesheet.size;
        float v2 = static_cast<float>(image->y) / spritesheet.size;

        float u3 = static_cast<float>(image->x + image->w) / spritesheet.size;
        float v3 = static_cast<float>(image->y) / spritesheet.size;

        float u4 = static_cast<float>(image->x + image->w) / spritesheet.size;
        float v4 = static_cast<float>(image->y + image->h) / spritesheet.size;

        spriteInfo.uvs[0] = GCVEC2(u1, v1); // Bottom-left
        spriteInfo.uvs[1] = GCVEC2(u2, v2); // Top-left
        spriteInfo.uvs[2] = GCVEC2(u3, v3); // Top-right
        spriteInfo.uvs[3] = GCVEC2(u4, v4); // Bottom-right

        spriteInfo.width = image->w;
        spriteInfo.height = image->h;


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

