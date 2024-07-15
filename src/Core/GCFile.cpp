#include "pch.h"
#include "GCFile.h"
#include <cstdio>
#include <iostream>
#include <fstream>

GCFile::GCFile(const char* filename, const char* mode)
{
	fopen_s(&file, filename, mode);
	if (file)
	{
		this->filename = filename;
	}
	else
	{
		std::cerr << "Error opening file: " << filename << std::endl;
	}
}

GCFile::~GCFile()
{
	Close();
}


bool GCFile::IsOpen() const
{
	if (!file) {
		std::cerr << "Error saving file: " << filename << std::endl;
		return false;
	}
	return file != nullptr;
}

void GCFile::Close()
{
	if (file)
	{
		fclose(file);
		file = nullptr;
	}
}

void GCFile::Leak()
{
	file = nullptr;
}

bool GCFile::Read(std::vector<uint8_t>& buffer, size_t size)
{
	if (file == nullptr)
		return false;

	buffer.resize(size);
	size_t bytesRead = fread(buffer.data(), 1, size, file);
	return bytesRead == size;
}

int GCFile::ReadByte()
{
	if (file == nullptr)
		return -1;

	return fgetc(file);
}

bool GCFile::Write(const std::vector<uint8_t>& buffer)
{
	if (!file)
		return false;

	size_t bytesWritten = fwrite(buffer.data(), 1, buffer.size(), file);
	return bytesWritten == buffer.size();

}

bool GCFile::Write(const BYTE* buffer, size_t size)
{
	if (!file)
		return false;

	size_t bytesWritten = fwrite(buffer, 1, size, file);
	return bytesWritten == size;
}

bool GCFile::WriteByte(uint8_t byte)
{
	if (file == nullptr)
		return false;

	return fputc(byte, file) != EOF;
}
