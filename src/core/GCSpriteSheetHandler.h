#pragma once

#include <cmath>
#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>
#include <filesystem>
#include "GCImage.h"
#include "metadata.h"
namespace fs = std::filesystem;

struct Sprite {
    GCImage image;
    int w;
    int h;
    bool rotated;
    std::string filename;
    bool cropped = false;
    REC2 cropRect = REC2(0, 0, 0, 0);
};

struct Pos {
    Pos(int x, int y) : x(x), y(y) {};
    bool operator==(const Pos& other) const {
        return x == other.x && y == other.y;
    }
    int x;
    int y;
};

struct Emplacement {
    Emplacement(int w, int h, Pos pos) : w(w), h(h), pos(pos) {};
    bool operator==(const Emplacement& other) const {
        return w == other.w && h == other.h && pos == other.pos;
    }
    int w;
    int h;
    Pos pos;
};

class GCSpriteSheetHandler {
    enum SaveFormat {
        BMP,
        PNG,
        DDS,
    };

    fs::path m_importPath = "";
    fs::path m_outputPath = "";
    int m_padding = 0;
    bool m_allowRotate = false;
    bool m_allowTrimming = false;
    bool m_tryRefillPrevRow = false;

    static int getSmallestSheetSize(int remainingSize);
    static int getSizeOfImageList(const std::vector<Sprite>& sprites, int padding);
    static int numberOfFilesInDirectory(const fs::path& path);
    static bool sortBySpriteHeight(const Sprite& a, const Sprite& b);

public:
    GCSpriteSheetHandler(const std::string& outputPath, const std::string& entryPath);

    int packer(const std::vector<fs::path>& inputPaths,
        int padding = 0,
        bool allowTrimming = true,
        bool allowRotate = false,
        bool tryRefillPrevRow = true);

    void saveSpritesheet(GCImage& image, int index);

    void setInputPath(const fs::path& inputPath) { m_importPath = inputPath; }
    fs::path getInputPath() const { return m_importPath; }

    void setOutputPath(const fs::path& outputPath) { m_outputPath = outputPath; }
    fs::path getOutputPath() const { return m_outputPath; }

    void setAllowRotate(bool allowRotate) { m_allowRotate = allowRotate; }
    bool getAllowRotate() const { return m_allowRotate; }

    void setAllowTrimming(bool allowTrimming) { m_allowTrimming = allowTrimming; }
    bool getAllowTrimming() const { return m_allowTrimming; }

    void setTryRefillPrevRow(bool tryRefillPrevRow) { m_tryRefillPrevRow = tryRefillPrevRow; }
    bool getTryRefillPrevRow() const { return m_tryRefillPrevRow; }

    void setPadding(int padding) { m_padding = padding; }
    int getPadding() const { return m_padding; }

    static GCImage trimImage(GCImage image, REC2& outCropRect);
};
