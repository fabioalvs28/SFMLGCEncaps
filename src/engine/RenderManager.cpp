#include "pch.h"

#include "RenderManager.h"
#include "GC.h"
#include "GameObject.h"
#include "../render/pch.h"

using namespace DirectX;

GCRenderManager::GCRenderManager()
{

    m_pGraphics = new GCGraphics();

    m_cameraPosition = XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f);
    m_cameraTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
    m_cameraUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    //Set Camera
    float viewWidth = 20.0f;
    float viewHeight = 20.0f;
    XMMATRIX projectionMatrix = XMMatrixOrthographicLH(viewWidth, viewHeight, 1.0f, 1000.0f);
    XMMATRIX viewMatrix = XMMatrixLookAtLH(m_cameraPosition, m_cameraTarget, m_cameraUp);
    XMMATRIX transposedProjectionMatrix = XMMatrixTranspose(projectionMatrix);
    XMMATRIX transposedViewMatrix = XMMatrixTranspose(viewMatrix);

    XMStoreFloat4x4(&m_storedProjectionMatrix, transposedProjectionMatrix);
    XMStoreFloat4x4(&m_storedViewMatrix, transposedViewMatrix);
}

GCRenderManager::~GCRenderManager()
{
}

void GCRenderManager::Render() // Render : order by layer, and spriteRenderer before Collider.
{
    m_pGraphics->StartFrame();

    m_pGraphics->UpdateViewProjConstantBuffer(m_storedProjectionMatrix, m_storedViewMatrix);

    // Affichage : premier de la liste au prmeier plan.

    for (GCListNode<Component*>* componentNode = m_componentList.GetFirstNode(); componentNode != nullptr; componentNode = componentNode->GetNext())
    {
        componentNode->GetData()->Render();
    }

    m_pGraphics->EndFrame();
}

void GCRenderManager::CreateGeometry()
{
    m_pPlane = m_pGraphics->CreateGeometryPrimitive(Plane, XMFLOAT4(Colors::Blue)).resource;
    //m_pCircle = m_pGraphics->CreateGeometryPrimitive(Circle, XMFLOAT4(Colors::Blue)).resource;
}


void GCRenderManager::RegisterComponent(Component* component)
{

    //component->m_pRenderNode = m_componentList.PushBack(component);
    
    GCListNode<Component*>* pFirstNode = m_componentList.GetFirstNode() ;

    if ( pFirstNode == nullptr )
    {
        component->m_pRenderNode = m_componentList.PushBack(component);
        return; 
    }


    if ( pFirstNode == m_componentList.GetLastNode() )
    {
        if ( pFirstNode->GetData()->GetGameObject()->GetLayer() < component->GetGameObject()->GetLayer() )
        {
            m_componentList.PushBack( component );
            return;
        }
        else if ( pFirstNode->GetData()->GetID() >= component->GetID() )
        {
            m_componentList.PushFront( component );
            return;
        }
    }


    for ( GCListNode<Component*>* pComponent = m_componentList.GetLastNode(); pComponent != nullptr; pComponent = pComponent->GetPrevious() )
    {
        if ( pComponent->GetData()->GetGameObject()->GetLayer() < component->GetGameObject()->GetLayer() )
        {
            component->m_pRenderNode = pComponent->PushAfter( component );
            return;
        }
        if ( pComponent->GetData()->GetGameObject()->GetLayer() == component->GetGameObject()->GetLayer() && pComponent->GetData()->GetID() <= component->GetID() )
        {
            component->m_pRenderNode = pComponent->PushAfter( component );
            return;
        }
    }

    m_componentList.PushBack( component );

}


