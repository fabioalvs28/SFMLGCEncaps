#include "pch.h"
#include "../Render/pch.h"

Animation::Animation() : m_pFrames(0), m_currentFrameIndex(0), m_currentFrameTime(0) 
{
	GCGraphics* pGraphics = GC::GetActiveRenderManager()->m_pGraphics;
	m_pGeometry = pGraphics->CreateGeometryPrimitive(Plane, XMFLOAT4(Colors::Green)).resource;
}

void Animation::AddFrame(int frameID, float displayTime, bool isFlipingX, bool isFlipingY)
{
	GCFrame* pFrame= new GCFrame( frameID, displayTime, isFlipingX, isFlipingY);
	m_pFrames.push_back(pFrame);
}

void Animation::StartAnimation()
{
	m_currentFrameIndex = 0;
	GC::GetActiveRenderManager()->m_pGraphics->m_pSpriteSheetGeometryLoader->SetSpriteUVs( m_pGeometry , m_pFrames[ m_currentFrameIndex ]->GetFrameID(), m_spriteSheetInfos );
}

bool Animation::Update()
{
	if (m_pFrames.size() > 0)
	{
		m_currentFrameTime += GC::GetActiveTimer()->DeltaTime();

		if (m_currentFrameTime >= m_pFrames[m_currentFrameIndex]->GetDisplayTime())
		{
			m_currentFrameTime = 0;
			IncrementFrame();
			GC::GetActiveRenderManager()->m_pGraphics->m_pSpriteSheetGeometryLoader->SetSpriteUVs(m_pGeometry, m_pFrames[m_currentFrameIndex]->GetFrameID(), m_spriteSheetInfos);
			return true;
		}
	}
	return false;
}

void Animation::IncrementFrame()
{
	m_currentFrameIndex = (m_currentFrameIndex + 1) % m_pFrames.size();
}

void Animation::Reset()
{
	m_currentFrameIndex = 0;
	m_currentFrameTime = 0;
}

const GCFrame* Animation::GetCurrentFrame() const
{
	if (m_pFrames.size() > 0)
	{
		return m_pFrames[m_currentFrameIndex];
	}
	return nullptr;
}


void Animation::SetSpriteSheet( std::string fileName , GC_SPRITESHEET_INFO* spriteSheet )
{
	GCGraphics* pGraphics = GC::GetActiveRenderManager()->m_pGraphics;
	m_spriteSheetInfos = *spriteSheet;

	pGraphics->InitializeGraphicsResourcesStart();
	m_pTexture = pGraphics->CreateTexture("../../../src/Textures/" + fileName).resource;
	pGraphics->InitializeGraphicsResourcesEnd();
}