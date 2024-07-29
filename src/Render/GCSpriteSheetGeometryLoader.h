#pragma once

struct SpriteInfo {
    GCVEC2 uvs[4]; 
    int width;     
    int height;   
};

struct GC_SPRITESHEET_INFO {
    bool isFlip;
    int width;                 
    int height;                
    std::map<int, SpriteInfo> spriteUVMap; 
};

class GCSpriteSheetGeometryLoader {
public:

    GC_SPRITESHEET_INFO LoadSpriteSheet(int rowNumber, int colNumber, int sheetWidth, int sheetHeight);
    void SetSpriteUVs(GCGeometry* geometry, int spriteID, const GC_SPRITESHEET_INFO& spriteSheetInfo);
    void CalculateUVs(int spriteWidth, int spriteHeight, int sheetWidth, int sheetHeight, GC_SPRITESHEET_INFO& info);

private:
    std::vector<GCGeometry*> m_geometries;
};
