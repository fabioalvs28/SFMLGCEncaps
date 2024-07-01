#include "pch.h"

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
		CHECK_EXTENSION(fileName, "obj");
		m_ParsedModel = dynamic_cast<GCModelParserObj*>(this)->Parse(fileName);
		return true;
	}
	return false;
}

bool GCModelParser::BuildModel(std::string fileName, DirectX::XMFLOAT4 color, Extensions fileExtension, GCGeometry* pGeometry)
{

	CHECK_FILE(fileName, ("Model file not found: " + fileName), ("Model file:" + fileName + " loaded successfully"));

	Parse(fileName, fileExtension);

	pGeometry->indiceNumber = m_ParsedModel->facesInfos.size();
	pGeometry->vertexNumber = m_ParsedModel->coords.size();

	for (int i = 0; i < m_ParsedModel->facesInfos.size(); i++)
	{
		pGeometry->pos.push_back(
			DirectX::XMFLOAT3(m_ParsedModel->coords[m_ParsedModel->facesInfos[i][0]][0], m_ParsedModel->coords[m_ParsedModel->facesInfos[i][0]][1], m_ParsedModel->coords[m_ParsedModel->facesInfos[i][0]][2]));
		
		pGeometry->indices.push_back(i);
	}

	/*if (HAS_FLAG(flagEnabledBits, HAS_UV)) 
	{*/
	for (int i = 0; i < m_ParsedModel->facesInfos.size(); i++)
	{
		pGeometry->uv.push_back(
			DirectX::XMFLOAT2(m_ParsedModel->uvs[m_ParsedModel->facesInfos[i][1]][0], m_ParsedModel->uvs[m_ParsedModel->facesInfos[i][1]][1]));
	}
	//}
	//if (HAS_FLAG(flagEnabledBits, HAS_COLOR)) 
	//{
	for (int i = 0; i < m_ParsedModel->facesInfos.size(); i++) 
	{
		pGeometry->color.push_back(
			DirectX::XMFLOAT4(color));
	}
	//}
	//if (HAS_FLAG(flagEnabledBits, HAS_NORMAL)) 
	//{
	for (int i = 0; i < m_ParsedModel->facesInfos.size(); i++)
	{
		pGeometry->normals.push_back(
			DirectX::XMFLOAT3(m_ParsedModel->normals[m_ParsedModel->facesInfos[i][2]][0], m_ParsedModel->normals[m_ParsedModel->facesInfos[i][2]][1], m_ParsedModel->normals[m_ParsedModel->facesInfos[i][2]][2])
		);
	}
	//}

	CHECK_POINTERSNULL("Model geometry loaded successfully","Model Geometry is empty", pGeometry);

	return true;
}

