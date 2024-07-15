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

    m_componentList = std::vector<Component*>(0);

}

GCRenderManager::~GCRenderManager()
{
}

void GCRenderManager::Render() // Render : order by layer, and spriteRenderer before Collider.
{
    m_pGraphics->StartFrame();

    m_pGraphics->UpdateViewProjConstantBuffer(m_storedProjectionMatrix, m_storedViewMatrix);

    // Affichage : premier de la liste au prmeier plan.

    for ( int i = 0 ; i < m_componentList.size() ; i ++ )
    {
        m_componentList[i]->Render();
    }

    m_pGraphics->EndFrame();
}

void GCRenderManager::CreateGeometry()
{
    m_pPlane = m_pGraphics->CreateGeometryPrimitive(Plane, XMFLOAT4(Colors::Red)).resource;
    //m_pCircle = m_pGraphics->CreateGeometryPrimitive(Circle, XMFLOAT4(Colors::Blue)).resource;
}


void GCRenderManager::RegisterComponent(Component* component)
{
    if (m_componentList.size() == 0)
    {
        m_componentList.push_back(component); 
        return;
    }

    m_componentList.push_back(component);
    
    ////if ( m_componentList.size() == 2 )
    ////{
    //    if ( m_componentList[0]->GetGameObject()->GetID() == component->GetGameObject()->GetID() )
    //    {
    //        if (m_componentList[0].)
    //    }
    ////    else if ( m_componentList[0]->GetGameObject()->GetLayer() >= m_componentList[1]->GetGameObject()->GetLayer() )
    ////        m_componentList.push_back(component);
    ////    else
    ////        m_componentList.insert(0, component);
    ////    return;
    ////}
    //
    //while ( true )
    //{
    //
    //}
}

