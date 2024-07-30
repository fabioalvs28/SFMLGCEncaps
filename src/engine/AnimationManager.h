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
	void SetActiveAnimation(GCString animationName);

	int GetNumberOfFrames() const;
	int GetCurrentFrameIndex() const;
	float GetTimeToDisplay() const;

private:
	GCMap<GCString, Animation> m_animationList;
	float m_timeToDisplay;
	int m_numberOfFrames;
	int m_currentFrameIndex;
	GCString m_activeAnimation;
};