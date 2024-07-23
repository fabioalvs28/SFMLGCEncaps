#include "GCImage.h"
#include "BMPHeader.h"
#include "GCFile.h"
#include "lodepng.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring>

GCImage::GCImage(const GCImage& img)
{
	m_rgba = img.m_rgba;
	m_width = img.m_width;
	m_height = img.m_height;
	m_bitDepth = img.m_bitDepth;
	m_channels = img.m_channels;
	m_rowStride = img.m_rowStride;
	m_size = img.m_size;
	m_rgba = img.m_rgba;
}

//** (operator=(GCSurface)) Not working **//
//GCImage& GCImage::operator=(GCSurface& surf)
//{
	//if (this != &surf)
	//{
	//	m_width = surf.width;
	//	m_height = surf.height;
	//	m_bitCount = surf.bits;
	//	m_channels = surf.count;
	//	m_rowStride = rowStride();
	//	m_size = m_width * m_height * m_channels;
	//	data.resize(m_size, 0);
	//	m_rgba = data.data();
	//	for (int y = 0; y < m_height; ++y)
	//	{
	//		for (int x = 0; x < m_width; ++x)
	//		{
	//			int index = (x + y * m_width) * m_channels;
	//			int surfIndex = (x + y * surf.width) * surf.count;
	//			data[index] = surf.rgba[surfIndex];
	//			data[index + 1] = surf.rgba[surfIndex + 1];
	//			data[index + 2] = surf.rgba[surfIndex + 2];
	//			if (m_channels == 4)
	//			{
	//				data[index + 3] = surf.rgba[surfIndex + 3];
	//			}
	//		}
	//	}
	//}
	//return *this;
//}

GCImage& GCImage::operator+=(GCImage& img)
{
	if (img.m_width != m_width || img.m_height != m_height || img.m_channels != m_channels)
	{
		std::cerr << "Images must have the same dimensions and channels." << std::endl;
		return *this;
	}

	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			int index = (x + y * m_width) * m_channels;
			int imgIndex = (x + y * img.m_width) * img.m_channels;
			m_rgba[index] += img.m_rgba[imgIndex];
			m_rgba[index + 1] += img.m_rgba[imgIndex + 1];
			m_rgba[index + 2] += img.m_rgba[imgIndex + 2];
			if (m_channels == 4)
			{
				m_rgba[index + 3] += img.m_rgba[imgIndex + 3];
			}
		}
	}
	return *this;
}

GCImage& GCImage::operator-=(GCImage& img)
{
	if (img.m_width != m_width || img.m_height != m_height || img.m_channels != m_channels)
	{
		std::cerr << "Images must have the same dimensions and channels." << std::endl;
		return *this;
	}

	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			int index = (x + y * m_width) * m_channels;
			int imgIndex = (x + y * img.m_width) * img.m_channels;
			m_rgba[index] -= img.m_rgba[imgIndex];
			m_rgba[index + 1] -= img.m_rgba[imgIndex + 1];
			m_rgba[index + 2] -= img.m_rgba[imgIndex + 2];
			if (m_channels == 4)
			{
				m_rgba[index + 3] -= img.m_rgba[imgIndex + 3];
			}
		}
	}
	return *this;
}

GCImage::~GCImage()
{
	Close();
}

GCImage& GCImage::operator=(const GCImage& img)
{
	if (this != &img) 
	{
		m_width = img.m_width;
		m_height = img.m_height;
		m_bitDepth = img.m_bitDepth;
		m_channels = img.m_channels;
		m_rgba = img.m_rgba;
	}
	return *this;
}

bool GCImage::Load(GCImage* img, REC2* pRect)
{
	if (img == nullptr)
	{
		return false;
	}

	if (pRect == nullptr) {
		m_width = img->m_width;
		m_height = img->m_height;
		m_bitDepth = img->m_bitDepth;
		m_channels = img->m_channels;
		m_rowStride = img->m_rowStride;
		m_size = img->m_size;
		m_rgba = img->m_rgba;
	}
	else {
		m_width = pRect->width;
		m_height = pRect->height;
		m_bitDepth = img->m_bitDepth;
		m_channels = img->m_channels;
		m_rowStride = rowStride();
		m_size = m_width * m_height * m_channels;
		m_rgba.resize(m_size, 0);
		for (int y = 0; y < m_height; ++y) {
			for (int x = 0; x < m_width; ++x) {
				int index = (x + y * m_width) * m_channels;
				int imgIndex = ((x + pRect->x) + (y + pRect->y) * img->m_width) * img->m_channels;
				m_rgba[index] = img->m_rgba[imgIndex];
				m_rgba[index + 1] = img->m_rgba[imgIndex + 1];
				m_rgba[index + 2] = img->m_rgba[imgIndex + 2];
				if (m_channels == 4) {
					m_rgba[index + 3] = img->m_rgba[imgIndex + 3];
				}
			}
		}
	}
	return true;
}

