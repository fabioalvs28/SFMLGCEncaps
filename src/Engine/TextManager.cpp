#include "pch.h"

GCTextManager::GCTextManager(GCGraphics* pGraphics)
{
	pGraphics->m_pFontGeometryLoader->Initialize("../../../src/Render/Fonts/LetterUV.txt");
	m_pGraphics = pGraphics;
	m_texturePath = "../../../src/Render/Textures/TimesFont.dds";

	m_pGraphics->InitializeGraphicsResourcesStart();
	m_pTexture = m_pGraphics->CreateTexture(m_texturePath).resource;
	m_pGraphics->InitializeGraphicsResourcesEnd();

	GCShader* pShader = m_pGraphics->CreateShaderTexture().resource;
	m_pMaterial = m_pGraphics->CreateMaterial(pShader).resource;
	m_pMaterial->SetTexture(m_pTexture);
}


void GCTextManager::CreateText(GCText* pTextComponent)
{
	m_pGraphics->InitializeGraphicsResourcesStart();
	pTextComponent->m_pGeometry = m_pGraphics->m_pFontGeometryLoader->CreateText(pTextComponent->m_text);
	pTextComponent->m_pMesh = m_pGraphics->CreateMeshTexture(pTextComponent->m_pGeometry).resource;
	m_pGraphics->InitializeGraphicsResourcesEnd();
}
