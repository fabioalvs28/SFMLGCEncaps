#include "RenderManager.h"
#include "GC.h"
#include "GameObject.h"
#include "../../src/Render/pch.h"

using namespace DirectX;

GCRenderManager::GCRenderManager()
{

    m_pGraphics = new GCGraphics();
}

GCRenderManager::~GCRenderManager()
{
}

void GCRenderManager::Render()
{
    m_pGraphics->StartFrame();

    m_pGraphics->UpdateViewProjConstantBuffer(m_storedProjectionMatrix, m_storedViewMatrix);

    for ( GCListNode<Component*>* componentNode = m_pComponentList.GetFirstNode(); componentNode != nullptr ; componentNode = componentNode->GetNext() )
    {
        Component* pComponent = componentNode->GetData();

        GCTest test = m_pGraphics->ToPixel<GCTest>(pComponent->m_pGameObject->m_transform.m_position.x  , pComponent->m_pGameObject->m_transform.m_position.y , m_storedProjectionMatrix, m_storedViewMatrix);

        m_pGraphics->UpdateCustomCBObject<GCTest>(pComponent->m_pMaterial, test);
        m_pGraphics->GetRender()->DrawObject(pComponent->m_pMesh, pComponent->m_pMaterial);
    }

    m_pGraphics->EndFrame();
}

void GCRenderManager::CreateGeometry()
{
    m_pPlane = m_pGraphics->CreateGeometryPrimitive(Plane, XMFLOAT4(Colors::Red)).resource;
    m_pCircle = m_pGraphics->CreateGeometryPrimitive(Circle, XMFLOAT4(Colors::Blue)).resource;
}


void GCRenderManager::RegisterComponent(Component* component)
{
    if (m_pComponentList.GetFirstNode() == nullptr)
    {
        component->m_pRenderNode = m_pComponentList.PushBack(component);
        return;
    }

    m_pComponentList.PushBack(component);
    
    //if ( m_pComponentList.GetFirstNode() == m_pComponentList.GetLastNode())
    //{
    //    if (m_pComponentList.GetFirstNode()->GetData()->GetGameObject()->GetLayer() <= m_pComponentList.GetLastNode()->GetData()->GetGameObject()->GetLayer())
    //        m_pComponentList.PushBack(component);
    //    else
    //        m_pComponentList.PushFront(component);
    //    return;
    //}

    //for (GCListNode<Component*>* pComponent = m_pComponentList.GetFirstNode(); pComponent != nullptr; pComponent->GetNext())
    //{
    //    if (pComponent->GetData()->m_pGameObject->GetLayer() >= component->m_pGameObject->GetLayer())
    //    {
    //        component->m_pRenderNode = pComponent->PushBefore(component);
    //        break;
    //    }
    //    else if ( pComponent == m_pComponentList.GetLastNode() )
    //    {
    //        component->m_pRenderNode = m_pComponentList.PushBack(component);
    //    }
    //}

}