bool GCImage::Load(GCSurface* pSurf, int bits)
{
	if (pSurf == nullptr)
	{
		return false;
	}

	m_width = pSurf->width;
	m_height = pSurf->height;
	m_bitDepth = bits;
	m_channels = m_bitDepth / 8;
	m_rowStride = rowStride();
	m_size = m_width * m_height * m_channels;
	m_rgba.resize(m_size, 0);
	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			int index = (x + y * m_width) * m_channels;
			int surfIndex = (x + y * pSurf->width) * pSurf->count;
			m_rgba[index] = pSurf->rgba[surfIndex];
			m_rgba[index + 1] = pSurf->rgba[surfIndex + 1];
			m_rgba[index + 2] = pSurf->rgba[surfIndex + 2];
			if (m_channels == 4) {
				m_rgba[index + 3] = pSurf->rgba[surfIndex + 3];
			}
		}
	}
	return true;

}

bool GCImage::Load(std::vector<uint8_t> buffer, int size)
{
	if (buffer.data() == nullptr || size <= 0)
		return false;

	Close();

	m_rgba = buffer;
	m_width = m_width;
	m_height = m_height;
	m_bitDepth = m_bitDepth;
	m_channels = m_channels;
	m_rowStride = m_rowStride;
	m_size = size;
	return true;
}

bool GCImage::Load(GCFile* file, int size)
{
	if (file == nullptr || size <= 0)
		return false;

	Close();

	std::vector<uint8_t> buffer(size);
	if (!file->Read(buffer, size)) {
		std::cerr << "Error reading file." << std::endl;
		return false;
	}

	m_rgba = buffer;
	m_width = m_width;
	m_height = m_height;
	m_bitDepth = m_bitDepth;
	m_channels = m_channels;
	m_rowStride = m_rowStride;
	size = size;
	return true;
}

bool GCImage::LoadRGB(std::vector<uint8_t> buffer, int width, int height, bool flip)
{
	if (buffer.data() == nullptr || width <= 0 || height <= 0)
		return false;

	Close();

	m_width = width;
	m_height = height;
	m_bitDepth = 24;
	m_channels = m_bitDepth / 8;
	m_rowStride = rowStride();
	m_size = m_width * m_height * m_channels;
	m_rgba.resize(m_size);

	if (flip) 
	{
		for (int y = 0; y < m_height; ++y) 
		{
			for (int x = 0; x < m_width; ++x) 
			{
				int index = (x + y * m_width) * m_channels;
				int bufferIndex = ((m_height - 1 - y) * m_width + x) * 3;
				m_rgba[index] = buffer[bufferIndex];
				m_rgba[index + 1] = buffer[bufferIndex + 1];
				m_rgba[index + 2] = buffer[bufferIndex + 2];
			}
		}
	}
	else 
	{
		for (int y = 0; y < m_height; ++y)
		{
			for (int x = 0; x < m_width; ++x) 
			{
				int index = (x + y * m_width) * m_channels;
				int bufferIndex = (y * m_width + x) * 3;
				m_rgba[index] = buffer[bufferIndex];
				m_rgba[index + 1] = buffer[bufferIndex + 1];
				m_rgba[index + 2] = buffer[bufferIndex + 2];
			}
		}
	}

	return true;

}

bool GCImage::LoadBGR(std::vector<uint8_t> buffer, int width, int height, bool flip)
{
	if (buffer.data() == nullptr || width <= 0 || height <= 0)
		return false;

	Close();

	m_width = width;
	m_height = height;
	m_bitDepth = 24;
	m_channels = m_bitDepth / 8;
	m_rowStride = rowStride();
	m_size = m_width * m_height * m_channels;
	m_rgba.resize(m_size);

	if (flip) 
	{
		for (int y = 0; y < m_height; ++y)
		{
			for (int x = 0; x < m_width; ++x) 
			{
				int index = (x + y * m_width) * m_channels;
				int bufferIndex = ((m_height - 1 - y) * m_width + x) * 3;
				m_rgba[index] = buffer[bufferIndex + 2];
				m_rgba[index + 1] = buffer[bufferIndex + 1];
				m_rgba[index + 2] = buffer[bufferIndex];
			}
		}
	}
	else 
	{
		for (int y = 0; y < m_height; ++y) 
		{
			for (int x = 0; x < m_width; ++x) 
			{
				int index = (x + y * m_width) * m_channels;
				int bufferIndex = (y * m_width + x) * 3;
				m_rgba[index] = buffer[bufferIndex + 2];
				m_rgba[index + 1] = buffer[bufferIndex + 1];
				m_rgba[index + 2] = buffer[bufferIndex];
			}
		}
	}

	return true;
}

bool GCImage::LoadBMP(const std::string& filename)
{
	GCFile file(filename.c_str(), "rb");

	BMPHeader header;
	std::vector<uint8_t> headerBuffer(sizeof(header));
	if (!file.Read(headerBuffer, sizeof(header))) 
	{
		std::cerr << "Error reading header from file: " << filename << std::endl;
		return false;
	}
	std::memcpy(&header, headerBuffer.data(), sizeof(header));

	m_width = header.biWidth;
	m_height = header.biHeight;
	m_bitDepth = header.biBitCount;
	m_channels = m_bitDepth / 8;
	m_size = m_width * m_height * m_channels;
	m_rgba.resize(m_size);

	if (fseek(file.file, header.bfOffBits, SEEK_SET) != 0) 
	{
		std::cerr << "Error seeking to pixel data in file: " << filename << std::endl;
		return false;
	}

	uint32_t padding = rowStride() - m_width * m_channels;
	std::vector<uint8_t> rowData(rowStride());

	for (int y = 0; y < m_height; ++y) {
		if (!file.Read(rowData, rowStride())) 
		{
			std::cerr << "Error reading pixel data from file: " << filename << std::endl;
			return false;
		}
		std::memcpy(&m_rgba[(m_height - 1 - y) * m_width * m_channels], rowData.data(), m_width * m_channels);
	}
	file.Close();
	return true;
}

