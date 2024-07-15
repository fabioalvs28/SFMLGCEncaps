#pragma once
#include <string>
#include <vector>
#include "GCUtilities.h"


class GCFile
{
private:

public:
	std::string filename;
	const char* mode;


	GCFile(const char* filename, const char* mode);
	~GCFile();

	bool IsOpen() const;

	bool Read(std::vector<uint8_t>& buffer, size_t size);
	bool Write(const std::vector<uint8_t>& buffer);
	bool Write(const BYTE* buffer, size_t size);



	int ReadByte();
	bool WriteByte(uint8_t byte);

	void Close();
	void Leak();

	FILE* file;
};

