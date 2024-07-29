#pragma once

class GCSpriteSheetGeometryLoader {
public:

    std::vector<GCGeometry*> LoadSpriteSheet(int rowNumber, int colNumber, int sheetWidth, int sheetHeight);
    void CalculateUVs(int spriteWidth, int spriteHeight, int sheetWidth, int sheetHeight);

private:
    std::vector<GCGeometry*> m_geometries;
};
