#pragma once
#include <vector>
#include <string>

typedef unsigned char BYTE;

class GCImage
{
private:

	int channels = bitCount / 8;
	int rowPadded = (width * channels + 3) & (~3);
	uint32_t rowStride() const { return ((width * bitCount / 8) + 3) & ~3; }

	void DrawLineLow(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void DrawLineHigh(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

public:

	int width;
	int height;
	int bitCount;
	std::vector<uint8_t> data;

	GCImage(int w = 1600, int h = 1200, int bpp = 32) : width(w), height(h), bitCount(bpp)
	{
		channels = bitCount / 8;
		rowPadded = (width * channels + 3) & (~3);
		data.resize(width * height * channels, 0);
	}

	bool LoadBMP(const std::string& filename);
	bool SaveBMP(const std::string& filename);
	void CreateEmptyImage(int w, int h, int bpp);
	void SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void DrawLine(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void DrawRect(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void FillRect(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void DrawCircle(int x, int y, int radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void FillCircle(int x, int y, int radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void Fill(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	void InverseBMP(const std::string& filename);
	bool CopyBMP(const std::string& filename, const std::string& newFilename);
	bool BlendSTD(const GCImage& overlay, float alpha);
	bool BlendPRE(const GCImage& overlay, float alpha);
	bool Premultiply();

	bool SavePNG(const std::string& filename);
	bool LoadPNG(BYTE* buffer, int size);
};

