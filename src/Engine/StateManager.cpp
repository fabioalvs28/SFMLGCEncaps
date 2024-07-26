#include "pch.h"
#include "StateManager.h"
#include "GC.h"
#include "Timer.h"

void GCStateManager::Update()
{
	float deltaTime = GC::GetActiveTimer()->DeltaTime();
	if (m_currentState)
	{
		m_currentState->Update(deltaTime);
	}
	if (m_nextState)
	{
		ChangeState(m_nextState);
	}
}

void GCStateManager::ChangeState(GCState* newState)
{
	if (m_currentState)
	{
		m_currentState->Exit(newState);
	}
	if (newState)
	{
		newState->Enter(m_currentState);
	}
	m_previousState = m_currentState;
	m_currentState = newState;
	m_nextState = nullptr;
}