bool GCImage::LoadPNG(const std::string& filename)
{
	GCFile imageFile = GCFile(filename.c_str(), "rb");
	std::vector<uint8_t> buffer;
	if (!imageFile.Read(buffer, imageFile.size))
	{
		std::printf("Error opening file\n");
		Close();
		return 1;
	}
	imageFile.Close();

	unsigned error = lodepng::decode(m_rgba, m_width, m_height, buffer);

	//if there's an error, display it
	if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

	m_rowStride = m_width * 4;
	m_size = m_rowStride * m_height;
	m_channels = m_bitDepth / 8;
	return true;
}

bool GCImage::LoadPNG(std::vector<uint8_t>& buffer, int size)
{
	if (buffer.data() == nullptr || size <= 0)
	{
		Close();
		return false;
	}

	unsigned error = lodepng::decode(m_rgba, m_width, m_height, buffer);

	//if there's an error, display it
	if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

	m_rowStride = m_width * 4;
	m_size = m_rowStride * m_height;
	m_channels = m_bitDepth / 8;
	return true;
}

bool GCImage::LoadJPG(std::vector<uint8_t> buffer, int size)
{
	if (buffer.data() == nullptr || size <= 0)
	{
		Close();
		return false;
	}

	unsigned error = lodepng::decode(m_rgba, m_width, m_height, buffer);

	//if there's an error, display it
	if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

	m_rowStride = m_width * 4;
	m_size = m_rowStride * m_height;
	m_channels = m_bitDepth / 8;
	return true;
}

bool GCImage::Save(GCFile* path, int type, int* pOutSize, int width, int height)
{
	if (path == nullptr)
		return false;

	switch (type)
	{
	case BMP:
		return SaveBMP(path, pOutSize);
		break;
	case PNG:
		return SavePNG(path, pOutSize);
		break;
	//case JPG:
	//	return SaveJPG(path, pOutSize);
	//	break;
	default:
		return false;
		break;
	}
}

bool GCImage::Save(cstr path, int type, int* pOutSize)
{
	GCFile file(path, "wb");
	return Save(&file, type, pOutSize);
}

bool GCImage::SaveBMP(GCFile* pFile, int* pOutSize)
{
	if (pFile == nullptr)
		return false;

	BMPHeader header{};
	header.bfType = 0x4D42;
	header.bfSize = sizeof(header) + m_size;
	header.bfReserved1 = 0;
	header.bfReserved2 = 0;
	header.bfOffBits = sizeof(header);

	header.biSize = 40;
	header.biWidth = m_width;
	header.biHeight = m_height;
	header.biPlanes = 1;
	header.biBitCount = m_bitDepth;
	header.biCompression = 0;
	header.biSizeImage = m_size;
	header.biXPelsPerMeter = 0;
	header.biYPelsPerMeter = 0;
	header.biClrUsed = 0;
	header.biClrImportant = 0;

	std::vector<uint8_t> headerBuffer(sizeof(header));
	std::memcpy(headerBuffer.data(), &header, sizeof(header));

	if (!pFile->Write(headerBuffer))
	{
		std::cerr << "Error writing header to file." << std::endl;
		return false;
	}

	uint32_t padding = rowStride() - m_width * m_channels;
	std::vector<uint8_t> rowData(rowStride(), 0);

	for (int y = 0; y < m_height; ++y) 
	{
		std::memcpy(rowData.data(), &m_rgba[(m_height - 1 - y) * m_width * m_channels], m_width * m_channels);
		if (!pFile->Write(rowData)) {
			std::cerr << "Error writing pixel data to file." << std::endl;
			return false;
		}
	}

	if (pOutSize)
		*pOutSize = sizeof(header) + m_size;
	return true;
}


bool GCImage::SavePNG(GCFile* pFile, int* pOutSize, bool gray)
{
	if (pFile == nullptr || m_rgba.data() == nullptr)
		return false;

	std::vector<uint8_t> png;
	size_t size = 0;
	unsigned error = lodepng::encode(png, m_rgba.data(), m_width, m_height);
	if (error)
		return false;

	pFile->Write(png);

	if (pOutSize)
		*pOutSize = (int)size;
	return true;
}

void GCImage::Close()
{
	if (m_rgba.data()) {
		m_rgba.clear();
	}
	m_width = 0;
	m_height = 0;
	m_bitDepth = 0;
	m_rowStride = 0;
	m_size = 0;
	m_channels = 0;
}

bool GCImage::Clear(const REC2* pRect)
{
	if (pRect == nullptr)
	{
		std::memset(m_rgba.data(), 0, m_size);
	}
	else
	{
		for (int y = pRect->y; y < pRect->y + pRect->height; ++y)
		{
			for (int x = pRect->x; x < pRect->x + pRect->width; ++x)
			{
				int index = (x + y * m_width) * m_channels;
				m_rgba[index] = 0;
				m_rgba[index + 1] = 0;
				m_rgba[index + 2] = 0;
				if (m_channels == 4)
				{
					m_rgba[index + 3] = 0;
				}
			}
		}
	}
	return true;
}

