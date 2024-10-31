#include "pch.h"



GCSprite::GCSprite( std::string filename )
{
	GCGraphics* pGraphics = GC::GetActiveRenderManager()->m_pGraphics;
	m_pGeometry = pGraphics->CreateGeometryPrimitive( Plane, XMFLOAT4( Colors::Blue ) ).resource;
	
    pGraphics->InitializeGraphicsResourcesStart();
	
    m_pMesh = pGraphics->CreateMeshTexture( m_pGeometry ).resource;
	GCTexture* pTexture = pGraphics->CreateTexture( std::string( "../../../res/" ) + filename ).resource;
	
    pGraphics->InitializeGraphicsResourcesEnd();
	
    m_pMaterial = pGraphics->CreateMaterial( pGraphics->CreateShaderTexture().resource ).resource;
	m_pMaterial->SetTexture( pTexture );
}