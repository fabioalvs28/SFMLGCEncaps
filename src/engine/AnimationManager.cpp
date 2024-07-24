#pragma once
#include "pch.h"
#include "AnimationManager.h"
#include "Animation.h"

GCAnimationManager::GCAnimationManager()
{
	Init();
}

void GCAnimationManager::Init()
{
	m_currentFrameIndex = 0;
	m_numberOfFrames = 0;
	m_timeToDisplay = 0;
}

void GCAnimationManager::AddAnimation(const Animation& animation, GCString animationName)
{
	m_animationList.PushBack(animation);
	m_numberOfFrames = animation.GetNumberOfFrames();
	m_currentFrameIndex = animation.GetCurrentFrameIndex();
	m_timeToDisplay = animation.GetCurrentFrame()->GetDisplayTime();
	m_animationName = animationName;
}



int GCAnimationManager::GetNumberOfFrames() const
{
	return m_numberOfFrames;
}

int GCAnimationManager::GetCurrentFrameIndex() const
{
	return m_currentFrameIndex;
}

float GCAnimationManager::GetTimeToDisplay() const
{
	return m_timeToDisplay;
}