void GCImage::SetBits(int bitDepth)
{
	m_bitDepth = bitDepth;
	m_channels = m_bitDepth / 8;
	m_rowStride = rowStride();
	m_size = m_width * m_height * m_channels;
	m_rgba.resize(m_size, 0);
}

void GCImage::CreateEmptyImage(int w, int h, int bitDepth)
{
	m_width = w;
	m_height = h;
	m_bitDepth = bitDepth;
	m_channels = m_bitDepth / 8;
	m_rgba.resize(m_width * m_height * m_channels, 0);
}

inline uint8_t GCImage::GetRGBA(int x, int y)
{
	if (x < 0 || x >= m_width || y < 0 || y >= m_height)
	{
		return NULL;
	}
	return m_rgba[(x + y * m_width) * m_channels];
}

inline bool GCImage::IsValidPixel(int x, int y)
{
	return x >= 0 && x < m_width && y >= 0 && y < m_height;
}

inline int GCImage::GetIndex(int x, int y)
{
	return (x + y * m_width) * m_channels;
}

void GCImage::SetPixel(int x, int y, int r, int g, int b, int a)
{
	if (x < 0 || x >= m_width || y < 0 || y >= m_height) return;

	int m_channels = m_bitDepth / 8;
	int index = (x + y * m_width) * m_channels;

	m_rgba[index] = r;
	m_rgba[index + 1] = g;
	m_rgba[index + 2] = b;
	if (m_channels == 4)
	{
		m_rgba[index + 3] = a;
	}
}

COLORREF GCImage::GetPixel(int x, int y)
{
	if (x < 0 || x >= m_width || y < 0 || y >= m_height) return 0;
	int index = (x + y * m_width) * m_channels;
	return RGB(m_rgba[index], m_rgba[index + 1], m_rgba[index + 2]);
}

void GCImage::WritePixel(int x, int y, int r, int g, int b, int a, int d, int id)
{
}

uint8_t GCImage::GetPixelA(int x, int y)
{
	if (x < 0 || x >= m_width || y < 0 || y >= m_height) return 0;
	int index = (x + y * m_width) * m_channels;
	return m_rgba[index + 3];
}

int GCImage::GetPixelDepth(int x, int y)
{
	if (x < 0 || x >= m_width || y < 0 || y >= m_height) return 0;
	return m_channels;
}

bool GCImage::GetPixels(std::vector<uint8_t> pTarget, int x, int y, int w, int h)
{
	if (pTarget.data() == nullptr || x < 0 || x >= m_width || y < 0 || y >= m_height || w <= 0 || h <= 0)
		return false;

	int channels = m_bitDepth / 8;
	int rowStride = (m_width * channels + 3) & (~3);
	int targetStride = (w * channels + 3) & (~3);

	for (int i = 0; i < h; ++i) 
	{
		if (y + i >= m_height) break;
		std::memcpy(&pTarget[i * targetStride], &m_rgba[(x + (y + i) * m_width) * channels], w * channels);
	}

	return true;
}

int GCImage::CountPixelOfColor(int r, int g, int b, int a)
{
	int count = 0;
	for (int y = 0; y < m_height; ++y) 
	{
		for (int x = 0; x < m_width; ++x) 
		{
			int index = (x + y * m_width) * m_channels;
			if (m_rgba[index] == r && m_rgba[index + 1] == g && m_rgba[index + 2] == b && m_rgba[index + 3] == a)
			{
				++count;
			}
		}
	}
	return count;
}

//** All drawing Functions **//
void GCImage::WritePixel(int x, int y, COLORREF color, int d, int id)
{
	if (x < 0 || x >= m_width || y < 0 || y >= m_height) 
	{
		return;
	}
	int index = (x + y * m_width) * m_channels;
	m_rgba[index] = GetRValue(color);
	m_rgba[index + 1] = GetGValue(color);
	m_rgba[index + 2] = GetBValue(color);
}

void GCImage::DrawLine(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	if (abs(y2 - y1) < abs(x2 - x1)) 
	{
		if (x1 > x2) 
		{
			DrawLineLow(x2, y2, x1, y1, r, g, b, a);
		}
		else 
		{
			DrawLineLow(x1, y1, x2, y2, r, g, b, a);
		}
	}
	else 
	{
		if (y1 > y2) 
		{
			DrawLineHigh(x2, y2, x1, y1, r, g, b, a);
		}
		else 
		{
			DrawLineHigh(x1, y1, x2, y2, r, g, b, a);
		}
	}
}

void GCImage::DrawLineLow(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	int dx = x2 - x1;
	int dy = y2 - y1;
	int yi = 1;
	if (dy < 0) 
	{
		yi = -1;
		dy = -dy;
	}
	int D = 2 * dy - dx;
	int y = y1;

	for (int x = x1; x <= x2; ++x) 
	{
		SetPixel(x, y, r, g, b, a);
		if (D > 0) {
			y += yi;
			D -= 2 * dx;
		}
		D += 2 * dy;
	}
}

void GCImage::DrawLineHigh(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	int dx = x2 - x1;
	int dy = y2 - y1;
	int xi = 1;
	if (dx < 0) 
	{
		xi = -1;
		dx = -dx;
	}
	int D = 2 * dx - dy;
	int x = x1;

	for (int y = y1; y <= y2; ++y) 
	{
		SetPixel(x, y, r, g, b, a);
		if (D > 0 )
		{
			x += xi;
			D -= 2 * dy;
		}
		D += 2 * dx;
	}
}

