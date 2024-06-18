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

void GCMaterial::CheckForRemoveNonUsedCB()
{
	//// Utiliser remove_if et erase pour supprimer les �l�ments non utilis�s
	//auto it = std::remove_if(m_vpObjectCB.begin(), m_vpObjectCB.end(), [](GCShaderUploadBufferBase* pObjectCB) {
	//	if (!pObjectCB->IsUsed()) {
	//		delete pObjectCB; // Lib�rer la m�moire
	//		return true;
	//	}
	//	return false;
	//	});

	//// Erase les �l�ments marqu�s pour suppression
	//m_vpObjectCB.erase(it, m_vpObjectCB.end());
}