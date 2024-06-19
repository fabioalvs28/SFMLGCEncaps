#include "framework.h"

GCModelParser::GCModelParser()
{
}

GCModelParser::~GCModelParser()
{
}

bool GCModelParser::Parse(std::string fileName, Extensions fileExtension)
{
	switch (fileExtension)
	{
	case 0:
		m_ParsedModel = dynamic_cast<GCModelParserObj*>(this)->Parse(fileName);
		return true;
	}
	return false;
}

GCGeometry* GCModelParser::BuildModelTexture(std::string fileName, Extensions fileExtension)
{
	//Parses an obj and puts it into a geometry with textures
	GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();

	std::wstring wideFileName(fileName.begin(), fileName.end());

	if (_waccess(wideFileName.c_str(), 0) == -1)
	{
		OutputDebugString((L"Obj file not found: " + wideFileName + L"\n").c_str());
		profiler.LogWarning("Obj file not found: " + fileName);
	}
	else
	{
		OutputDebugString((L"Obj file: " + wideFileName + L" loaded successfully \n").c_str());
		profiler.LogInfo("Obj file:" + fileName + " loaded successfully");
	}

	Parse(fileName, fileExtension);

	GCGeometry* objGeometry = new GCGeometry();

	objGeometry->indiceNumber = m_ParsedModel->facesInfos.size();
	objGeometry->vertexNumber = m_ParsedModel->coords.size();

	for (int i = 0; i < m_ParsedModel->facesInfos.size(); i++)
	{
		objGeometry->pos.push_back(
			DirectX::XMFLOAT3(m_ParsedModel->coords[m_ParsedModel->facesInfos[i][0]][0], m_ParsedModel->coords[m_ParsedModel->facesInfos[i][0]][1], m_ParsedModel->coords[m_ParsedModel->facesInfos[i][0]][2]));

		objGeometry->texC.push_back(
			DirectX::XMFLOAT2(m_ParsedModel->uvs[m_ParsedModel->facesInfos[i][1]][0], m_ParsedModel->uvs[m_ParsedModel->facesInfos[i][1]][1]));

		objGeometry->indices.push_back(i);
	}

	if (CheckNull(objGeometry))
	{
		OutputDebugString(L"Obj Geometry is empty \n");
		profiler.LogWarning("Obj geometry is empty");
	}
	return objGeometry;
}

GCGeometry* GCModelParser::BuildModelColor(std::string fileName, Extensions fileExtension)
{
	//Parses an obj and puts it into a geometry with colors
	GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();

	std::wstring wideFileName(fileName.begin(), fileName.end());

	if (_waccess(wideFileName.c_str(), 0) == -1)
	{
		OutputDebugString((L"Obj file not found: " + wideFileName + L"\n").c_str());
		profiler.LogWarning("Obj file not found: " + fileName);
	}
	else
	{
		OutputDebugString((L"Obj file : " + wideFileName + L" loaded successfully \n").c_str());
		profiler.LogInfo("Obj file:" + fileName + " loaded successfully");
	}

	Parse(fileName, fileExtension);

	GCGeometry* objGeometry = new GCGeometry();

	objGeometry->indiceNumber = m_ParsedModel->facesInfos.size();
	objGeometry->vertexNumber = m_ParsedModel->coords.size();

	for (int i = 0; i < m_ParsedModel->coords.size(); i++) {
		objGeometry->pos.push_back(
			DirectX::XMFLOAT3(m_ParsedModel->coords[i][0], m_ParsedModel->coords[i][1], m_ParsedModel->coords[i][2]));

		objGeometry->color.push_back(
			DirectX::XMFLOAT4(DirectX::Colors::White));
	}

	for (int i = 0; i < m_ParsedModel->facesInfos.size(); i++) {
		objGeometry->indices.push_back(m_ParsedModel->facesInfos[i][0]);
	}

	if (CheckNull(objGeometry))
	{
		OutputDebugString(L"Obj Geometry is empty \n");
		profiler.LogWarning("Obj geometry is empty");
	}
	return objGeometry;
}