void GCImage::DrawRect(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	DrawLine(x, y, x + w - 1, y, r, g, b, a);
	DrawLine(x + w - 1, y, x + w - 1, y + h - 1, r, g, b, a);
	DrawLine(x + w - 1, y + h - 1, x, y + h - 1, r, g, b, a);
	DrawLine(x, y + h - 1, x, y, r, g, b, a);
}

void GCImage::FillRect(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	for (int i = y; i < y + h; ++i)
	{
		for (int j = x; j < x + w; ++j) 
		{
			SetPixel(j, i, r, g, b, a);
		}
	}
}

void GCImage::DrawCircle(int x, int y, int radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x1 = 0;
	int y1 = radius;

	SetPixel(x, y + radius, r, g, b, a);
	SetPixel(x, y - radius, r, g, b, a);
	SetPixel(x + radius, y, r, g, b, a);
	SetPixel(x - radius, y, r, g, b, a);

	while (x1 < y1)
	{
		if (f >= 0)
		{
			y1--;
			ddF_y += 2;
			f += ddF_y;
		}
		x1++;
		ddF_x += 2;
		f += ddF_x;

		SetPixel(x + x1, y + y1, r, g, b, a);
		SetPixel(x - x1, y + y1, r, g, b, a);
		SetPixel(x + x1, y - y1, r, g, b, a);
		SetPixel(x - x1, y - y1, r, g, b, a);
		SetPixel(x + y1, y + x1, r, g, b, a);
		SetPixel(x - y1, y + x1, r, g, b, a);
		SetPixel(x + y1, y - x1, r, g, b, a);
		SetPixel(x - y1, y - x1, r, g, b, a);
	}

}

void GCImage::FillCircle(int x, int y, int radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x1 = 0;
	int y1 = radius;

	for (int i = y - radius; i <= y + radius; i++) 
	{
		SetPixel(x, i, r, g, b, a);
	}

	while (x1 < y1)
	{
		if (f >= 0) 
		{
			y1--;
			ddF_y += 2;
			f += ddF_y;
		}
		x1++;
		ddF_x += 2;
		f += ddF_x;

		for (int i = y - y1; i <= y + y1; i++) 
		{
			SetPixel(x + x1, i, r, g, b, a);
			SetPixel(x - x1, i, r, g, b, a);
			SetPixel(x + y1, i, r, g, b, a);
			SetPixel(x - y1, i, r, g, b, a);
		}
	}
}

void GCImage::Fill(uint8_t r, uint8_t g, uint8_t b, uint8_t a) 
{
	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			SetPixel(x, y, r, g, b, a);
		}
	}
}

bool GCImage::Flip(bool horz, bool vert)
{
	if (!horz && !vert)
	{
		return false;
	}

	int channels = m_bitDepth / 8;
	if (channels != 4)
	{
		std::cerr << "Flip function requires images with 4 channels (RGBA)." << std::endl;
		return false;
	}

	std::vector<uint8_t> tempData(m_size);
	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			int index = (x + y * m_width) * channels;
			int targetIndex = (x + (m_height - 1 - y) * m_width) * channels;
			std::memcpy(&tempData[targetIndex], &m_rgba[index], channels);
		}
	}

	std::memcpy(m_rgba.data(), tempData.data(), m_size);
	return true;
}

bool GCImage::AddHorizontalImage(GCImage* pImg)
{
	if (pImg == nullptr)
	{
		std::cerr << "Image is null." << std::endl;
		return false;
	}

	if (m_height != pImg->m_height)
	{
		std::cerr << "Both images must have the same height." << std::endl;
		return false;
	}

	int channels = m_bitDepth / 8;
	if (channels != 4)
	{
		std::cerr << "AddHorizontalImage function requires images with 4 channels (RGBA)." << std::endl;
		return false;
	}

	std::vector<uint8_t> tempData(m_size + pImg->m_size);
	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			int index = (x + y * m_width) * channels;
			int targetIndex = (x + y * m_width) * channels;
			std::memcpy(&tempData[targetIndex], &m_rgba[index], channels);
		}
		for (int x = 0; x < pImg->m_width; ++x)
		{
			int index = (x + y * pImg->m_width) * channels;
			int targetIndex = (x + (m_width + y) * m_width) * channels;
			std::memcpy(&tempData[targetIndex], &pImg->m_rgba[index], channels);
		}
	}

	m_width += pImg->m_width;
	m_size = m_width * m_height * channels;
	m_rgba.resize(m_size);
	std::memcpy(m_rgba.data(), tempData.data(), m_size);
	return true;

}

