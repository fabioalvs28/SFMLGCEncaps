#include "pch.h"
#include "StateMachine.h"
#include "State.h"

GCStateMachine::GCStateMachine() : m_pCurrentState(nullptr) {}

void GCStateMachine::ChangeState(GCState* newState, void* args)
{
	if (m_pCurrentState)
		m_pCurrentState->Exit();

	m_pPreviousState = m_pCurrentState;
	m_pCurrentState = newState;
	m_pCurrentState->Enter(this, args);
}

void GCStateMachine::Update(float deltaTime)
{
	if (m_pCurrentState)
		m_pCurrentState->Update(deltaTime);
}