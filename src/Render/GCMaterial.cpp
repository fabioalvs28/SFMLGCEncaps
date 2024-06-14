#include "framework.h"

GCMaterial::GCMaterial(){}

GCMaterial::~GCMaterial(){}


void GCMaterial::Initialize(GCShader* pShader, GCTexture* pTexture) {
	m_pShader = pShader;
	m_pTexture = pTexture;
}

void GCMaterial::addObjectCB(GCShaderUploadBufferBase* pObjectCB) {
	m_vpObjectCB.push_back(pObjectCB);
}

void GCMaterial::addCameraCB(GCShaderUploadBufferBase* pCameraCB) {
	m_vpCameraCB.push_back(pCameraCB);
}

void GCMaterial::UpdateConstantBufferData(const GCSHADERCB& objectData, GCShaderUploadBufferBase* uploadBufferInstance)
{
	uploadBufferInstance->CopyData(0, objectData);
}