#include "pch.h"



/////////////////////////////////////////////////////////////////////////
/// @brief Updates all registered Components that wants to be updated.
/////////////////////////////////////////////////////////////////////////
void GCUpdateManager::Update()
{
    for ( GCListNode<GCComponent*>* pComponentNode = m_registeredComponents.GetFirstNode(); pComponentNode != nullptr; pComponentNode = pComponentNode->GetNext() )
        pComponentNode->GetData()->Update();
}



////////////////////////////////////////////////////////////////
/// @brief Registers a Component with the UpdateManager.
/// 
/// @param pComponent A pointer to the Component to register.
////////////////////////////////////////////////////////////////
void GCUpdateManager::RegisterComponent( GCComponent* pComponent )
{
    ASSERT( pComponent != nullptr, LOG_FATAL, "Trying to register a nullptr pComponent to the UpdateManager" );
    pComponent->m_pUpdateNode = m_registeredComponents.PushBack( pComponent );
}