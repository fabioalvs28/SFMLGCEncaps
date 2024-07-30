#include "pch.h"
#include "../core/pch.h"

using namespace DirectX;

GCRenderManager::GCRenderManager( Window* pWindow )
{
    m_pGraphics = new GCGraphics();
    m_pGraphics->Initialize(pWindow,1920,1080);
    CreateGeometry();
}

GCRenderManager::~GCRenderManager()
{
}

//////////////////////////////////////////////////////////
/// @brief Calls the Render function of the components.
//////////////////////////////////////////////////////////
void GCRenderManager::Render()
{
    m_pGraphics->StartFrame();
    
    GC::GetActiveScene()->GetMainCamera()->Update();

    for (GCListNode<Component*>* pComponentNode = m_componentList.GetFirstNode(); pComponentNode != nullptr; pComponentNode = pComponentNode->GetNext())
    {
        pComponentNode->GetData()->Render();
    }

    m_pGraphics->EndFrame();
}

/////////////////////////////////////////////
/// @brief Create geometry, (only square).
/////////////////////////////////////////////
void GCRenderManager::CreateGeometry()
{
    m_pPlane = m_pGraphics->CreateGeometryPrimitive(Plane, XMFLOAT4(Colors::Blue)).resource;
    //m_pCircle = m_pGraphics->CreateGeometryPrimitive(Circle, XMFLOAT4(Colors::Blue)).resource;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Stores components with RENDER flags, and order them by gameObject Layer and component Layer.
///
/// @param pComponent a pointer to the stored component. 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void GCRenderManager::RegisterComponent( Component* pComponent )
{   
    GCListNode<Component*>* pFirstNode = m_componentList.GetFirstNode() ;

    if ( pFirstNode == nullptr )
    {
        pComponent->m_pRenderNode = m_componentList.PushBack(pComponent);
        return; 
    }


    if ( pFirstNode == m_componentList.GetLastNode() )
    {
        if ( pFirstNode->GetData()->m_pGameObject->GetLayer() < pComponent->m_pGameObject->GetLayer() )
        {
            m_componentList.PushBack(pComponent);
            return;
        }

        if ( pFirstNode->GetData()->m_pGameObject->GetLayer() == pComponent->m_pGameObject->GetLayer() )
        {
            if ( pFirstNode->GetData()->GetComponentLayer() < pComponent->GetComponentLayer() )
            {
                m_componentList.PushBack( pComponent );
                return;
            }
        }

        m_componentList.PushFront( pComponent );
    }


    for ( GCListNode<Component*>* pComponentNode = m_componentList.GetLastNode(); pComponentNode != nullptr; pComponentNode = pComponentNode->GetPrevious() )
    {
        Component* pComponentInList = pComponentNode->GetData();

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

    m_componentList.PushFront(pComponent);
}


