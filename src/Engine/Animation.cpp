#include "pch.h"
#include "../Render/pch.h"

Animation::Animation() : m_frames(0), m_currentFrameIndex(0), m_currentFrameTime(0) 
{
	GCGraphics* pGraphics = GC::GetActiveRenderManager()->m_pGraphics;
	m_pGeometry = pGraphics->CreateGeometryPrimitive(Plane, XMFLOAT4(Colors::Green)).resource;
}

void Animation::AddFrame(int frameID, float displayTime, bool isFlipingX, bool isFlipingY)
{
	GCFrame frame( frameID, displayTime, isFlipingX, isFlipingY);
	m_frames.PushBack(frame);
}

bool Animation::Update()
{
	if (m_frames.GetSize() > 0)
	{
		m_currentFrameTime += GC::GetActiveTimer()->DeltaTime();

		if (m_currentFrameTime >= m_frames[m_currentFrameIndex].GetDisplayTime())
		{
			m_currentFrameTime = 0;
			IncrementFrame();
			GC::GetActiveRenderManager()->m_pGraphics->m_pSpriteSheetGeometryLoader->SetSpriteUVs(m_pGeometry, m_frames[m_currentFrameIndex].GetID(), m_spriteSheetInfos);
			return true;
		}
	}
	return false;
}

void Animation::IncrementFrame()
{
	m_currentFrameIndex = (m_currentFrameIndex + 1) % m_frames.GetSize();
}

void Animation::Reset()
{
	m_currentFrameIndex = 0;
	m_currentFrameTime = 0;
}

const GCFrame* Animation::GetCurrentFrame() const
{
	if (m_frames.GetSize() > 0)
	{
		return &m_frames[m_currentFrameIndex];
	}
	return nullptr;
}


void Animation::SetSpriteSheet(std::string fileName,int rowNumber, int colNumber, int sheetWidth, int sheetHeight)
{
	GCGraphics* pGraphics = GC::GetActiveRenderManager()->m_pGraphics;
	m_spriteSheetInfos = pGraphics->m_pSpriteSheetGeometryLoader->LoadSpriteSheet(rowNumber, colNumber, sheetWidth, sheetHeight);

	pGraphics->InitializeGraphicsResourcesStart();
	m_pTexture = pGraphics->CreateTexture("../../../src/Textures/" + fileName).resource;
	pGraphics->InitializeGraphicsResourcesEnd();
}