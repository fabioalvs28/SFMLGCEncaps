#pragma once
#include "pch.h"

class GCTextManager
{
friend class GCRenderManager;
friend class GCGameManager;
friend class GCText;

private:
	GCTextManager(GCGraphics* pGraphics);
	~GCTextManager() {};

	void CreateText(GCText* pTextComponent);

private:
	GCGraphics* m_pGraphics;

	GCTexture* m_pTexture;
	GCMaterial* m_pMaterial;

	std::string m_texturePath;
};