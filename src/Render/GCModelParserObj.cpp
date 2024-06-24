#include "framework.h"

std::vector<std::string> split(std::string str, std::string delimiter)
{
	//Util splits a vector at delimiter
	std::vector<std::string> v;
	if (!str.empty()) {
		int start = 0;
		do {
			// Find the index of occurrence
			int idx = str.find(delimiter, start);
			if (idx == std::string::npos) {
				break;
			}

			// If found add the substring till that
			// occurrence in the vector
			int length = idx - start;
			v.push_back(str.substr(start, length));
			start += (length + delimiter.size());
		} while (true);
		v.push_back(str.substr(start));
	}

	return v;
}

std::vector<float> getFloatCoordinates(std::vector<std::string>* pStrCoord)
{
	//Util changes string coords to floats 
	std::vector<float> floatCoord;

	for (int i = 0; i < pStrCoord->size(); i++)
	{
		floatCoord.push_back(std::stof(pStrCoord->at(i)));
	}

	return floatCoord;
}

GCModelParserObj::GCModelParserObj() 
{
}

GCModelParserObj::~GCModelParserObj() 
{
}

ModelInfos* GCModelParserObj::Parse(std::string filePath)
{
	//Parses the file into a vector with the coordinates, the triangles and the uvs
	std::ifstream objFile(filePath);
	std::string line;

	ModelInfos* parsedModel = new ModelInfos();

	while (!objFile.eof()) {

		std::getline(objFile, line);

		if (line[0] == 'v' && line[1] == ' ') //vertex coordinates
		{

			std::string templine;

			for (int i = 2; i < line.size(); i++)
			{
				templine.push_back(line[i]);
			}

			line = templine;

			std::vector<std::string> strCoord = split(line, " ");

			std::vector<float> coordinates = getFloatCoordinates(&strCoord);

			parsedModel->coords.push_back(coordinates);
		}

		else if (line[0] == 'f') //face triangles
		{
			std::string templine;

			for (int i = 2; i < line.size(); i++)
			{
				templine.push_back(line[i]);
			}

			line = templine;

			std::vector<std::string> strFace = split(line, " ");

			std::vector<std::vector<std::string>> strVertInfos;

			for (int i = 0; i < strFace.size(); i++)
			{
				strVertInfos.push_back(split(strFace[i], "/"));
			}

			for (int i = 0; i < strVertInfos.size(); i++)
			{
				std::vector<std::uint16_t> tempInfos;

				for (int j = 0; j < strVertInfos[i].size(); j++)
					tempInfos.push_back(static_cast<uint16_t>(std::stoi(strVertInfos[i][j]) - 1));

				parsedModel->facesInfos.push_back(tempInfos);
			}
		}

		else if (line[0] == 'v' && line[1] == 't') //UVs
		{
			std::string templine;

			for (int i = 3; i < line.size(); i++)
			{
				templine.push_back(line[i]);
			}

			line = templine;

			std::vector<std::string> strUv = split(line, " ");

			std::vector<float> uv = getFloatCoordinates(&strUv);

			parsedModel->uvs.push_back(uv);
		}

		else if (line[0] == 'v' && line[1] == 'n') //Normals
		{
			std::string templine;

			for (int i = 3; i < line.size(); i++)
			{
				templine.push_back(line[i]);
			}

			line = templine;

			std::vector<std::string> strNormals = split(line, " ");

			std::vector<float> normals = getFloatCoordinates(&strNormals);

			parsedModel->normals.push_back(normals);
		}
	}

	return parsedModel;
}