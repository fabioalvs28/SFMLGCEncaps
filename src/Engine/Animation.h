#pragma once
#include "../core/framework.h"

class Animation
{
public:
	Animation();
	~Animation() = default;

	void AddFrame(int x, int y, int width, int height, float displayTime, bool isFlipingX, bool isFlipingY);
	bool Update(float deltaTime);
	void IncrementFrame();
	void Reset();
	const GCFrame* GetCurrentFrame() const;
	int GetNumberOfFrames() const { return static_cast<int>(m_frames.GetSize()); }
	int GetCurrentFrameIndex() const { return m_currentFrameIndex; }

private:
	GCVector<GCFrame> m_frames;
	int m_currentFrameIndex;
	float m_currentFrameTime;
};