#pragma once
#include "pch.h"

GCAnimationManager::GCAnimationManager()
{}


void GCAnimationManager::AddAnimation(const Animation& animation, std::string animationName)
{
	m_animationList.Insert(animationName, animation);
}