#include "framework.h"

GCMaterial::GCMaterial(){}

GCMaterial::~GCMaterial(){}


void GCMaterial::Initialize(GCShader* pShader, GCTexture* pTexture, GCRender* pRender) 
{
	m_pRender = pRender;
	m_pShader = pShader;
	m_pTexture = pTexture;
}

void GCMaterial::UpdateConstantBuffer(const GCSHADERCB& objectData, GCShaderUploadBufferBase* uploadBufferInstance)
{
	uploadBufferInstance->CopyData(0, objectData);
}