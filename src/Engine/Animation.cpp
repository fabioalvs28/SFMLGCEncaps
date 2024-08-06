#include "pch.h"
#include "../Render/pch.h"

Animation::Animation() : m_pFrames( 0 )
{
	GCGraphics* pGraphics = GC::GetActiveRenderManager()->m_pGraphics;
	m_pGeometry = pGraphics->CreateGeometryPrimitive( Plane, XMFLOAT4( Colors::Green ) ).resource;
}

void Animation::AddFrame( int frameID, float displayTime, bool isFlipingX, bool isFlipingY )
{
	GCFrame* pFrame= new GCFrame( frameID, displayTime, isFlipingX, isFlipingY );
	m_pFrames.push_back( pFrame );
}

void Animation::StartAnimation()
{
	GC::GetActiveRenderManager()->m_pGraphics->m_pSpriteSheetGeometryLoader->SetSpriteUVs( m_pGeometry , m_pFrames[ 0 ]->GetFrameID(), m_spriteSheetInfos );
}

bool Animation::Update( int* currentFrameIndex, float* currentFrameTime )
{
	if ( m_pFrames.size() > 0 )
	{
		*currentFrameTime += GC::GetActiveTimer()->DeltaTime();

		if ( *currentFrameTime >= m_pFrames[ *currentFrameIndex ]->GetDisplayTime() )
		{
			*currentFrameTime = 0;
			IncrementFrame( currentFrameIndex );
			GC::GetActiveRenderManager()->m_pGraphics->m_pSpriteSheetGeometryLoader->SetSpriteUVs( m_pGeometry, m_pFrames[ *currentFrameIndex ]->GetFrameID(), m_spriteSheetInfos );
			return true;
		}
	}
	return false;
}

void Animation::IncrementFrame( int* currentFrameIndex )
{
	*currentFrameIndex = ( *currentFrameIndex + 1 ) % m_pFrames.size();
}


const GCFrame* Animation::GetCurrentFrame( int currentFrameIndex ) const
{
	if ( m_pFrames.size() > 0 )
		return m_pFrames[ currentFrameIndex ];
	return nullptr;
}


void Animation::SetSpriteSheet( std::string fileName , GC_SPRITESHEET_INFO* spriteSheet )
{
	GCGraphics* pGraphics = GC::GetActiveRenderManager()->m_pGraphics;
	m_spriteSheetInfos = *spriteSheet;

	pGraphics->InitializeGraphicsResourcesStart();
	m_pTexture = pGraphics->CreateTexture( "../../../src/Textures/" + fileName ).resource;
	pGraphics->InitializeGraphicsResourcesEnd();
}