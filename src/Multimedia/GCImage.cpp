#include "pch.h"
#include "GCImage.h"
#include "BMPHeader.h"
#include "GCFile.h"
#include "lib_lodepng.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring>


GCImage::GCImage(const GCImage& img)
{
	rgba = img.rgba;
	m_width = img.m_width;
	height = img.height;
	bitCount = img.bitCount;
	channels = img.channels;
	rowPadded = img.rowPadded;
	size = img.size;
	data = img.data;
}

GCImage::~GCImage()
{
	Close();
}

GCImage& GCImage::operator=(const GCImage& img)
{
	if (this != &img) {
		m_width = img.m_width;
		height = img.height;
		bitCount = img.bitCount;
		channels = img.channels;
		data = img.data;
	}
	return *this;
}

bool GCImage::Load(GCImage* img)
{
	m_width = img->m_width;
	height = img->height;
	bitCount = img->bitCount;
	channels = img->channels;
	data = img->data;
	return true;
}

bool GCImage::LoadBMP(const std::string& filename)
{
	GCFile file(filename);
	if (!file.IsOpen()) {
		std::cerr << "Error opening file: " << filename << std::endl;
		return false;
	}

	BMPHeader header;
	std::vector<uint8_t> headerBuffer(sizeof(header));
	if (!file.Read(headerBuffer, sizeof(header))) {
		std::cerr << "Error reading header from file: " << filename << std::endl;
		return false;
	}
	std::memcpy(&header, headerBuffer.data(), sizeof(header));

	m_width = header.biWidth;
	height = header.biHeight;
	bitCount = header.biBitCount;
	channels = bitCount / 8;
	data.resize(m_width * height * channels);

	if (fseek(file.file, header.bfOffBits, SEEK_SET) != 0) {
		std::cerr << "Error seeking to pixel data in file: " << filename << std::endl;
		return false;
	}

	uint32_t padding = rowStride() - m_width * channels;
	std::vector<uint8_t> rowData(rowStride());

	for (int y = 0; y < height; ++y) {
		if (!file.Read(rowData, rowStride())) {
			std::cerr << "Error reading pixel data from file: " << filename << std::endl;
			return false;
		}
		std::memcpy(&data[(height - 1 - y) * m_width * channels], rowData.data(), m_width * channels);
	}

	return true;
}

bool GCImage::LoadPNG(BYTE* buffer, int size)
{
	if (buffer == nullptr || size <= 0)
		return false;

	Close();
	
	lodepng::State state;
	UI32 pngWidth, pngHeight;
	if (lodepng_inspect(&pngWidth, &pngHeight, &state, buffer, size))
		return false;
	switch (state.info_png.color.colortype)
	{
	case LCT_RGBA:
	case LCT_GREY_ALPHA:
	case LCT_PALETTE:
		bitCount = 32;
		break;
	case LCT_RGB:
	case LCT_GREY:
		bitCount = 24;
		break;
	default:
		return false;
		break;
	}

	UI32 error = lodepng_decode32(&rgba, (UI32*)&m_width, (UI32*)&height, buffer, size);
	if (error)
	{
		Close();
		return false;
	}

	rowPadded = m_width * 4;
	size = rowPadded * height;
	bitCount = m_width * height;
	return true;

}

bool GCImage::SaveBMP(const std::string& filename)
{
	GCFile file(filename);
	if (!file.IsOpen()) {
		std::cerr << "Error opening file: " << filename << std::endl;
		return false;
	}

	BMPHeader fileHeader;

	fileHeader.bfType = 0x4D42;
	fileHeader.bfSize = sizeof(BMPHeader) + rowStride() * height;
	fileHeader.bfReserved1 = 0;
	fileHeader.bfReserved2 = 0;
	fileHeader.bfOffBits = sizeof(BMPHeader);
	fileHeader.biSize = 40;
	fileHeader.biWidth = m_width;
	fileHeader.biHeight = height;
	fileHeader.biPlanes = 1;
	fileHeader.biBitCount = bitCount;
	fileHeader.biCompression = 0;
	fileHeader.biSizeImage = 0;
	fileHeader.biXPelsPerMeter = 0;
	fileHeader.biYPelsPerMeter = 0;
	fileHeader.biClrUsed = 0;
	fileHeader.biClrImportant = 0;

	std::vector<uint8_t> headerBuffer(reinterpret_cast<uint8_t*>(&fileHeader), reinterpret_cast<uint8_t*>(&fileHeader) + sizeof(fileHeader));
	if (!file.Write(headerBuffer)) {
		std::cerr << "Error writing header to file: " << filename << std::endl;
		return false;
	}

	int channels = bitCount / 8;
	uint32_t padding = rowStride() - m_width * channels;
	for (int y = 0; y < height; ++y) {
		if (!file.Write(std::vector<uint8_t>(data.begin() + y * m_width * channels, data.begin() + (y + 1) * m_width * channels))) {
			std::cerr << "Error writing pixel data to file: " << filename << std::endl;
			return false;
		}
		if (padding > 0) {
			std::vector<uint8_t> paddingBuffer(padding, 0);
			if (!file.Write(paddingBuffer)) {
				std::cerr << "Error writing padding to file: " << filename << std::endl;
				return false;
			}
		}
	}


	return true;
}

