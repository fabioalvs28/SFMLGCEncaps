#include "pch.h"
#include "RenderManager.h"

#include "GameObject.h"
#include "../../src/render/framework.h"

GCRenderManager::GCRenderManager()
{

    m_pGraphics = new GCGraphics();

    //euuh HASSOUL

    DirectX::XMVECTOR cameraPosition = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f);  // Position de la cam�ra
    DirectX::XMVECTOR targetPosition = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f); // Point vis� par la cam�ra
    DirectX::XMVECTOR upVector = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);       // Vecteur "up" de la cam�ra

    float viewWidth = 10.0f;    // Largeur de la vue
    float viewHeight = 10.0f;   // Hauteur de la vue
    float nearZ = 0.1f;         // Plan proche
    float farZ = 100.0f;        // Plan �loign�

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

void GCRenderManager::Update()
{
    m_pGraphics->StartFrame();

    m_pGraphics->UpdateViewProjConstantBuffer(m_storedProjectionMatrix, m_storedViewMatrix);

    for ( GCListNode<SpriteRenderer*>* spriteNode = m_pSpriteRendererList.GetFirstNode(); spriteNode != nullptr ; spriteNode  = spriteNode->GetNext() )
    {
        SpriteRenderer* pSprite = spriteNode->GetData(); 
        GCTest test = m_pGraphics->ToPixel<GCTest>(pSprite->pos.x , pSprite->pos.y , m_storedProjectionMatrix, m_storedViewMatrix);
        m_pGraphics->UpdateCustomCBObject<GCTest>(pSprite->m_pMaterial, test);
        m_pGraphics->GetRender()->DrawObject(pSprite->m_pMesh, pSprite->m_pMaterial);
    }

    m_pGraphics->EndFrame();
}

void GCRenderManager::Render()
{

};

void GCRenderManager::CreateGeometry()
{
    m_pPlane = m_pGraphics->CreateGeometryPrimitiveTexture("plane");
}


void GCRenderManager::RegisterSpriteRenderer(SpriteRenderer* spriteRenderer)
{
    m_pSpriteRendererList.PushBack(spriteRenderer);
    spriteRenderer->m_pRenderNode = m_pSpriteRendererList.GetLastNode();
    m_pGraphics->InitializeGraphicsResourcesStart();
    spriteRenderer->m_pMesh = m_pGraphics->CreateMesh(m_pPlane);
    m_pGraphics->InitializeGraphicsResourcesEnd();

}


void GCRenderManager::SetShaderTexture(SpriteRenderer* spriteRenderer, std::string texturePath)
{
    m_pGraphics->InitializeGraphicsResourcesStart();
    GCTexture* texture = m_pGraphics->CreateTexture(texturePath);
    m_pGraphics->InitializeGraphicsResourcesEnd();

    GCShader* shaderTexture = m_pGraphics->CreateShaderTexture();
    spriteRenderer->m_pMaterial = m_pGraphics->CreateMaterial(shaderTexture);
    spriteRenderer->m_pMaterial->SetTexture(texture);
}

void GCRenderManager::SetShaderColor(SpriteRenderer* spriteRenderer)
{
    GCShader* shaderColor = m_pGraphics->CreateShaderColor();
    spriteRenderer->m_pMaterial = m_pGraphics->CreateMaterial(shaderColor);

}