#pragma once
#include "../core/framework.h"

struct GCGeometry;

class Animation
{
public:
	Animation();
	~Animation() = default;

	void AddFrame( int frameID, float displayTime = 2.0f, bool isFlipingX = false, bool isFlipingY = false );
	void StartAnimation();
	bool Update( int* currentFrameIndex, float* currentFrameTime  );
	void IncrementFrame( int* currentFrameIndex );
	const GCFrame* GetCurrentFrame( int currentFrameIndex ) const;
	int GetNumberOfFrames() const { return static_cast<int>( m_pFrames.size() ); }
	GCGeometry* GetGeometry() { return m_pGeometry; }
	GCTexture* GetTexture() { return m_pTexture; }

	void SetSpriteSheet(std::string fileName, GC_SPRITESHEET_INFO* spriteSheet );

private:

	GC_SPRITESHEET_INFO m_spriteSheetInfos;
	GCGeometry* m_pGeometry; 
	GCTexture* m_pTexture;

	std::vector<GCFrame*> m_pFrames;

};