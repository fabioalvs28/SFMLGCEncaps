#pragma region 2DDEMO

//2D DEMO
#include "pch.h"

#include "LE.h"

#include <assert.h>

#define WIDTH 1920
#define HEIGHT 1080

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) 
{
	LEWindow* pWindow = new LEWindow();
	pWindow->Initialize(hInstance, WIDTH, HEIGHT, "GC works!");

	LETexture* pTexture = new LETexture();

#ifdef GC
	pTexture->Load("../../../res/Testing/Textures/happyImage.dds");
#else
    pTexture->Load("../../../res/Testing/image.png");
#endif

	LESprite* pSprite = new LESprite();
	pSprite->SetTexture(pTexture);
	pSprite->SetPosition(0, 0);

    while (true)
    {
        pWindow->Clear();
        pWindow->Draw(pSprite);
        pWindow->Render();
    }

    return 0;
}

/*
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    //Initialize graphics resource
    GCGraphicsLogger& profiler = GCGraphicsLogger::GetInstance();
    profiler.InitializeConsole();

    Window* pWindow = new Window(hInstance);
    pWindow->Initialize(L"hello world");

    GCGraphics* pGraphics = new GCGraphics();

    pGraphics->Initialize(pWindow, WIDTH, HEIGHT);
    pGraphics->GetRender()->Set2DMode();

    int flagsColorTexture = 0;

    GC_SET_FLAG(flagsColorTexture, GC_VERTEX_POSITION);
    GC_SET_FLAG(flagsColorTexture, GC_VERTEX_COLOR);
    GC_SET_FLAG(flagsColorTexture, GC_VERTEX_UV);

    //Create geometries
    auto geoPlane = pGraphics->CreateGeometryPrimitive(Plane, DirectX::XMFLOAT4(DirectX::Colors::Beige));

    pGraphics->InitializeGraphicsResourcesStart();

    //Mesh creation
    auto mesh = pGraphics->CreateMeshTexture(geoPlane.resource);
    auto meshColor = pGraphics->CreateMeshColor(geoPlane.resource);

    //Texture creation
    std::string texturePath = "../../../res/Testing/Textures/happyImage.dds";
    auto texture = pGraphics->CreateTexture(texturePath);

    assert(texture.success);

    int textWidth = texture.resource->GetWidth();
    int textHeight = texture.resource->GetHeight();

    pGraphics->InitializeGraphicsResourcesEnd();

    //Create shaders
    auto shaderTexture = pGraphics->CreateShaderTexture();
    auto shaderColor = pGraphics->CreateShaderColor();

    //Create materials
    auto material = pGraphics->CreateMaterial(shaderTexture.resource);
    material.resource->SetTexture(texture.resource);

    //Setting up camera
    DirectX::XMFLOAT3 cameraPosition(0.0f, 0.0f, -10.0f);
    DirectX::XMFLOAT3 cameraTarget(0.0f, 0.0f, 0.0f);
    DirectX::XMFLOAT3 cameraUp(0.0f, 1.0f, 0.0f);

    //Orthographic projection
    float viewWidth = WIDTH;
    float viewHeight = HEIGHT;
    float nearZ = 1.0f;
    float farZ = 1000.0f;

    DirectX::XMMATRIX storedProjectionMatrix, storedViewMatrix;

    //Create view and projection matrixes
    pGraphics->CreateViewProjConstantBuffer(
        cameraPosition,
        cameraTarget,
        cameraUp,
        0.0f,                 //FOV not used for orthographic
        0.0f,                 //Aspect ration not used for orthographic
        nearZ,
        farZ,
        viewWidth,
        viewHeight,
        ORTHOGRAPHIC,         //Projection type
        storedProjectionMatrix,
        storedViewMatrix
    );

    //Setting up matrixes
    DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixScaling(textWidth, textHeight, 1.0f) * DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);

    while (pWindow->IsClosed() == false)
    {
        pGraphics->StartFrame();

        pGraphics->UpdateViewProjConstantBuffer(storedProjectionMatrix, storedViewMatrix);
        pGraphics->UpdateWorldConstantBuffer(material.resource, worldMatrix);
        pGraphics->GetRender()->DrawObject(mesh.resource, material.resource, true);

        pGraphics->EndFrame();

        pWindow->Run(pGraphics->GetRender());
    }

    return 0;
}
*/

#pragma endregion