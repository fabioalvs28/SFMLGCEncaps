#pragma once

class GCStateManager;


class GCAnimationPlayer
{
public:

	void UpdateState();
	void PlayAnimation();
	void StopAnimation();

private:
	GCStateManager* m_globalState;
};