void GCImage::Close()
{
	data.clear();
	m_width = 0;
	height = 0;
	bitCount = 0;
	channels = 0;
	rowPadded = 0;

}

void GCImage::CreateEmptyImage(int w, int h, int bpp)
{
	m_width = w;
	height = h;
	bitCount = bpp;
	channels = bitCount / 8;
	data.resize(m_width * height * channels, 0);
}

void GCImage::SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	if (x < 0 || x >= m_width || y < 0 || y >= height) return;
	int index = (x + y * m_width) * channels;
	data[index] = r;
	data[index + 1] = g;
	data[index + 2] = b;

}


bool GCImage::SavePNG(GCFile* file, int* pOutSize, bool gray)
{
	if (file == nullptr || rgba == nullptr)
		return false;

	BYTE* png = nullptr;
	size_t size = 0;
	UI32 error = lodepng_encodeEx(&png, &size, rgba, m_width, height, bitCount, gray);
	if (error)
		return false;

	file->Write(png, (int)size);
	DELPTRS(png);

	if (pOutSize)
		*pOutSize = (int)size;
	return true;

}

void GCImage::DrawLine(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	if (abs(y2 - y1) < abs(x2 - x1)) {
		if (x1 > x2) {
			DrawLineLow(x2, y2, x1, y1, r, g, b, a);
		}
		else {
			DrawLineLow(x1, y1, x2, y2, r, g, b, a);
		}
	}
	else {
		if (y1 > y2) {
			DrawLineHigh(x2, y2, x1, y1, r, g, b, a);
		}
		else {
			DrawLineHigh(x1, y1, x2, y2, r, g, b, a);
		}
	}
}

