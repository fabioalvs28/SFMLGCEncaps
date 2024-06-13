#include "framework.h"


// Utils 
std::vector<std::string> split(std::string str, std::string delimiter)
{
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
	std::vector<float> floatCoord;

	for (int i = 0; i < pStrCoord->size(); i++)
	{
		floatCoord.push_back(std::stof(pStrCoord->at(i)));
	}

	return floatCoord;
}
//








/// CLASS
GCModelParserObj::GCModelParserObj() {
}

GCModelParserObj::~GCModelParserObj() {
}

void GCModelParserObj::Initialize(GCRender* pRender)
{
	m_pRender = pRender;
}

bool GCModelParserObj::ParseObj(std::string fileName)
{

	std::ifstream objFile(fileName);
	std::string line;

	if (objFile) {

	}
	else {
		return false;
	}

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

			m_ParsedObj.coords.push_back(coordinates);
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

				m_ParsedObj.facesInfos.push_back(tempInfos);
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

			m_ParsedObj.uvs.push_back(uv);
		}
	}

	return true;
}

// TEXTURE
GCGeometry* GCModelParserObj::BuildObjTexture(std::string fileName)
{

	ParseObj(fileName);


	GCGeometry* objGeometry = new GCGeometry();

	objGeometry->indiceNumber = m_ParsedObj.facesInfos.size();
	objGeometry->vertexNumber = m_ParsedObj.coords.size();

	for (int i = 0; i < m_ParsedObj.facesInfos.size(); i++)
	{
		objGeometry->pos.push_back(
			DirectX::XMFLOAT3(m_ParsedObj.coords[m_ParsedObj.facesInfos[i][0]][0], m_ParsedObj.coords[m_ParsedObj.facesInfos[i][0]][1], m_ParsedObj.coords[m_ParsedObj.facesInfos[i][0]][2]));

		objGeometry->texC.push_back(
				DirectX::XMFLOAT2(m_ParsedObj.uvs[m_ParsedObj.facesInfos[i][1]][0], m_ParsedObj.uvs[m_ParsedObj.facesInfos[i][1]][1]) );

		objGeometry->indices.push_back(i);
	}

	return objGeometry;
}

// COLOR

GCGeometry* GCModelParserObj::BuildObjColor(std::string fileName) {

	GCGeometry* objGeometry = new GCGeometry();

	objGeometry->indiceNumber = m_ParsedObj.facesInfos.size();
	objGeometry->vertexNumber = m_ParsedObj.coords.size();

	for (int i = 0; i < m_ParsedObj.coords.size(); i++) {
		objGeometry->pos.push_back(
			DirectX::XMFLOAT3(m_ParsedObj.coords[i][0], m_ParsedObj.coords[i][1], m_ParsedObj.coords[i][2]));

		objGeometry->color.push_back(
			DirectX::XMFLOAT4(DirectX::Colors::White));
	}

	for (int i = 0; i < m_ParsedObj.facesInfos.size(); i++) {
		objGeometry->indices.push_back(m_ParsedObj.facesInfos[i][0]);
	}



	return objGeometry;
}

