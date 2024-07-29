#pragma once
#include "../core/framework.h"

class Animation; 

class GCAnimationManager
{
public:

	GCAnimationManager();
	~GCAnimationManager() = default;

	void Init();
	void AddAnimation(const Animation& animation, GCString animationName);

	int GetNumberOfFrames() const;
	int GetCurrentFrameIndex() const;
	float GetTimeToDisplay() const;

private:
	GCVector<Animation> m_animationList;
	GCString m_animationName;
	float m_timeToDisplay;
	int m_numberOfFrames;
	int m_currentFrameIndex;
};