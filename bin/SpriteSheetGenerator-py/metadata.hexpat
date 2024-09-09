struct Image
{
    char filename[];
    u16 x;
    u16 y;
    u16 w;
    u16 h;
    bool rotated;
    bool cropped;
    u16 crop_x;
    u16 crop_y;
    u16 original_w;
    u16 original_h;
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