#pragma once
#include "pch.h"

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
	m_animationList.Insert(animationName, animation);
	m_numberOfFrames = animation.GetNumberOfFrames();
	m_currentFrameIndex = animation.GetCurrentFrameIndex();
	m_timeToDisplay = animation.GetCurrentFrame()->GetDisplayTime();
}

void GCAnimationManager::SetActiveAnimation(GCString animationName)
{
	m_activeAnimation = animationName; 
}



int GCAnimationManager::GetNumberOfFrames() const
{
	return m_animationList.GetValue(m_activeAnimation).GetNumberOfFrames();
}

int GCAnimationManager::GetCurrentFrameIndex() const
{
	return m_animationList.GetValue(m_activeAnimation).GetCurrentFrameIndex();
}

float GCAnimationManager::GetTimeToDisplay() const
{
	return m_timeToDisplay;
}
