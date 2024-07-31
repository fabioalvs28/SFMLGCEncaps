#pragma once
#include "../core/framework.h"

struct GCGeometry;

class Animation
{
public:
	Animation();
	~Animation() = default;

	void AddFrame( int frameID, float displayTime = 3.2f, bool isFlipingX = false, bool isFlipingY = false);
	void StartAnimation();
	bool Update();
	void IncrementFrame();
	void Reset();
	const GCFrame* GetCurrentFrame() const;
	int GetNumberOfFrames() const { return static_cast<int>(m_frames.GetSize()); }
	int GetCurrentFrameIndex() const { return m_currentFrameIndex; }
	GCGeometry* GetGeometry() { return m_pGeometry; }
	GCTexture* GetTexture() { return m_pTexture; }

	void SetSpriteSheet(std::string fileName, GC_SPRITESHEET_INFO* spriteSheet );

private:

	GC_SPRITESHEET_INFO m_spriteSheetInfos;
	GCGeometry* m_pGeometry; 
	GCTexture* m_pTexture;

	GCVector<GCFrame> m_frames;
	int m_currentFrameIndex;
	float m_currentFrameTime;
};