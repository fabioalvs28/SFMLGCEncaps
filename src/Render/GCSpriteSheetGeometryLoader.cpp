#include "pch.h"

GC_SPRITESHEET_INFO GCSpriteSheetGeometryLoader::LoadSpriteSheet(int rowNumber, int colNumber, int sheetWidth, int sheetHeight) {
    float spriteWidth = static_cast<float>(sheetWidth) / colNumber;
    float spriteHeight = static_cast<float>(sheetHeight) / rowNumber;

    GC_SPRITESHEET_INFO info;
    info.isFlip = false; 
    info.width = sheetWidth;
    info.height = sheetHeight;

    CalculateUVs(spriteWidth, spriteHeight, sheetWidth, sheetHeight, info);

    return info;
}

void GCSpriteSheetGeometryLoader::CalculateUVs(int spriteWidth, int spriteHeight, int sheetWidth, int sheetHeight, GC_SPRITESHEET_INFO& info) {
    info.spriteUVMap.clear();

    int numSpritesX = sheetWidth / spriteWidth;
    int numSpritesY = sheetHeight / spriteHeight;

    for (int y = 0; y < numSpritesY; ++y) {
        for (int x = 0; x < numSpritesX; ++x) {
            int index = y * numSpritesX + x; // Calculer l'index du sprite

            float u1 = static_cast<float>(x * spriteWidth) / sheetWidth;
            float v1 = static_cast<float>((y + 1) * spriteHeight) / sheetHeight;

            float u2 = static_cast<float>(x * spriteWidth) / sheetWidth;
            float v2 = static_cast<float>(y * spriteHeight) / sheetHeight;

            float u3 = static_cast<float>((x + 1) * spriteWidth) / sheetWidth;
            float v3 = static_cast<float>(y * spriteHeight) / sheetHeight;

            float u4 = static_cast<float>((x + 1) * spriteWidth) / sheetWidth;
            float v4 = static_cast<float>((y + 1) * spriteHeight) / sheetHeight;

            GC_SPRITE_INFO spriteInfo;
            spriteInfo.uvs[0] = GCVEC2(u1, v1); // Bottom-left
            spriteInfo.uvs[1] = GCVEC2(u2, v2); // Top-left
            spriteInfo.uvs[2] = GCVEC2(u3, v3); // Top-right
            spriteInfo.uvs[3] = GCVEC2(u4, v4); // Bottom-right

            spriteInfo.width = static_cast<int>(spriteWidth);
            spriteInfo.height = static_cast<int>(spriteHeight);

            info.spriteUVMap[index] = spriteInfo; 
        }
    }
}

// Utils function to set sprite sheet info uv on 4 vertex plane
void GCSpriteSheetGeometryLoader::SetSpriteUVs(GCGeometry* geometry, int spriteID, const GC_SPRITESHEET_INFO& spriteSheetInfo) {
    auto it = spriteSheetInfo.spriteUVMap.find(spriteID);

    if (it != spriteSheetInfo.spriteUVMap.end()) {
        const GC_SPRITE_INFO& spriteInfo = it->second;

        geometry->uv.resize(4);

        for (int i = 0; i < 4; ++i) {
            geometry->uv[i] = DirectX::XMFLOAT2(spriteInfo.uvs[i].x, spriteInfo.uvs[i].y);
        }
    }
}