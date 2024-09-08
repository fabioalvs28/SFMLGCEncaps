#include "pch.h"


GCTextManager::GCTextManager(GCGraphics* pGraphics)
{
	m_flagsColorTexture = 0;
	GC_SET_FLAG(m_flagsColorTexture, GC_VERTEX_POSITION);
	GC_SET_FLAG(m_flagsColorTexture, GC_VERTEX_COLOR);
	GC_SET_FLAG(m_flagsColorTexture, GC_VERTEX_UV);

	pGraphics->m_pFontGeometryLoader->Initialize("../../../src/Render/Fonts/LetterUV.txt");
	m_pGraphics = pGraphics;
	m_texturePath = "../../../src/Render/Textures/TimesFont.dds";

	m_pGraphics->InitializeGraphicsResourcesStart();
	m_pTexture = m_pGraphics->CreateTexture(m_texturePath).resource;
	m_pGraphics->InitializeGraphicsResourcesEnd();

	std::string shaderTextFilePath = "../../../src/Render/Shaders/textTexture.hlsl";
	std::string csoDestinationTextPath = "../../../src/Render/CsoCompiled/textTexture";
	GCShader* pShader = m_pGraphics->CreateShaderCustom(shaderTextFilePath, csoDestinationTextPath, m_flagsColorTexture).resource;

	m_pMaterial = m_pGraphics->CreateMaterial(pShader).resource;
	m_pMaterial->SetTexture(m_pTexture);

}


void GCTextManager::CreateText(GCText* pTextComponent)
{
	m_pGraphics->InitializeGraphicsResourcesStart();
	pTextComponent->m_pGeometry = m_pGraphics->m_pFontGeometryLoader->CreateText(pTextComponent->m_text, pTextComponent->m_color);
	pTextComponent->m_pMesh = m_pGraphics->CreateMeshCustom(pTextComponent->m_pGeometry, m_flagsColorTexture).resource;
	m_pGraphics->InitializeGraphicsResourcesEnd();
}
