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
		CHECK_EXTENSION(fileName, "obj");
		m_ParsedModel = dynamic_cast<GCModelParserObj*>(this)->Parse(fileName);
		return true;
	}
	return false;
}

GCGeometry* GCModelParser::BuildModel(std::string fileName, DirectX::XMFLOAT4 color, Extensions fileExtension, int& flagEnabledBits)
{

	CHECK_FILE(fileName, ("Model file not found: " + fileName), ("Model file:" + fileName + " loaded successfully"));

	Parse(fileName, fileExtension);

	GCGeometry* objGeometry = new GCGeometry();

	objGeometry->indiceNumber = m_ParsedModel->facesInfos.size();
	objGeometry->vertexNumber = m_ParsedModel->coords.size();

	if (HAS_FLAG(flagEnabledBits, HAS_UV)) {

		for (int i = 0; i < m_ParsedModel->facesInfos.size(); i++)
		{
			objGeometry->pos.push_back(
				DirectX::XMFLOAT3(m_ParsedModel->coords[m_ParsedModel->facesInfos[i][0]][0], m_ParsedModel->coords[m_ParsedModel->facesInfos[i][0]][1], m_ParsedModel->coords[m_ParsedModel->facesInfos[i][0]][2]));

			objGeometry->uv.push_back(
				DirectX::XMFLOAT2(m_ParsedModel->uvs[m_ParsedModel->facesInfos[i][1]][0], m_ParsedModel->uvs[m_ParsedModel->facesInfos[i][1]][1]));

			objGeometry->indices.push_back(i);
		}
	}
	if (HAS_FLAG(flagEnabledBits, HAS_COLOR)) {
		for (int i = 0; i < m_ParsedModel->coords.size(); i++) {
			objGeometry->pos.push_back(
				DirectX::XMFLOAT3(m_ParsedModel->coords[i][0], m_ParsedModel->coords[i][1], m_ParsedModel->coords[i][2]));

			objGeometry->color.push_back(
				DirectX::XMFLOAT4(color));
		}

		for (int i = 0; i < m_ParsedModel->facesInfos.size(); i++) {
			objGeometry->indices.push_back(m_ParsedModel->facesInfos[i][0]);
		}
	}



	objGeometry->m_flagEnabledBits = flagEnabledBits;

	CHECK_POINTERSNULL("Model geometry loaded successfully","Model Geometry is empty", objGeometry);

	return objGeometry;
}

