#include "pch.h"
#include "UpdateManager.h"



void GCUpdateManager::Update()
{
    for ( GCListNode<Component*>* pComponentNode = m_registeredComponents.GetFirstNode(); pComponentNode != nullptr; pComponentNode = pComponentNode->GetNext() )
        pComponentNode->GetData()->Update();
}



void GCUpdateManager::RegisterComponent( Component* pComponent )
{
    ASSERT( pComponent != nullptr, LOG_FATAL, "Trying to register a nullptr pComponent to the UpdateManager" );
    pComponent->m_pUpdateNode = m_registeredComponents.PushBack( pComponent );
}