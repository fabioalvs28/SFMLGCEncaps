#pragma once


class GCAnimationManager
{
public:

	void UpdateState();
	void PlayAnimation();
	void StopAnimation();

private:
	GCStateManager* m_globalState;
};