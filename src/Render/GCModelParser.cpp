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
		CheckExtension(fileName, "obj");
		m_ParsedModel = dynamic_cast<GCModelParserObj*>(this)->Parse(fileName);
		return true;
	}
	return false;
}

GCGeometry* GCModelParser::BuildModelTexture(std::string fileName, Extensions fileExtension)
{

	CheckFile(fileName, ("Model file not found: " + fileName), ("Model file:" + fileName + " loaded successfully"));

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

	GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();
	CHECK_POINTERSNULL(profiler, "Model geometry loaded successfully","Model Geometry is empty", objGeometry);

	return objGeometry;
}

GCGeometry* GCModelParser::BuildModelColor(std::string fileName, Extensions fileExtension)
{

	CheckFile(fileName, ("Model file not found: " + fileName), ("Model file:" + fileName + " loaded successfully"));

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

	GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();
	CHECK_POINTERSNULL(profiler, "Model geometry loaded successfully", "Model Geometry is empty", objGeometry);

	return objGeometry;
}