bool GCImage::AddVerticalImage(GCImage* pImg)
{
	if (pImg == nullptr)
	{
		std::cerr << "Image is null." << std::endl;
		return false;
	}

	if (m_width != pImg->m_width)
	{
		std::cerr << "Both images must have the same width." << std::endl;
		return false;
	}

	int channels = m_bitDepth / 8;
	if (channels != 4)
	{
		std::cerr << "AddVerticalImage function requires images with 4 channels (RGBA)." << std::endl;
		return false;
	}

	std::vector<uint8_t> tempData(m_size + pImg->m_size);
	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			int index = (x + y * m_width) * channels;
			int targetIndex = (x + y * m_width) * channels;
			std::memcpy(&tempData[targetIndex], &m_rgba[index], channels);
		}
	}
	for (int y = 0; y < pImg->m_height; ++y)
	{
		for (int x = 0; x < pImg->m_width; ++x)
		{
			int index = (x + y * pImg->m_width) * channels;
			int targetIndex = (x + (y + m_height) * m_width) * channels;
			std::memcpy(&tempData[targetIndex], &pImg->m_rgba[index], channels);
		}
	}

	m_height += pImg->m_height;
	m_size = m_width * m_height * channels;
	m_rgba.resize(m_size);
	std::memcpy(m_rgba.data(), tempData.data(), m_size);
	return true;
}

bool GCImage::SetAlpha(uint8_t alpha)
{
	int channels = m_bitDepth / 8;
	if (channels != 4)
	{
		std::cerr << "SetAlpha function requires images with 4 channels (RGBA)." << std::endl;
		return false;
	}

	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			int index = (x + y * m_width) * channels;
			m_rgba[index + 3] = alpha;
		}
	}
	return true;
}

bool GCImage::SetAlphaForColor(uint8_t alpha, COLORREF colorToFind)
{
	int channels = m_bitDepth / 8;
	if (channels != 4)
	{
		std::cerr << "SetAlphaForColor function requires images with 4 channels (RGBA)." << std::endl;
		return false;
	}

	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			int index = (x + y * m_width) * channels;
			if (GetPixel(x, y) == colorToFind)
			{
				m_rgba[index + 3] = alpha;
			}
		}
	}
	return true;
}

bool GCImage::Rotate(int angle)
{
	if (angle % 90 != 0)
	{
		std::cerr << "Angle must be a multiple of 90." << std::endl;
		return false;
	}

	int channels = m_bitDepth / 8;
	if (channels != 4)
	{
		std::cerr << "Rotate function requires images with 4 channels (RGBA)." << std::endl;
		return false;
	}

	std::vector<uint8_t> tempData(m_size);
	if (angle == 90)
	{
		for (int y = 0; y < m_height; ++y)
		{
			for (int x = 0; x < m_width; ++x)
			{
				int index = (x + y * m_width) * channels;
				int targetIndex = ((m_height - 1 - y) + x * m_height) * channels;
				std::memcpy(&tempData[targetIndex], &m_rgba[index], channels);
			}
		}
		int temp = m_height;
		m_height = m_width;
		m_width = temp;
	}
	else if (angle == 180)
	{
		for (int y = 0; y < m_height; ++y)
		{
			for (int x = 0; x < m_width; ++x)
			{
				int index = (x + y * m_width) * channels;
				int targetIndex = ((m_width - 1 - x) + (m_height - 1 - y) * m_width) * channels;
				std::memcpy(&tempData[targetIndex], &m_rgba[index], channels);
			}
		}
	}
	else if (angle == 270)
	{
		for (int y = 0; y < m_height; ++y)
		{
			for (int x = 0; x < m_width; ++x)
			{
				int index = (x + y * m_width) * channels;
				int targetIndex = (y + (m_width - 1 - x) * m_height) * channels;
				std::memcpy(&tempData[targetIndex], &m_rgba[index], channels);
			}
		}
		m_width = m_height;
		m_height = m_width;
	}

	std::memcpy(m_rgba.data(), tempData.data(), m_size);
	return true;
}

bool GCImage::Premultiply()
{
	int channels = m_bitDepth / 8;
	if (channels != 4) {
		std::cerr << "Premultiply function requires images with 4 channels (RGBA)." << std::endl;
		return false;
	}

	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			int index = (x + y * m_width) * channels;

			uint8_t r = m_rgba[index];
			uint8_t g = m_rgba[index + 1];
			uint8_t b = m_rgba[index + 2];
			uint8_t a = m_rgba[index + 3];

			float alpha = a / 255.0f;

			m_rgba[index] = static_cast<uint8_t>(r * alpha);
			m_rgba[index + 1] = static_cast<uint8_t>(g * alpha);
			m_rgba[index + 2] = static_cast<uint8_t>(b * alpha);
		}
	}
	return true;
}

//** It's not a Solidify function -> Need Refacto **//
bool GCImage::Solidify(GCImage* pOpaque)
{
	int channels = m_bitDepth / 8;
	if (channels != 4) {
		std::cerr << "Solidify function requires images with 4 channels (RGBA)." << std::endl;
		return false;
	}

	if (pOpaque == nullptr) {
		std::cerr << "Opaque image is null." << std::endl;
		return false;
	}

	if (pOpaque->m_width != m_width || pOpaque->m_height != m_height) {
		std::cerr << "Both images must have the same dimensions." << std::endl;
		return false;
	}

	if (pOpaque->m_bitDepth != 24) {
		std::cerr << "Opaque image must have 24 bits." << std::endl;
		return false;
	}

	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			int index = (x + y * m_width) * channels;

			uint8_t r = m_rgba[index];
			uint8_t g = m_rgba[index + 1];
			uint8_t b = m_rgba[index + 2];
			uint8_t a = m_rgba[index + 3];

			if (a == 0) {
				int opaqueIndex = (x + y * m_width) * 3;
				m_rgba[index] = pOpaque->m_rgba[opaqueIndex];
				m_rgba[index + 1] = pOpaque->m_rgba[opaqueIndex + 1];
				m_rgba[index + 2] = pOpaque->m_rgba[opaqueIndex + 2];
				m_rgba[index + 3] = 255;
			}
		}
	}
	return true;
}

