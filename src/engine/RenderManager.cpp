#include "pch.h"

using namespace DirectX;

GCRenderManager::GCRenderManager( Window* pWindow )
{
    m_pGraphics = new GCGraphics();
    m_pGraphics->Initialize( pWindow, 1920, 1080 );
}

GCRenderManager::~GCRenderManager() {}

//////////////////////////////////////////////////////////
/// @brief Calls the Render function of the components.
//////////////////////////////////////////////////////////
void GCRenderManager::Render()
{
    m_pGraphics->StartFrame();
    
    GC::GetActiveScene()->GetMainCamera()->Update();

    for ( GCListNode<GCComponent*>* pComponentNode = m_componentsList.GetFirstNode(); pComponentNode != nullptr; pComponentNode = pComponentNode->GetNext() )
        pComponentNode->GetData()->Render();

    m_pGraphics->EndFrame();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Stores components with RENDER flags, and order them by gameObject Layer and component Layer.
///
/// @param pComponent a pointer to the stored component. 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void GCRenderManager::RegisterComponent( GCComponent* pComponent )
{   
    GCListNode<GCComponent*>* pFirstNode = m_componentsList.GetFirstNode();

    if ( pFirstNode == nullptr )
    {
        pComponent->m_pRenderNode = m_componentsList.PushBack( pComponent );
        return; 
    }


    if ( pFirstNode == m_componentsList.GetLastNode() )
    {
        if ( pFirstNode->GetData()->m_pGameObject->GetLayer() < pComponent->m_pGameObject->GetLayer() )
        {
            m_componentsList.PushBack(pComponent);
            return;
        }

        if ( pFirstNode->GetData()->m_pGameObject->GetLayer() == pComponent->m_pGameObject->GetLayer() )
        {
            if ( pFirstNode->GetData()->GetComponentLayer() < pComponent->GetComponentLayer() )
            {
                m_componentsList.PushBack( pComponent );
                return;
            }
        }

        m_componentsList.PushFront( pComponent );
    }


    for ( GCListNode<GCComponent*>* pComponentNode = m_componentsList.GetLastNode(); pComponentNode != nullptr; pComponentNode = pComponentNode->GetPrevious() )
    {
        GCComponent* pComponentInList = pComponentNode->GetData();

        if (pComponentInList->m_pGameObject->GetLayer() < pComponent->m_pGameObject->GetLayer() )
        {
            pComponent->m_pRenderNode = pComponentNode->PushAfter(pComponent);
            return;
        }

        if ( pComponentInList->m_pGameObject->GetLayer() == pComponent->m_pGameObject->GetLayer() )
        {
            if ( pComponentInList->GetComponentLayer() <= pComponent->GetComponentLayer() )
            {
                pComponent->m_pRenderNode = pComponentNode->PushAfter( pComponent );
                return;
            }

        }
    }

    m_componentsList.PushFront( pComponent );
}


void GCRenderManager::AddAnimation( GCAnimation* pAnimation , std::string animationName )
{ m_animationsList.Insert( animationName , pAnimation ); }

GCAnimation* GCRenderManager::GetAnimation( std::string animationName )
{
    GCAnimation* pAnimation;
    if ( m_animationsList.Find( animationName , pAnimation ) )
        return pAnimation;
    return nullptr;
}