#pragma once 
#include "State.h"

class GCStateMachine
{

public:
	GCStateMachine();
	~GCStateMachine() = default;

	void ChangeState(GCState* pNewState, void* args = nullptr);
	void Update(float deltaTime);

	GCState* GetCurrentState() { return m_pCurrentState; }
	GCState* GetPreviousState() { return m_pPreviousState; }


private:
	GCState* m_pCurrentState;
	GCState* m_pPreviousState;
};