//** It's not a Solidify function -> Need Refacto **//
bool GCImage::Solidify()
{
	int channels = m_bitDepth / 8;
	if (channels != 4) {
		std::cerr << "Solidify function requires images with 4 channels (RGBA)." << std::endl;
		return false;
	}

	for (int y = 0; y < m_height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			int index = (x + y * m_width) * channels;

			uint8_t r = m_rgba[index];
			uint8_t g = m_rgba[index + 1];
			uint8_t b = m_rgba[index + 2];
			uint8_t a = m_rgba[index + 3];

			float alpha = a / 255.0f;

			m_rgba[index] = static_cast<uint8_t>(r / alpha);
			m_rgba[index + 1] = static_cast<uint8_t>(g / alpha);
			m_rgba[index + 2] = static_cast<uint8_t>(b / alpha);
		}
	}
	return true;
}

bool GCImage::Copy(int x, int y, GCImage* pImg, int xsrc, int ysrc, int w, int h)
{
	if (pImg == nullptr) 
	{
		std::cerr << "Source image is null." << std::endl;
		return false;
	}

	if (x < 0 || x >= m_width || y < 0 || y >= m_height) 
	{
		std::cerr << "Invalid target position." << std::endl;
		return false;
	}

	if (xsrc < 0 || xsrc >= pImg->m_width || ysrc < 0 || ysrc >= pImg->m_height) 
	{
		std::cerr << "Invalid source position." << std::endl;
		return false;
	}

	if (w <= 0 || h <= 0) 
	{
		std::cerr << "Invalid width or height." << std::endl;
		return false;
	}

	int channels = m_bitDepth / 8;
	if (channels != pImg->m_bitDepth / 8) 
	{
		std::cerr << "Both images must have the same bit count." << std::endl;
		return false;
	}

	for (int i = 0; i < h; ++i) 
	{
		for (int j = 0; j < w; ++j) 
		{
			int targetX = x + j;
			int targetY = y + i;
			int sourceX = xsrc + j;
			int sourceY = ysrc + i;

			if (targetX < 0 || targetX >= m_width || targetY < 0 || targetY >= m_height) 
			{
				continue;
			}

			if (sourceX < 0 || sourceX >= pImg->m_width || sourceY < 0 || sourceY >= pImg->m_height)
			{
				continue;
			}

			int targetIndex = (targetX + targetY * m_width) * channels;
			int sourceIndex = (sourceX + sourceY * pImg->m_width) * channels;

			m_rgba[targetIndex] = pImg->m_rgba[sourceIndex];
			m_rgba[targetIndex + 1] = pImg->m_rgba[sourceIndex + 1];
			m_rgba[targetIndex + 2] = pImg->m_rgba[sourceIndex + 2];
			if (channels == 4) {
				m_rgba[targetIndex + 3] = pImg->m_rgba[sourceIndex + 3];
			}
		}
	}

	return true;
}

bool GCImage::Copy(int x, int y, GCImage* pImg, const REC2* pRect)
{
	if (pRect == nullptr)
	{
		std::cerr << "Source rectangle is null." << std::endl;
		return false;
	}
	if (pImg == nullptr)
	{
		std::cerr << "Source image is null." << std::endl;
		return false;
	}
	return Copy(x, y, pImg, pRect->x, pRect->y, pRect->width, pRect->height);
}

bool GCImage::Blend(int x, int y, GCImage* pImg, int xsrc, int ysrc, int w, int h)
{
	if (pImg == nullptr)
	{
		std::cerr << "Source image is null." << std::endl;
		return false;
	}

	if (x < 0 || x >= m_width || y < 0 || y >= m_height)
	{
		std::cerr << "Invalid target position." << std::endl;
		return false;
	}

	if (xsrc < 0 || xsrc >= pImg->m_width || ysrc < 0 || ysrc >= pImg->m_height)
	{
		std::cerr << "Invalid source position." << std::endl;
		return false;
	}

	if (w <= 0 || h <= 0)
	{
		std::cerr << "Invalid width or height." << std::endl;
		return false;
	}

	int channels = m_bitDepth / 8;
	if (channels != pImg->m_bitDepth / 8)
	{
		std::cerr << "Both images must have the same bit count." << std::endl;
		return false;
	}

	for (int i = 0; i < h; ++i)
	{
		for (int j = 0; j < w; ++j)
		{
			int targetX = x + j;
			int targetY = y + i;
			int sourceX = xsrc + j;
			int sourceY = ysrc + i;

			if (targetX < 0 || targetX >= m_width || targetY < 0 || targetY >= m_height)
			{
				continue;
			}

			if (sourceX < 0 || sourceX >= pImg->m_width || sourceY < 0 || sourceY >= pImg->m_height)
			{
				continue;
			}

			int targetIndex = (targetX + targetY * m_width) * channels;
			int sourceIndex = (sourceX + sourceY * pImg->m_width) * channels;

			float sourceR = pImg->m_rgba[sourceIndex] / 255.0f;
			float sourceG = pImg->m_rgba[sourceIndex + 1] / 255.0f;
			float sourceB = pImg->m_rgba[sourceIndex + 2] / 255.0f;
			float sourceA = pImg->m_rgba[sourceIndex + 3] / 255.0f;
		}
	}
}

