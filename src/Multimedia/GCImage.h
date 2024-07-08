#pragma once
#include <vector>
#include <string>
#include <GCFile.h>


enum DrawQuality {
	DRAWQUALITY_NEAREST = 0,
	DRAWQUALITY_BILINEAR = 1,
	DRAWQUALITY_BICUBIC = 2
};

typedef unsigned char BYTE;

typedef uint32_t COLORREF;
#define RGB(r, g, b) ((COLORREF)(((uint8_t)(r) | ((uint16_t)((uint8_t)(g)) << 8)) | (((uint32_t)(uint8_t)(b)) << 16)))
#define GetRValue(rgb) ((uint8_t)(rgb))
#define GetGValue(rgb) ((uint8_t)(((uint16_t)(rgb)) >> 8))
#define GetBValue(rgb) ((uint8_t)((rgb) >> 16))

struct REC2 {
	int x;
	int y;
	int width;
	int height;

	REC2() : x(0), y(0), width(0), height(0) {}
	REC2(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}
};

class GCSurface {
public:
	int width;
	int height;
	int bits;
	int stride;
	int size;
	int count;
	BYTE* rgba;

	GCSurface() : width(0), height(0), bits(0), stride(0), size(0), count(0), rgba(nullptr) {}

	virtual ~GCSurface() {
		delete[] rgba;
	}

	// Add any necessary methods for SSurface
};

enum ImageType {
	BMP = 0,
	PNG = 1,
	JPG = 3,
	TGA = 4
};

class GCImage
{
private:

	BYTE* m_rgba;
	UI32 m_width;
	UI32 m_height;
	int m_bitCount;
	int m_channels = m_bitCount / 8;
	int m_rowStride = (m_width * m_channels + 3) & (~3);
	int m_size;
	int m_bits;

	std::vector<uint8_t> data;
	uint32_t rowStride() const { return ((m_width * m_bitCount / 8) + 3) & ~3; }

	//void DrawLineLow(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	//void DrawLineHigh(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a);


public:


	GCImage(int w = 1600, int h = 1200, int bpp = 32) : m_width(w), m_height(h), m_bitCount(bpp)
	{
		m_channels = m_bitCount / 8;
		m_rowStride = (m_width * m_channels + 3) & (~3);
		data.resize(m_width * m_height * m_channels, 0);
		m_size = m_width * m_height * m_channels;
		m_rgba = data.data();
		m_bits = bpp;
	}

	virtual ~GCImage();
	GCImage(const GCImage& img);
	GCImage& operator=(GCSurface& surf);
	GCImage& operator+=(GCImage& img);
	GCImage& operator-=(GCImage& img);
	GCImage* Address() { return this; }


	GCImage& operator=(const GCImage& img);

	bool Load(GCImage* img, REC2* pRect = nullptr);
	bool Load(GCSurface* pSurf, int bits);
	bool Load(BYTE* buffer, int size);
	bool Load(GCFile* file, int size);

	bool LoadRGB(BYTE* buffer, int width, int height, bool flip = false);
	bool LoadBGR(BYTE* buffer, int width, int height, bool flip = false);
	bool LoadBMP(const std::string& filename);
	bool LoadPNG(BYTE* buffer, int size);
	bool LoadJPG(BYTE* buffer, int size);

	bool Save(GCFile* path, int type = BMP, int* OutSize = nullptr, int width = -1, int height = -1);
	bool Save(cstr path, int type = BMP, int* OutSize = nullptr);
	bool SaveBMP(GCFile* pFile, int* pOutSize = nullptr);
	bool SavePNG(GCFile* pFile, int* pOutSize = nullptr, bool gray = false);
	void Close();

	bool Clear(const REC2* pRect = nullptr);
	bool Copy(int x, int y, GCImage* pSrc, int xsrc, int ysrc, int w = -1, int h = -1);
	bool Copy(int x, int y, GCImage* pSrc, const REC2* pRect = nullptr);
	bool Blend(int x, int y, GCImage* pSrc, int xsrc, int ysrc, int wsrc = -1, int hsrc = -1);
	bool Blend(int x, int y, GCImage* pSrc, const REC2* pRect = nullptr);
	bool Blend(REC2* pRect, GCImage* pSrc, int quality = DRAWQUALITY_BILINEAR);
	bool Blend(int x, int y, int w, int h, GCImage* pSrc, int quality = DRAWQUALITY_BILINEAR);

	//bool SaveJPG(GCFile* pFile, int* pOutSize = nullptr, int quality = 70);

	bool Has() {return m_rgba != nullptr; }
	int GetSize() { return m_size; }
	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }
	int GetPixelCount() { return m_bitCount; }
	void SetBits(int bits);
	int GetBits() { return m_bits; }

	BYTE* GetRGBA() { return m_rgba; }
	inline BYTE* GetRGBA(int x, int y);
	inline bool IsValidPixel(int x, int y);
	inline int GetIndex(int x, int y);
	void SetPixel(int x, int y, int r, int g, int b, int a);
	COLORREF GetPixel(int x, int y);
	void WritePixel(int x, int y, int r, int g, int b, int a, int d = 0, int id = -1);
	BYTE GetPixelA(int x, int y);
	int GetPixelDepth(int x, int y);
	bool GetPixels(BYTE* pTarget, int x, int y, int w, int h);
	int GetPixelCount(int r, int g, int b, int a);

	void CreateEmptyImage(int w, int h, int bpp);
	//void WritePixel(int x, int y, COLORREF color, int d = 0, int id = -1);
	//void DrawLine(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	//void DrawRect(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	//void FillRect(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	//void DrawCircle(int x, int y, int radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	//void FillCircle(int x, int y, int radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	//void Fill(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	bool Flip(bool horz = true, bool vert = false);
	bool AddHorizontalImage(GCImage* pImg);
	bool AddVerticalImage(GCImage* pImg);
	bool SetAlpha(BYTE alpha);
	bool SetAlphaForColor(BYTE alpha, COLORREF colorToFind);
	bool Rotate(int angle);
	bool Premultiply();
	bool Solidify(GCImage* pOpaque);
	bool Solidify();

	bool Copy(int x, int y, GCImage* pImg, int xsrc, int ysrc, int w = -1, int h = -1);
	bool Copy(int x, int y, GCImage* pImg, const REC2* pRect = nullptr);
	bool Blend(int x, int y, GCImage* pImg, int xsrc, int ysrc, int w = -1, int h = -1);
	bool BlendSTD(const GCImage& overlay, float alpha);
	bool BlendPRE(const GCImage& overlay, float alpha);


};



