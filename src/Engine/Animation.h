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

	void SetSpriteSheet(std::string fileName, GC_SPRITESHEET_INFO* spriteSheet );

private:
	GCGraphics* m_pGraphics;
	GC_SPRITESHEET_INFO m_spriteSheetInfos;
	GCGeometry* m_pGeometry;
	GCMaterial* m_pMaterial;

	std::vector<GCFrame*> m_pFrames;

};