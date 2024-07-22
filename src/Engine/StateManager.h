#pragma once
#include "State.h"

class GCStateManager
{
public:

	void Update();
	void ChangeState(GCState* newState);

private:
	GCState* m_currentState = nullptr;
	GCState* m_nextState = nullptr;
	GCState* m_previousState = nullptr;
};