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

    for (GCListNode<GCGameObject*>* pGameObjectNode = m_gameObjectList.GetFirstNode(); pGameObjectNode != nullptr; pGameObjectNode = pGameObjectNode->GetNext())
    {
        pGameObjectNode->GetData()->Render();
    }

    m_pGraphics->EndFrame();
}

void GCRenderManager::CreateGeometry()
{
    m_pPlane = m_pGraphics->CreateGeometryPrimitive(Plane, XMFLOAT4(Colors::Blue)).resource;
    //m_pCircle = m_pGraphics->CreateGeometryPrimitive(Circle, XMFLOAT4(Colors::Blue)).resource;
}


void GCRenderManager::RegisterGameObject( GCGameObject* pGameObject )
{   
    GCListNode<GCGameObject*>* pFirstNode = m_gameObjectList.GetFirstNode() ;

    if ( pFirstNode == nullptr )
    {
        pGameObject->m_pRenderNode = m_gameObjectList.PushBack(pGameObject);
        return; 
    }


    if ( pFirstNode == m_gameObjectList.GetLastNode() )
    {
        if ( pFirstNode->GetData()->GetLayer() < pGameObject->GetLayer() )
        {
            m_gameObjectList.PushBack(pGameObject);
            return;
        }
    }


    for ( GCListNode<GCGameObject*>* pGameObjectNode = m_gameObjectList.GetLastNode(); pGameObjectNode != nullptr; pGameObjectNode = pGameObjectNode->GetPrevious() )
    {
        if (pGameObjectNode->GetData()->GetLayer() <= pGameObject->GetLayer() )
        {
            pGameObject->m_pRenderNode = pGameObjectNode->PushAfter(pGameObject);
            return;
        }       
    }

    m_gameObjectList.PushFront(pGameObject);
}


