#pragma once
#include "pch.h"
#include "../core/pch.h"

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
	int GetNumberOfFrames() const { return m_frames.GetSize(); }
	int GetCurrentFrameIndex() const { return m_currentFrameIndex; }

private:
	GCVector<GCFrame> m_frames;
	size_t m_currentFrameIndex;
	int m_currentFrameTime;
};