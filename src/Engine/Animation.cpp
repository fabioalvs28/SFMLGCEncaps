#include "pch.h"
#include "../Render/pch.h"

Animation::Animation()
{
	m_pFrames = std::vector<GCFrame*>(0);
	m_pTexture = nullptr;
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
	GCRenderManager* pRender = GC::GetActiveRenderManager();
	pRender->m_pGraphics->m_pSpriteSheetGeometryLoader->SetSpriteUVs( m_pGeometry , m_spriteSheetID, m_pFrames[ 0 ]->GetFrameID(), *pRender->GetSpriteSheetData() );
}

bool Animation::Update( int* currentFrameIndex, float* currentFrameTime )
{
	GCRenderManager* pRender = GC::GetActiveRenderManager();
	if (m_pFrames.size() > 0)
	{
		*currentFrameTime += GC::GetActiveTimer()->DeltaTime();

		if ( *currentFrameTime >= m_pFrames[ *currentFrameIndex ]->GetDisplayTime())
		{
			*currentFrameTime -= m_pFrames[*currentFrameIndex]->GetDisplayTime();
			IncrementFrame( currentFrameIndex );
			pRender->m_pGraphics->m_pSpriteSheetGeometryLoader->SetSpriteUVs(m_pGeometry, m_spriteSheetID, m_pFrames[*currentFrameIndex]->GetFrameID(), *pRender->GetSpriteSheetData());
			return true;
		}
	}
	return false;
}

void Animation::IncrementFrame( int* currentFrameIndex )
{
	*currentFrameIndex += 1;
	if (*currentFrameIndex == m_pFrames.size())
		*currentFrameIndex = 0;
}


const GCFrame* Animation::GetCurrentFrame( int currentFrameIndex ) const
{
	if (m_pFrames.size() > 0)
	{
		return m_pFrames[ currentFrameIndex ];
	}
	return nullptr;
}


void Animation::SetSpriteSheet( std::string fileName , int spriteSheetID )
{
	GCGraphics* pGraphics = GC::GetActiveRenderManager()->m_pGraphics;
	m_spriteSheetID = spriteSheetID;

	pGraphics->InitializeGraphicsResourcesStart();
	m_pTexture = pGraphics->CreateTexture("../../../src/Textures/" + fileName).resource;
	pGraphics->InitializeGraphicsResourcesEnd();
}