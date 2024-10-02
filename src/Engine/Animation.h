#pragma once
#include "pch.h"

class GCAnimation
{
friend class GCAnimator;

public:
	GCAnimation();
	~GCAnimation() = default;

	void AddFrame( int frameID, float displayTime = 2.0f, bool isFlipingX = false, bool isFlipingY = false );
	void StartAnimation();
	bool Update( int* currentFrameIndex, float* currentFrameTime  );
	void IncrementFrame( int* currentFrameIndex );
	const GCFrame* GetCurrentFrame( int currentFrameIndex ) const;
	int GetNumberOfFrames() const { return static_cast<int>( m_pFrames.size() ); }
	GCGeometry* GetGeometry() { return m_pGeometry; }

	int GetLastFrameIndex() { return m_pFrames.size() - 1; }

	void SetSpriteSheet(std::string filename, int spriteSheetID );

private:
	GCSprite* m_pSprite;
	GCGraphics* m_pGraphics;
	int m_spriteSheetID;
	GCMesh* m_pMesh;
	GCGeometry* m_pGeometry; 
	GCMaterial* m_pMaterial;
	std::vector<GCFrame*> m_pFrames;

};