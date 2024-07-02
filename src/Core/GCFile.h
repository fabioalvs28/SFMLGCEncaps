#pragma once
#include <string>
#include <vector>
#include "GCUtilities.h"




class GCFile
{
public:
	GCFile(const std::string& filename);
	~GCFile();

	FILE* file;
	bool IsOpen() const;

	bool Read(std::vector<uint8_t>& buffer, size_t size);
	bool Write(const std::vector<uint8_t>& buffer);
	bool Write(const BYTE* buffer, size_t size);



	int ReadByte();
	bool WriteByte(uint8_t byte);

	void Open(GCFile* file);
	void Close();
	void Leak();

private:
	const char* mode = "rw";
};

