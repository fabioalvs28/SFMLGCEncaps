#include "pch.h"

GCMaterial::GCMaterial()
    : m_pRender(nullptr),
    m_pShader(nullptr),
    m_pTexture(nullptr),
    m_pCbMaterialPropertiesInstance(nullptr)
{
}

GCMaterial::~GCMaterial()
{
    DELETE(m_pCbMaterialPropertiesInstance);

    for (auto* cb : m_pCbObjectInstances)
    {
        DELETE(cb);
    }
    m_pCbObjectInstances.clear();
}


GC_GRAPHICS_ERROR GCMaterial::Initialize(GCShader* pShader)
{
	m_pShader = pShader;
    m_pRender = m_pShader->m_pRender;

    m_pCbMaterialPropertiesInstance = new GCShaderUploadBuffer<GCMATERIALPROPERTIES>(m_pRender->GetRenderResources()->Getmd3dDevice(), 1, true);

    GCMATERIALPROPERTIES materialProperties;
    materialProperties.ambientLightColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    materialProperties.ambient = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
    materialProperties.diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    materialProperties.specular = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
    materialProperties.shininess = 5.0f;                                    

    UpdateConstantBuffer(materialProperties, m_pCbMaterialPropertiesInstance);

    return GCRENDER_SUCCESS_OK;
}

bool GCMaterial::SetTexture(GCTexture* pTexture) {
    m_pTexture = pTexture;
    CHECK_POINTERSNULL("Texture loaded successfully for material", "The material doesn't contain texture", pTexture);

    return true;
}

void GCMaterial::UpdateConstantBuffer(const GCSHADERCB& objectData, GCShaderUploadBufferBase* uploadBufferInstance)
{
	uploadBufferInstance->CopyData(0, objectData);
}

bool GCMaterial::UpdateTexture()
{
    if (HAS_FLAG(m_pShader->GetFlagEnabledBits(), VERTEX_UV))
    {
        if (m_pTexture)
        {
            m_pRender->GetRenderResources()->GetCommandList()->SetGraphicsRootDescriptorTable(m_pShader->m_rootParameter_DescriptorTable_1, m_pTexture->GetTextureAddress());
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}