void GCImage::DrawLineLow(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	int dx = x2 - x1;
	int dy = y2 - y1;
	int yi = 1;
	if (dy < 0) {
		yi = -1;
		dy = -dy;
	}
	int D = 2 * dy - dx;
	int y = y1;

	for (int x = x1; x <= x2; ++x) {
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
	if (dx < 0) {
		xi = -1;
		dx = -dx;
	}
	int D = 2 * dx - dy;
	int x = x1;

	for (int y = y1; y <= y2; ++y) {
		SetPixel(x, y, r, g, b, a);
		if (D > 0) {
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
	for (int i = y; i < y + h; ++i) {
		for (int j = x; j < x + w; ++j) {
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

	while (x1 < y1) {
		if (f >= 0) {
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

	for (int i = y - radius; i <= y + radius; i++) {
		SetPixel(x, i, r, g, b, a);
	}

	while (x1 < y1) {
		if (f >= 0) {
			y1--;
			ddF_y += 2;
			f += ddF_y;
		}
		x1++;
		ddF_x += 2;
		f += ddF_x;

		for (int i = y - y1; i <= y + y1; i++) {
			SetPixel(x + x1, i, r, g, b, a);
			SetPixel(x - x1, i, r, g, b, a);
			SetPixel(x + y1, i, r, g, b, a);
			SetPixel(x - y1, i, r, g, b, a);
		}
	}
}

void GCImage::Fill(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			SetPixel(x, y, r, g, b, a);
		}
	}
}

void GCImage::InverseBMP(const std::string& filename)
{
	LoadBMP(filename);
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			int index = (x + y * m_width) * channels;
			data[index] = data[index];
			data[index + 1] = data[index + 1];
			data[index + 2] = data[index + 2];
		}
	}
}

bool GCImage::CopyBMP(const std::string& filename, const std::string& newFilename)
{
	LoadBMP(filename);
	SaveBMP(newFilename);
	return true;
}

bool GCImage::Premultiply()
{
	int channels = bitCount / 8;
	if (channels != 4) {
		std::cerr << "Premultiply function requires images with 4 channels (RGBA)." << std::endl;
		return false;
	}

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < m_width; ++x) {
			int index = (x + y * m_width) * channels;

			uint8_t r = data[index];
			uint8_t g = data[index + 1];
			uint8_t b = data[index + 2];
			uint8_t a = data[index + 3];

			float alpha = a / 255.0f;

			data[index] = static_cast<uint8_t>(r * alpha);
			data[index + 1] = static_cast<uint8_t>(g * alpha);
			data[index + 2] = static_cast<uint8_t>(b * alpha);
		}
	}
	return true;
}

bool GCImage::BlendSTD(const GCImage& overlay, float alpha)
{
	if (bitCount != overlay.bitCount) {
		std::cerr << "Both images must have the same bit count." << std::endl;
		return false;
	}

	int channels = bitCount / 8;
	if (channels != 4) {
		std::cerr << "Blend function requires images with 4 channels (RGBA)." << std::endl;
		return false;
	}

	for (int y = 0; y < overlay.height; ++y) {
		for (int x = 0; x < overlay.m_width; ++x) {
			int targetX = x;
			int targetY = y;

			if (targetX < 0 || targetX >= m_width || targetY < 0 || targetY >= height) {
				continue;
			}

			int overlayIndex = (x + y * overlay.m_width) * channels;
			int baseIndex = (targetX + targetY * m_width) * channels;

			float sourceR = overlay.data[overlayIndex] / 255.0f;
			float sourceG = overlay.data[overlayIndex + 1] / 255.0f;
			float sourceB = overlay.data[overlayIndex + 2] / 255.0f;
			float sourceA = overlay.data[overlayIndex + 3] / 255.0f;

			float destR = data[baseIndex] / 255.0f;
			float destG = data[baseIndex + 1] / 255.0f;
			float destB = data[baseIndex + 2] / 255.0f;

			float resultR = sourceR + (destR * (1.0f - sourceA));
			float resultG = sourceG + (destG * (1.0f - sourceA));
			float resultB = sourceB + (destB * (1.0f - sourceA));

			data[baseIndex] = static_cast<uint8_t>(resultR * 255);
			data[baseIndex + 1] = static_cast<uint8_t>(resultG * 255);
			data[baseIndex + 2] = static_cast<uint8_t>(resultB * 255);
		}
	}


	return true;
}

bool GCImage::BlendPRE(const GCImage& overlay, float alpha)
{
	if (bitCount != overlay.bitCount) {
		std::cerr << "Both images must have the same bit count." << std::endl;
		return false;
	}

	int channels = bitCount / 8;
	if (channels != 4) {
		std::cerr << "Blend function requires images with 4 channels (RGBA)." << std::endl;
		return false;
	}

	for (int y = 0; y < overlay.height; ++y) {
		for (int x = 0; x < overlay.m_width; ++x) {
			int targetX = x;
			int targetY = y;

			if (targetX < 0 || targetX >= m_width || targetY < 0 || targetY >= height) {
				continue;
			}

			int overlayIndex = (x + y * overlay.m_width) * channels;
			int baseIndex = (targetX + targetY * m_width) * channels;

			float sourceR = overlay.data[overlayIndex] / 255.0f;
			float sourceG = overlay.data[overlayIndex + 1] / 255.0f;
			float sourceB = overlay.data[overlayIndex + 2] / 255.0f;
			float sourceA = overlay.data[overlayIndex + 3] / 255.0f;

			float destR = data[baseIndex] / 255.0f;
			float destG = data[baseIndex + 1] / 255.0f;
			float destB = data[baseIndex + 2] / 255.0f;

			float resultR = sourceR + (destR * (1.0f - sourceA));
			float resultG = sourceG + (destG * (1.0f - sourceA));
			float resultB = sourceB + (destB * (1.0f - sourceA));

			data[baseIndex] = static_cast<uint8_t>(resultR * 255);
			data[baseIndex + 1] = static_cast<uint8_t>(resultG * 255);
			data[baseIndex + 2] = static_cast<uint8_t>(resultB * 255);
		}
	}

	return true;
}