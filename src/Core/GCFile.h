#pragma once
#include <string>
#include <vector>



class GCFile
{
public:
	GCFile(const std::string& filename, const char* mode);
	~GCFile();

	FILE* file;
	bool IsOpen() const;

	bool Read(std::vector<uint8_t>& buffer, size_t size);
	bool Write(const std::vector<uint8_t>& buffer);

	int ReadByte();
	bool WriteByte(uint8_t byte);

	void Open(GCFile* file);
	void Close();
	void Leak();
};

