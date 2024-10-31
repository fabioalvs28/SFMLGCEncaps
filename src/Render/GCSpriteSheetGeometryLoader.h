#pragma once

#include "../Core/metadata.h"
#include <cstdint>

struct GC_SPRITE {
    GCVEC2 uvs[4]; 
    int width;     
    int height;   
};

struct GC_SPRITESHEET {

    int size;
    std::map<int, GC_SPRITE> sprites;
};

struct GC_SPRITESHEET_DATA {
                 
    std::vector<GC_SPRITESHEET> spritesheets;
};


class GCSpriteSheetGeometryLoader {
public:

    GC_SPRITESHEET_DATA LoadSpriteSheets(const char* filename);
    void SetSpriteUVs(GCGeometry* geometry, int spriteSheetID, int spriteID, const GC_SPRITESHEET_DATA& spriteSheetInfo);
    void CalculateUVs(GC_SPRITESHEET& spritesheet, const std::vector<Metadata::Image*>& images);

private:
    std::vector<GCGeometry*> m_geometries;
};