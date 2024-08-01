#pragma once
#include "../core/framework.h"

struct GCGeometry;

class Animation
{
public:
	Animation();
	~Animation() = default;

	void AddFrame( int frameID, float displayTime = 2.0f, bool isFlipingX = false, bool isFlipingY = false);
	void StartAnimation();
	bool Update();
	void IncrementFrame();
	void Reset();
	const GCFrame* GetCurrentFrame() const;
	int GetNumberOfFrames() const { return static_cast<int>(m_pFrames.size()); }
	int GetCurrentFrameIndex() const { return m_currentFrameIndex; }
	GCGeometry* GetGeometry() { return m_pGeometry; }
	GCTexture* GetTexture() { return m_pTexture; }

	void SetSpriteSheet(std::string fileName, GC_SPRITESHEET_INFO* spriteSheet );

private:

	GC_SPRITESHEET_INFO m_spriteSheetInfos;
	GCGeometry* m_pGeometry; 
	GCTexture* m_pTexture;

	std::vector<GCFrame*> m_pFrames;
	int m_currentFrameIndex;
	float m_currentFrameTime;
};