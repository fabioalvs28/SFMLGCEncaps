#pragma once
#include "Components.h"

class GCGraphics;
class GCGeometry;

class GCRenderManager
{
    friend class GCGameObject;

    struct GCTest : GCSHADERCB {
        DirectX::XMFLOAT4X4 world;
        DirectX::XMFLOAT4 color;
    };

private:
    GCList<SpriteRenderer*> m_pSpriteRendererList;
public:
    GCRenderManager();
    ~GCRenderManager();

    void RegisterSpriteRenderer(SpriteRenderer* spriteRenderer);

    void SetShaderTexture(SpriteRenderer* spriteRenderer, std::string texturePath);
    void SetShaderColor(SpriteRenderer* spriteRenderer);

    void CreateGeometry();

    void Update();
    void Render();

    GCGraphics* m_pGraphics; 
    GCGeometry* m_pPlane; 

    DirectX::XMFLOAT4X4 m_storedProjectionMatrix;
    DirectX::XMFLOAT4X4 m_storedViewMatrix;

};