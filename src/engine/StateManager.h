#pragma once
#include "pch.h"

class GCStateManager
{
public:

	void Update();
	void ChangeState( GCState* pNewState );

private:
	GCState* m_pCurrentState = nullptr;
	GCState* m_pNextState = nullptr;
	GCState* m_pPreviousState = nullptr;
};