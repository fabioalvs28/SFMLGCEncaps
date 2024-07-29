#pragma once

#include <math.h>
#include <iostream>
#include <fstream>
#include <tuple>
#include <iostream>
#include "GCImage.h"
#include "metadata.h"
namespace fs = std::filesystem;

struct Sprite
{
    GCImage image;
    int w;
    int h;
    bool rotated;
    std::string filename;
};
struct Pos
{
    Pos(int x, int y) : x(x), y(y) {};
    bool operator==(const Pos& other)
    {
        return x == other.x && y == other.y;
    }
    int x;
    int y;
};
struct Emplacement
{
    Emplacement(int w, int h, Pos pos) : w(w), h(h), pos(pos) {};
    bool operator==(const Emplacement& other)
    {
        return w == other.w && h == other.h && pos == other.pos;
    }
    int w;
    int h;
    Pos pos;
};

class SpriteSheetGenerator
{
    enum SaveFormat
    {
        BMP,
        PNG,
        DDS,
    };

    fs::path m_importPath = "";
    fs::path m_outputPath = "";
    int m_padding = 0;
    bool m_allowRotate = false;
    bool m_tryRefillPrevRow = false;

    static int getSmallestSheetSize(int remainingSize);
    static int getSizeOfImageList(std::vector<Sprite> Sprites, int padding);
    static int number_of_files_in_directory(fs::path path);
    static bool sortBySpriteHeight(const Sprite& a, const Sprite& b);

public:
    SpriteSheetGenerator() {};
    int Packer();

    void setInputPath(fs::path inputPath) { m_importPath = inputPath; }
    fs::path getInputPath() { return m_importPath; }

    void setOutputPath(fs::path outputPath) { m_outputPath = outputPath; }
    fs::path getOutputPath() { return m_outputPath; }

    void setAllowRotate(bool allowRotate) { m_allowRotate = allowRotate; }
    bool getAllowRotate() { return m_allowRotate; }

    void setTryRefillPrevRow(bool tryRefillPrevRow) { m_tryRefillPrevRow = tryRefillPrevRow; }
    bool getTryRefillPrevRow() { return m_tryRefillPrevRow; }

    void setPadding(int padding) { m_padding = padding; }
    int getPadding() { return m_padding; }


};