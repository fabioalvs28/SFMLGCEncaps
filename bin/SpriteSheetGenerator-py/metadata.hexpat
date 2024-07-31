struct Image
{
    char filename[];
    u16 x;
    u16 y;
    u16 w;
    u16 h;
    bool rotated;
};

struct Texture
{
    char textureName[];
    u16 texturesize;
    u32 imageCount;
    Image images[imageCount];
};

struct Header
{
    u32 magic;
    u32 totalImageCount;
    u32 texturesCount;
};

struct Data
{
    Header header;
    Texture textures[header.texturesCount];
};

Data data @ 0x0;