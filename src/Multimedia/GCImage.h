#pragma once
#include <vector>
#include <string>
#include <GCFile.h>

typedef unsigned char BYTE;

class GCImage
{
private:

	BYTE* m_rgba;
	UI32 m_width;
	UI32 m_height;
	int m_bitCount;
	int m_channels = m_bitCount / 8;
	int m_rowStride = (m_width * m_channels + 3) & (~3);
	int size;

	std::vector<uint8_t> data;
	uint32_t rowStride() const { return ((m_width * m_bitCount / 8) + 3) & ~3; }

	void DrawLineLow(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void DrawLineHigh(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a);


public:


	GCImage(int w = 1600, int h = 1200, int bpp = 32) : m_width(w), m_height(h), m_bitCount(bpp)
	{
		m_channels = m_bitCount / 8;
		m_rowStride = (m_width * m_channels + 3) & (~3);
		data.resize(m_width * m_height * m_channels, 0);
		size = m_width * m_height * m_channels;
		m_rgba = data.data();
	}

	virtual ~GCImage();
	GCImage(const GCImage& img);

	GCImage& operator=(const GCImage& img);

	bool Load(GCImage* img);
	bool LoadBMP(const std::string& filename);
	bool LoadPNG(BYTE* buffer, int size);
	bool SaveBMP(const std::string& filename);
	bool SavePNG(GCFile* file, int* pOutSize = nullptr, bool gray = false);
	void Close();

	void CreateEmptyImage(int w, int h, int bpp);
	void SetPixel(int x, int y, int r, int g, int b, int a);
	void WritePixel(int x, int y, COLORREF color, int d = 0, int id = -1);
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
};

