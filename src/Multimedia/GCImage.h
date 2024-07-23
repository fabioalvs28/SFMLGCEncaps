#pragma once
#include <vector>
#include <string>
#include <GCFile.h>


enum DrawQuality {
	DRAWQUALITY_NEAREST = 0,
	DRAWQUALITY_BILINEAR = 1,
	DRAWQUALITY_BICUBIC = 2
};

typedef unsigned char uint8_t;


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
	uint8_t* rgba;

	GCSurface() : width(0), height(0), bits(0), stride(0), size(0), count(0), rgba(nullptr) {}

	virtual ~GCSurface() {
		delete[] rgba;
	}
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
	UI32 m_width;
	UI32 m_height;
	int m_bitDepth;
	int m_channels = m_bitDepth / 8;
	int m_rowStride = rowStride();
	int m_size;

	std::vector<uint8_t> m_rgba;
	uint32_t rowStride() const { return ((m_width * m_channels) + 3) & (~3); }

public:


	GCImage(int w = 1600, int h = 1200, int bpp = 32) : m_width(w), m_height(h), m_bitDepth(bpp)
	{
		m_channels = m_bitDepth / 8;
		m_rowStride = rowStride();
		m_rgba.resize(m_width * m_height * m_channels, 0);
		m_size = m_width * m_height * m_channels;
	}

	// Copy constructor
	virtual ~GCImage();
	GCImage(const GCImage& img);
	//GCImage& operator=(GCSurface& surf);
	GCImage& operator+=(GCImage& img);
	GCImage& operator-=(GCImage& img);
	GCImage* Address() { return this; }


	GCImage& operator=(const GCImage& img);

	// Load image from file, img is the path and pRect is the rectangle to load
	bool Load(GCImage* img, REC2* pRect = nullptr);
	// Load image from file, pSurf is the surface and bits is the bits per pixel
	bool Load(GCSurface* pSurf, int bits);
	// Load image from file, buffer is the buffer and size is the size of the buffer
	bool Load(std::vector<uint8_t> buffer, int size);
	// Load image from file, file is the file and size is the size of the file
	bool Load(GCFile* file, int size);

	// Load image from file, buffer is the buffer, width and height are the size of the image and flip is the flip flag
	bool LoadRGB(std::vector<uint8_t> buffer, int width, int height, bool flip = false);
	// Load image from file, buffer is the buffer, width and height are the size of the image and flip is the flip flag
	bool LoadBGR(std::vector<uint8_t> buffer, int width, int height, bool flip = false);
	// Load image from file, filename is the path of the file
	bool LoadBMP(const std::string& filename);
	// Load image from file, filename is the path of the file
	bool LoadPNG(const std::string& filename);
	// Load image from PNG buffer, buffer is the buffer and size is the size of the buffer
	bool LoadPNG(std::vector<uint8_t>& buffer, int size);
	// Load image from JPG file, buffer is the path of the file and size is the size of the buffer
	bool LoadJPG(std::vector<uint8_t> buffer, int size);


	// Save image to file, path is the path of the file and type is the type of the file, 
	bool Save(GCFile* path, int type = BMP, int* pOutSize = nullptr, int width = -1, int height = -1);
	// Save image to file, path is the path of the file and type is the type of the file
	bool Save(cstr path, int type = BMP, int* pOutSize = nullptr);
	// Save image to BMP file, path is the path of the file and Outsize is the size of the file
	bool SaveBMP(GCFile* pFile, int* pOutSize = nullptr);
	// Save image to PNG file, path is the path of the file, pOutSize and gray is the gray flag
	bool SavePNG(GCFile* pFile, int* pOutSize = nullptr, bool gray = false);
	// Close the image
	void Close();

	// Clear the image, pRect is the rectangle to clear
	bool Clear(const REC2* pRect = nullptr);
	// Copy the image, x and y are the position, pSrc is the source image, xsrc and ysrc are the source position, w and h are the width and height
	bool Copy(int x, int y, GCImage* pSrc, int xsrc, int ysrc, int w = -1, int h = -1);
	// Copy the image, x and y are the position, pSrc is the source image, pRect is the rectangle to copy
	bool Copy(int x, int y, GCImage* pSrc, const REC2* pRect = nullptr);
	// Blend images, x and y are the position, pSrc is the source image, xsrc and ysrc are the source position, wsrc and hsrc are the width and height
	bool Blend(int x, int y, GCImage* pSrc, int xsrc, int ysrc, int wsrc = -1, int hsrc = -1);
	// Blend images, pSrc is the source image, x and y are the position and pRect is the rectangle to blend
	bool Blend(int x, int y, GCImage* pSrc, const REC2* pRect = nullptr);
	// Blend images, pRect is the rectangle to blend, pSrc is the source image, quality is the quality of the blend
	bool Blend(REC2* pRect, GCImage* pSrc, int quality = DRAWQUALITY_BILINEAR);
	// Blend images, x and y are the position, w and h are the width and height, pSrc is the source image, quality is the quality of the blend
	bool Blend(int x, int y, int w, int h, GCImage* pSrc, int quality = DRAWQUALITY_BILINEAR);

	//bool SaveJPG(GCFile* pFile, int* pOutSize = nullptr, int quality = 70);

	// check if the image has pixel data
	bool Has() {return &m_rgba != nullptr; }
	// get Size of the image
	int GetSize() { return m_size; }
	// get Width of the image
	int GetWidth() { return m_width; }
	// get Height of the image
	int GetHeight() { return m_height; }
	// Set Bit Depth of the image
	void SetBits(int bitDepth);
	// get Bit Depth of the image
	int GetBitDepth() { return m_bitDepth; }

	// get RGBA of the image
	std::vector<uint8_t> GetRGBA() { return m_rgba; }
	// get RGBA of the image
	inline uint8_t GetRGBA(int x, int y);
	// check if the pixel is valid
	inline bool IsValidPixel(int x, int y);
	// get Index of the pixel
	inline int GetIndex(int x, int y);
	// set Pixel of the image
	void SetPixel(int x, int y, int r, int g, int b, int a);
	// get Pixel of the image
	COLORREF GetPixel(int x, int y);
	// write a pixel to the image
	void WritePixel(int x, int y, int r, int g, int b, int a, int d = 0, int id = -1);
	// get pixel alpha of the image
	uint8_t GetPixelA(int x, int y);
	// get pixel depth of the image
	int GetPixelDepth(int x, int y);
	// get pixel color of the image
	bool GetPixels(std::vector<uint8_t> pTarget, int x, int y, int w, int h);
	// count how many pixel of this color are in the image
	int CountPixelOfColor(int r, int g, int b, int a);

	// Create Empty Image, w and h are the width and height, bpp is the bits per pixel (24 or 32)
	void CreateEmptyImage(int w, int h, int bpp);

	void WritePixel(int x, int y, COLORREF color, int d = 0, int id = -1);
	void DrawLine(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void DrawLineLow(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void DrawLineHigh(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void DrawRect(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void FillRect(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void DrawCircle(int x, int y, int radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void FillCircle(int x, int y, int radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void Fill(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	//Flip the image, horz is the horizontal flag and vert is the vertical flag
	bool Flip(bool horz = true, bool vert = false);
	// Add Horizontal Image, pImg is the image to add
	bool AddHorizontalImage(GCImage* pImg);
	// Add Vertical Image, pImg is the image to add
	bool AddVerticalImage(GCImage* pImg);
	// Set Alpha of the image, alpha is the alpha value
	bool SetAlpha(uint8_t alpha);
	// Set Alpha for Color, alpha is the alpha value and colorToFind is the color to find
	bool SetAlphaForColor(uint8_t alpha, COLORREF colorToFind);
	// Rotate the image, angle is the angle to rotate
	bool Rotate(int angle);
	//Premultiply the image
	bool Premultiply();
	//Solidify the image, pOpaque is the opaque image
	bool Solidify(GCImage* pOpaque);
	//Solidify the image
	bool Solidify();

	//Blend the image for Post Process, overlay is the overlay image, alpha is the alpha value
	bool BlendSTD(const GCImage& overlay, float alpha);
	//Blend the image for Pre Process, overlay is the overlay image, alpha is the alpha value
	bool BlendPRE(const GCImage& overlay, float alpha);


};



