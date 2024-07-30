#include "pch.h"

Animation::Animation() : m_frames(0), m_currentFrameIndex(0), m_currentFrameTime(0) {}

void Animation::AddFrame(int x, int y, int width, int height, float displayTime, bool isFlipingX, bool isFlipingY)
{
	GCFrame frame(x, y, width, height, displayTime, isFlipingX, isFlipingY);
	m_frames.PushBack(frame);
}

bool Animation::Update(float deltaTime)
{
	if (m_frames.GetSize() > 0)
	{
		m_currentFrameTime += deltaTime;

		if (m_currentFrameTime >= m_frames[m_currentFrameIndex].GetDisplayTime())
		{
			m_currentFrameTime = 0;
			IncrementFrame();
			return true;
		}
	}
	return false;
}

void Animation::IncrementFrame()
{
	m_currentFrameIndex = (m_currentFrameIndex + 1) % m_frames.GetSize();
}

void Animation::Reset()
{
	m_currentFrameIndex = 0;
	m_currentFrameTime = 0;
}

const GCFrame* Animation::GetCurrentFrame() const
{
	if (m_frames.GetSize() > 0)
	{
		return &m_frames[m_currentFrameIndex];
	}
	return nullptr;
}
