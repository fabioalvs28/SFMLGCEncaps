#pragma once



struct HLSLFile {
	std::wstring fileName;
	std::string content;

	HLSLFile(const std::wstring& name) : fileName(name) {
		readFile();
	}

private:
	void readFile() {
		std::ifstream fileStream(fileName);
		if (fileStream.is_open()) {
			std::stringstream buffer;
			buffer << fileStream.rdbuf();
			content = buffer.str();
		}
	}
};