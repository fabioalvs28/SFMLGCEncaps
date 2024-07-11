#include "pch.h"
#include "UpdateManager.h"

#include "Components.h"



void GCUpdateManager::Update()
{
    for ( GCListNode<Component*>* pComponentNode = m_registeredComponents.GetFirstNode(); pComponentNode != nullptr; pComponentNode = pComponentNode->GetNext() )
        pComponentNode->GetData()->Update();
}



void GCUpdateManager::RegisterComponent( Component* pComponent ) { pComponent->m_pUpdateNode = m_registeredComponents.PushBack( pComponent ); }