#include "pch.h"
#include "RenderManager.h"
#include "GC.h"
#include "GameObject.h"
#include "../../src/render/framework.h"

GCRenderManager::GCRenderManager()
{

    m_pGraphics = new GCGraphics();

    /*m_pGraphics->Initialize(pWindow, pWindow->GetWidth(), pWindow->GetHeight());*/

    //euuh HASSOUL c'est pour la caméra.

    DirectX::XMVECTOR cameraPosition = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f);  // Position de la caméra
    DirectX::XMVECTOR targetPosition = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f); // Point visé par la caméra
    DirectX::XMVECTOR upVector = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);       // Vecteur "up" de la caméra

    float viewWidth = 10.0f;    // Largeur de la vue
    float viewHeight = 10.0f;   // Hauteur de la vue
    float nearZ = 0.1f;         // Plan proche
    float farZ = 100.0f;        // Plan éloigné

    DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ);
    DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(cameraPosition, targetPosition, upVector);

    DirectX::XMMATRIX transposedProjectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);
    DirectX::XMMATRIX transposedViewMatrix = DirectX::XMMatrixTranspose(viewMatrix);

    DirectX::XMStoreFloat4x4(&m_storedProjectionMatrix, transposedProjectionMatrix);
    DirectX::XMStoreFloat4x4(&m_storedViewMatrix, transposedViewMatrix);

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
    m_pPlane = m_pGraphics->CreateGeometryPrimitiveTexture("plane");
    m_pCircle = m_pGraphics->CreateGeometryPrimitiveTexture("circle");
}


void GCRenderManager::RegisterComponent(Component* component)
{

    /*for (GCListNode<Component*>* pComponent = m_pComponentList.GetFirstNode(); pComponent != nullptr; pComponent->GetNext())
    {
        if (pComponent->GetData()->m_pGameObject->GetLayer() <= component->m_pGameObject->GetLayer())
        {
            component->m_pRenderNode = pComponent->PushBefore(component);
            break;
        }
        else if ( pComponent == m_pComponentList.GetLastNode() )
        {
            m_pComponentList.PushBack(component);
            component->m_pRenderNode = m_pComponentList.GetLastNode();
        }
    }*/

    m_pComponentList.PushBack(component);
    component->m_pRenderNode = m_pComponentList.GetLastNode();
}