bool GCImage::Blend(int x, int y, GCImage* pSrc, const REC2* pRect)
{
	if (pRect == nullptr)
	{
		std::cerr << "Source rectangle is null." << std::endl;
		return false;
	}
	if (pSrc == nullptr)
	{
		std::cerr << "Source image is null." << std::endl;
		return false;
	}
	return Blend(x, y, pSrc, pRect->x, pRect->y, pRect->width, pRect->height);
}

bool GCImage::Blend(REC2* pRect, GCImage* pSrc, int quality)
{
	if (!pRect || !pSrc)
		return false;

	switch (quality) {
	case DRAWQUALITY_NEAREST:
		break;
	case DRAWQUALITY_BILINEAR:
		break;
	case DRAWQUALITY_BICUBIC:
		break;
	default:
		std::cerr << "Unknown quality type: " << quality << std::endl;
		return false;
	}

	//** Implement Blend function here **//

	return true;
}

bool GCImage::Blend(int x, int y, int w, int h, GCImage* pSrc, int quality)
{
	if (!pSrc)
		return false;

	switch (quality) {
	case DRAWQUALITY_NEAREST:
		break;
	case DRAWQUALITY_BILINEAR:
		break;
	case DRAWQUALITY_BICUBIC:
		break;
	default:
		std::cerr << "Unknown quality type: " << quality << std::endl;
		return false;
	}

	//** Implement Blend function here **//

	return true;
}

bool GCImage::BlendSTD(const GCImage& overlay, float alpha)
{
	if (m_bitDepth != overlay.m_bitDepth) {
		std::cerr << "Both images must have the same bit count." << std::endl;
		return false;
	}

	int channels = m_bitDepth / 8;
	if (channels != 4) {
		std::cerr << "Blend function requires images with 4 channels (RGBA)." << std::endl;
		return false;
	}

	for (int y = 0; y < overlay.m_height; ++y) {
		for (int x = 0; x < overlay.m_width; ++x) {
			int targetX = x;
			int targetY = y;

			if (targetX < 0 || targetX >= m_width || targetY < 0 || targetY >= m_height) {
				continue;
			}

			int overlayIndex = (x + y * overlay.m_width) * channels;
			int baseIndex = (targetX + targetY * m_width) * channels;

			float sourceR = overlay.m_rgba[overlayIndex] / 255.0f;
			float sourceG = overlay.m_rgba[overlayIndex + 1] / 255.0f;
			float sourceB = overlay.m_rgba[overlayIndex + 2] / 255.0f;
			float sourceA = overlay.m_rgba[overlayIndex + 3] / 255.0f;

			float destR = m_rgba[baseIndex] / 255.0f;
			float destG = m_rgba[baseIndex + 1] / 255.0f;
			float destB = m_rgba[baseIndex + 2] / 255.0f;

			float resultR = sourceR + (destR * (1.0f - sourceA));
			float resultG = sourceG + (destG * (1.0f - sourceA));
			float resultB = sourceB + (destB * (1.0f - sourceA));

			m_rgba[baseIndex] = static_cast<uint8_t>(resultR * 255);
			m_rgba[baseIndex + 1] = static_cast<uint8_t>(resultG * 255);
			m_rgba[baseIndex + 2] = static_cast<uint8_t>(resultB * 255);
		}
	}


	return true;
}

bool GCImage::BlendPRE(const GCImage& overlay, float alpha)
{
	if (m_bitDepth != overlay.m_bitDepth) {
		std::cerr << "Both images must have the same bit count." << std::endl;
		return false;
	}

	int channels = m_bitDepth / 8;
	if (channels != 4) {
		std::cerr << "Blend function requires images with 4 channels (RGBA)." << std::endl;
		return false;
	}

	for (int y = 0; y < overlay.m_height; ++y) {
		for (int x = 0; x < overlay.m_width; ++x) {
			int targetX = x;
			int targetY = y;

			if (targetX < 0 || targetX >= m_width || targetY < 0 || targetY >= m_height) {
				continue;
			}

			int overlayIndex = (x + y * overlay.m_width) * channels;
			int baseIndex = (targetX + targetY * m_width) * channels;

			float sourceR = overlay.m_rgba[overlayIndex] / 255.0f;
			float sourceG = overlay.m_rgba[overlayIndex + 1] / 255.0f;
			float sourceB = overlay.m_rgba[overlayIndex + 2] / 255.0f;
			float sourceA = overlay.m_rgba[overlayIndex + 3] / 255.0f;

			float destR = m_rgba[baseIndex] / 255.0f;
			float destG = m_rgba[baseIndex + 1] / 255.0f;
			float destB = m_rgba[baseIndex + 2] / 255.0f;

			float resultR = sourceR + (destR * (1.0f - sourceA));
			float resultG = sourceG + (destG * (1.0f - sourceA));
			float resultB = sourceB + (destB * (1.0f - sourceA));

			m_rgba[baseIndex] = static_cast<uint8_t>(resultR * 255);
			m_rgba[baseIndex + 1] = static_cast<uint8_t>(resultG * 255);
			m_rgba[baseIndex + 2] = static_cast<uint8_t>(resultB * 255);
		}
	}

	return true;
}