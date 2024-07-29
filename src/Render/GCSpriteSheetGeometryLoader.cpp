#include "pch.h"

std::vector<GCGeometry*> GCSpriteSheetGeometryLoader::LoadSpriteSheet(int rowNumber, int colNumber, int sheetWidth, int sheetHeight) {

    float spriteWidth = static_cast<float>(sheetWidth) / colNumber;
    float spriteHeight = static_cast<float>(sheetHeight) / rowNumber;

    CalculateUVs(spriteWidth, spriteHeight, sheetWidth, sheetHeight);

    return m_geometries;
}
void GCSpriteSheetGeometryLoader::CalculateUVs(int spriteWidth, int spriteHeight, int sheetWidth, int sheetHeight) {
    m_geometries.clear();


    int numSpritesX = sheetWidth / spriteWidth;
    int numSpritesY = sheetHeight / spriteHeight;

    for (int y = 0; y < numSpritesY; ++y) {
        for (int x = 0; x < numSpritesX; ++x) {
            GCGeometry* geometry = new GCGeometry();
            m_geometries.push_back(geometry);

            

            float u1 = static_cast<float>(x * spriteWidth) / sheetWidth;
            float v1 = static_cast<float>((y + 1) * spriteHeight) / sheetHeight;


            float u2 = static_cast<float>(x * spriteWidth) / sheetWidth;
            float v2 = static_cast<float>(y * spriteHeight) / sheetHeight;

            float u3 = static_cast<float>((x + 1) * spriteWidth) / sheetWidth;
            float v3 = static_cast<float>(y * spriteHeight) / sheetHeight;

            float u4 = static_cast<float>((x + 1) * spriteWidth) / sheetWidth;
            float v4 = static_cast<float>((y + 1) * spriteHeight) / sheetHeight;

            geometry->pos.push_back(DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f)); // Bottom-left
            geometry->pos.push_back(DirectX::XMFLOAT3(-0.5f, 0.5f, 0.0f));  // Top-left
            geometry->pos.push_back(DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f));   // Top-right
            geometry->pos.push_back(DirectX::XMFLOAT3(0.0f, -0.5f, 0.0f));  // Bottom-right

            geometry->uv.push_back(DirectX::XMFLOAT2(u1, v1)); // Bottom-left
            geometry->uv.push_back(DirectX::XMFLOAT2(u2, v2)); // Top-left
            geometry->uv.push_back(DirectX::XMFLOAT2(u3, v3)); // Top-right
            geometry->uv.push_back(DirectX::XMFLOAT2(u4, v4)); // Bottom-right

            geometry->indices.push_back(0);
            geometry->indices.push_back(1);
            geometry->indices.push_back(2);
            geometry->indices.push_back(0);
            geometry->indices.push_back(2);
            geometry->indices.push_back(3);

            geometry->vertexNumber = geometry->pos.size();
            geometry->indiceNumber = geometry->indices.size();
        }
    }
}
