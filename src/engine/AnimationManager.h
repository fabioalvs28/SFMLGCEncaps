#pragma once
#include "../core/framework.h"

class Animation; 

class GCAnimationManager
{
public:

	GCAnimationManager();
	~GCAnimationManager() = default;

	void AddAnimation(const Animation& animation, std::string animationName);

private:
	GCMap<std::string, Animation> m_animationList;

};