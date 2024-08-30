#pragma once

#include <cstdint>

class Metadata
{
public:

    struct Image
    {
        std::string filename;
        uint16_t x;
        uint16_t y;
        uint16_t w;
        uint16_t h;
        uint8_t rotated;
    };

    struct Texture
    {
        std::string textureName;
        uint16_t texturesize;
        uint32_t imageCount;
        std::vector<Image> images;
    };

    struct Header
    {
        uint32_t magic; //??
        uint32_t totalImageCount;
        uint32_t texturesCount;
    };

    struct Data
    {
        Header header{};
        std::vector<Texture> textures;
    };

    static bool MetadataFileToStruct(GCFile file, Metadata::Data& metadata);
};




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
    void CalculateUVs(GC_SPRITESHEET& spritesheet, const std::vector<Metadata::Image>& images);

private:
    std::vector<GCGeometry*> m_geometries;
};
