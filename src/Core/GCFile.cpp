#include "pch.h"
#include "GCFile.h"
#include <cstdio>
#include <iostream>
#include <fstream>

GCFile::GCFile(const std::string& filename, const char* mode)
{
	file = nullptr;

	if (fopen_s(&file, filename.c_str(), mode) != 0) {
		file = nullptr;
	}
}

GCFile::~GCFile()
{
	Close();
}

void GCFile::Open(GCFile* file)
{
	if (file)
	{
		Close();
		this->file = file->file;
		file->file = nullptr;
	}
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

bool GCFile::IsOpen() const
{
	return file != nullptr;
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

bool GCFile::WriteByte(uint8_t byte)
{
	if (file == nullptr)
		return -1;

	return fputc(byte, file) != EOF;
}
