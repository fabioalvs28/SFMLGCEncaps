#include "pch.h"

void GCStateManager::Update()
{
	float deltaTime = GC::GetActiveTimer()->DeltaTime();
	if ( m_pCurrentState )
		m_pCurrentState->Update( deltaTime );
	if ( m_pNextState )
		ChangeState( m_pNextState );
}

void GCStateManager::ChangeState( GCState* pNewState)
{
	if ( m_pCurrentState )
		m_pCurrentState->Exit( pNewState );
	if ( pNewState )
		pNewState->Enter( m_pCurrentState );
	m_pPreviousState = m_pCurrentState;
	m_pCurrentState = pNewState;
	m_pNextState = nullptr;
}
