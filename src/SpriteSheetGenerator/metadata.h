#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include <GCFile.h>
#include "nlohmann/json.hpp"
using json = nlohmann::ordered_json;

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
        auto operator<=>(const Image&) const = default;
    };

    struct Texture
    {
        std::string textureName;
        uint16_t texturesize;
        uint32_t imageCount;
        std::vector<Image> images;
        auto operator<=>(const Texture&) const = default;
    };

    struct Header
    {
        uint32_t magic;
        uint32_t totalImageCount;
        uint32_t texturesCount;
        auto operator<=>(const Header&) const = default;
    };

    struct Data
    {
        Header header{};
        std::vector<Texture> textures;
        auto operator<=>(const Data&) const = default;
    };

    static Data jsonToMetadataFile(json data);
    static bool MetadataFileToStruct(GCFile file, Metadata::Data& metadata);
};