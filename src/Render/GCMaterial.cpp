#include "framework.h"

GCMaterial::GCMaterial()
{
    m_pRender = nullptr;
    m_pShader = nullptr;
    m_pTexture = nullptr;
}

GCMaterial::~GCMaterial()
{
    delete(m_pRender);
    delete(m_pShader);
    delete(m_pTexture);
}


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

bool GCMaterial::UpdateTexture()
{
    if (m_pShader->GetType() == texture)
    {
        if (m_pTexture)
        {
            auto commandList = m_pRender->GetCommandList();

            //// Transition to copy destination state
            //CD3DX12_RESOURCE_BARRIER transitionBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
            //    m_pTexture->GetTextureBuffer(),  // Ressource concernée
            //    D3D12_RESOURCE_STATE_COMMON,     // État avant la transition
            //    D3D12_RESOURCE_STATE_COPY_DEST   // État après la transition
            //);

            //// Application de la barrière à la liste de commandes
            //commandList->ResourceBarrier(1, &transitionBarrier);

            //// Assuming that CopyTextureRegion is set up properly
            //// Copy the texture to the mipmap
            //for (UINT mipLevel = 1; mipLevel < m_pTexture->GetMipLevels(); ++mipLevel)
            //{
            //    D3D12_TEXTURE_COPY_LOCATION destLocation(m_pTexture->GetTextureBuffer());
            //    D3D12_TEXTURE_COPY_LOCATION srcLocation(m_pTexture->GetTextureBuffer());
            //    commandList->CopyTextureRegion(&destLocation, 0, 0, 0, &srcLocation, nullptr);
            //}

            //// Transition to read state
            //CD3DX12_RESOURCE_BARRIER transition2Barrier = CD3DX12_RESOURCE_BARRIER::Transition(
            //    m_pTexture->GetTextureBuffer(),  // Ressource concernée
            //    D3D12_RESOURCE_STATE_COPY_DEST,
            //    D3D12_RESOURCE_STATE_GENERIC_READ
            //);

            //// Application de la barrière à la liste de commandes
            //commandList->ResourceBarrier(1, &transition2Barrier);

            m_pRender->GetCommandList()->SetGraphicsRootDescriptorTable(2, m_pTexture->GetTextureAddress());

            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}