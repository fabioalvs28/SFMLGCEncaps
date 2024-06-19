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
}

bool GCMaterial::UpdateTexture()
{
    if (m_pShader->GetType() == 1) // Texture?
    {
        if (m_pTexture)
        {
            m_pRender->GetCommandList()->SetGraphicsRootDescriptorTable(2, m_pTexture->GetTextureAddress());
        }
        else
        {
            return false;
        }
    }
}