#include "pch.h"

GCAnimation::GCAnimation()
{
	m_pFrames = std::vector<GCFrame*>(0);
	m_pGraphics = GC::GetActiveRenderManager()->m_pGraphics;
	m_pGeometry = m_pGraphics->CreateGeometryPrimitive( Plane, XMFLOAT4( Colors::Green ) ).resource;
	m_pGraphics->InitializeGraphicsResourcesStart();
	m_pMesh = m_pGraphics->CreateMeshTexture(m_pGeometry).resource;
	m_pGraphics->InitializeGraphicsResourcesEnd();
}

void GCAnimation::AddFrame( int frameID, float displayTime, bool isFlipingX, bool isFlipingY )
{
	GCFrame* pFrame= new GCFrame( frameID, displayTime, isFlipingX, isFlipingY );
	m_pFrames.push_back( pFrame );
}

void GCAnimation::StartAnimation()
{
	m_pGraphics->m_pSpriteSheetGeometryLoader->SetSpriteUVs( m_pGeometry , m_spriteSheetID, m_pFrames[ 0 ]->GetFrameID(), *GC::GetActiveRenderManager()->GetSpriteSheetData() );
	m_pMesh->UpdateGeometryData();
}

bool GCAnimation::Update( int* currentFrameIndex, float* currentFrameTime )
{

	if ( m_pFrames.size() > 0 )
	{
		*currentFrameTime += GC::GetActiveTimer()->DeltaTime();

		if ( *currentFrameTime >= m_pFrames[ *currentFrameIndex ]->GetDisplayTime() )
		{
			*currentFrameTime -= m_pFrames[*currentFrameIndex]->GetDisplayTime();
			IncrementFrame( currentFrameIndex );
			m_pGraphics->m_pSpriteSheetGeometryLoader->SetSpriteUVs(m_pGeometry, m_spriteSheetID, m_pFrames[*currentFrameIndex]->GetFrameID(), *GC::GetActiveRenderManager()->GetSpriteSheetData() );
			m_pMesh->UpdateGeometryData();
			return true;
		}
	}
	return false;
}

void GCAnimation::IncrementFrame( int* currentFrameIndex )
{
	*currentFrameIndex += 1;
	if ( *currentFrameIndex == m_pFrames.size() )
		*currentFrameIndex = 0;
}


const GCFrame* GCAnimation::GetCurrentFrame( int currentFrameIndex ) const
{
	if ( m_pFrames.size() > 0 )
		return m_pFrames[ currentFrameIndex ];
	return nullptr;
}


void GCAnimation::SetSpriteSheet(std::string filename, int spriteSheetID)
{
	m_spriteSheetID = spriteSheetID;

	m_pGraphics->InitializeGraphicsResourcesStart();
	GCTexture* pTexture = m_pGraphics->CreateTexture(std::string("../../../res/spritesheet/") + filename).resource;
	m_pGraphics->InitializeGraphicsResourcesEnd();

	m_pMaterial = m_pGraphics->CreateMaterial(m_pGraphics->CreateShaderTexture().resource).resource;
	m_pMaterial->SetTexture(pTexture);
}
