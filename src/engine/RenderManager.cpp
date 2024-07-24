#include "pch.h"
#include "../core/framework.h"
#include "RenderManager.h"
#include "GC.h"
#include "GameObject.h"
#include "../render/pch.h"

using namespace DirectX;

GCRenderManager::GCRenderManager()
{

    m_pGraphics = new GCGraphics();
    m_pGraphics->Initialize(GC::GetWindow(),1920,1080);
    CreateGeometry();

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
    
    m_storedProjectionMatrix = GCUtils::XMMATRIXToGCMATRIX(transposedProjectionMatrix);
    m_storedViewMatrix = GCUtils::XMMATRIXToGCMATRIX(transposedViewMatrix);
}

GCRenderManager::~GCRenderManager()
{
}

void GCRenderManager::Render() // Render : order by layer, and spriteRenderer before Collider.
{
    m_pGraphics->StartFrame();

    m_pGraphics->UpdateViewProjConstantBuffer(m_storedProjectionMatrix, m_storedViewMatrix);

    // Affichage : premier de la liste au prmeier plan.

    for (GCListNode<Component*>* pComponentNode = m_componentList.GetFirstNode(); pComponentNode != nullptr; pComponentNode = pComponentNode->GetNext())
    {
        pComponentNode->GetData()->Render();
    }

    m_pGraphics->EndFrame();
}

void GCRenderManager::CreateGeometry()
{
    m_pPlane = m_pGraphics->CreateGeometryPrimitive(Plane, XMFLOAT4(Colors::Blue)).resource;
    //m_pCircle = m_pGraphics->CreateGeometryPrimitive(Circle, XMFLOAT4(Colors::Blue)).resource;
}


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


