#pragma once
#include "pch.h"

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