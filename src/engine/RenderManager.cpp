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

    //HASSOUL
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

void GCRenderManager::Render()
{
    m_pGraphics->StartFrame();

    m_pGraphics->UpdateViewProjConstantBuffer(m_storedProjectionMatrix, m_storedViewMatrix);

    for ( GCListNode<Component*>* componentNode = m_pComponentList.GetFirstNode(); componentNode != nullptr ; componentNode = componentNode->GetNext() )
    {
        Component* pComponent = componentNode->GetData();
        
        XMMATRIX worldMatrix = XMMatrixScaling(pComponent->m_pGameObject->m_transform.m_scale.x, pComponent->m_pGameObject->m_transform.m_scale.y, pComponent->m_pGameObject->m_transform.m_scale.z) * XMMatrixTranslation(pComponent->m_pGameObject->m_transform.m_position.x, pComponent->m_pGameObject->m_transform.m_position.y, pComponent->m_pGameObject->m_transform.m_position.z); // Cube externe (skybox)

        XMFLOAT4X4 worldMatrice; 
        XMStoreFloat4x4(&worldMatrice, XMMatrixTranspose(worldMatrix));

        m_pGraphics->UpdateWorldConstantBuffer(pComponent->m_pMaterial, worldMatrice);
        m_pGraphics->GetRender()->DrawObject(pComponent->m_pMesh, pComponent->m_pMaterial);
        
    }

    m_pGraphics->EndFrame();
}

void GCRenderManager::CreateGeometry()
{
    m_pPlane = m_pGraphics->CreateGeometryPrimitive(Plane, XMFLOAT4(Colors::Orange)).resource;
    //m_pCircle = m_pGraphics->CreateGeometryPrimitive(Circle, XMFLOAT4(Colors::Blue)).resource;
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

