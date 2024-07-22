#pragma once
#include "Vector.h"
#include "Frame.h"

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


private:
	GCVector<GCFrame> m_frames;
	size_t m_currentFrameIndex;
	int m_currentFrameTime;
};