#include "pch.h"

GCModelParserFactory::GCModelParserFactory()
	: m_ParsedModel(nullptr)
{
	m_filePath.clear();
}

GCModelParserFactory::~GCModelParserFactory()
{
}

bool GCModelParserFactory::Parse(std::string fileName, GC_EXTENSIONS fileExtension)
{

	switch (fileExtension)
	{
	case 0:
		if (CHECK_EXTENSION(fileName, "obj") == false) return false;

		m_ParsedModel = dynamic_cast<GCModelParserObj*>(this)->Parse(fileName);

		return true;
	}
	return false;
}

GC_GRAPHICS_ERROR GCModelParserFactory::BuildModel(std::string fileName, DirectX::XMFLOAT4 color, GC_EXTENSIONS fileExtension, GCGeometry* pGeometry)
{
	if (CHECK_POINTERSNULL("Model geometry loaded successfully", "Model Geometry is empty", pGeometry) == false)
		return GCRENDER_ERROR_POINTER_NULL;
	if (CHECK_FILE(fileName, ("Model file not found: " + fileName), ("Model file:" + fileName + " loaded successfully")) == false)
		return GCRENDER_ERROR_FILEPATH_NOT_FOUND;



	if (Parse(fileName, fileExtension) == false)
		return GCRENDER_ERROR_BAD_EXTENSION;

	pGeometry->indiceNumber = m_ParsedModel->facesInfos.size();
	pGeometry->vertexNumber = m_ParsedModel->coords.size();

	for (int i = 0; i < m_ParsedModel->facesInfos.size(); i++)
	{
		pGeometry->pos.push_back(
			DirectX::XMFLOAT3(m_ParsedModel->coords[m_ParsedModel->facesInfos[i][0]][0], m_ParsedModel->coords[m_ParsedModel->facesInfos[i][0]][1], m_ParsedModel->coords[m_ParsedModel->facesInfos[i][0]][2]));
		
		pGeometry->indices.push_back(i);
	}

	for (int i = 0; i < m_ParsedModel->facesInfos.size(); i++)
	{
		pGeometry->uv.push_back(
			DirectX::XMFLOAT2(m_ParsedModel->uvs[m_ParsedModel->facesInfos[i][1]][0], m_ParsedModel->uvs[m_ParsedModel->facesInfos[i][1]][1]));
	}

	for (int i = 0; i < m_ParsedModel->facesInfos.size(); i++) 
	{
		pGeometry->color.push_back(
			DirectX::XMFLOAT4(color));
	}

	for (int i = 0; i < m_ParsedModel->facesInfos.size(); i++)
	{
		pGeometry->normals.push_back(
			DirectX::XMFLOAT3(m_ParsedModel->normals[m_ParsedModel->facesInfos[i][2]][0], m_ParsedModel->normals[m_ParsedModel->facesInfos[i][2]][1], m_ParsedModel->normals[m_ParsedModel->facesInfos[i][2]][2])
		);
	}

	return GCRENDER_SUCCESS_OK;
}

