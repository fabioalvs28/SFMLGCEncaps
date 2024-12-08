#include "pch.h"
#include "GC.h"

#include <d3d11.h>

LEWindowGC::LEWindowGC()
{
	mpLastInstance = this;
}

void LEWindowGC::Initialize(HINSTANCE hInstance, unsigned int width, unsigned int height, const char* title)
{
	GCGraphicsLogger::GetInstance().InitializeConsole();

	mpWindow = new Window(hInstance);
	mpWindow->Initialize(L"hello world");

	mpGraphics = new GCGraphics();
	mpGraphics->Initialize(mpWindow, width, height);
	mpGraphics->GetRender()->Set2DMode();

    //Setting up camera
    DirectX::XMFLOAT3 cameraPosition(0.0f, 0.0f, -10.0f);
    DirectX::XMFLOAT3 cameraTarget(0.0f, 0.0f, 0.0f);
    DirectX::XMFLOAT3 cameraUp(0.0f, 1.0f, 0.0f);

    //Orthographic projection
    float viewWidth = width;
    float viewHeight = height;
    float nearZ = 1.0f;
    float farZ = 1000.0f;

    //Create view and projection matrixes
    mpGraphics->CreateViewProjConstantBuffer(
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
        mProjectionMatrix,
        mViewMatrix
    );
}

void LEWindowGC::Clear()
{
	//mpWindow->clear();
}

bool start = false;

void LEWindowGC::Draw(IDrawable* pDrawable)
{
    if (start == false) 
    {
        mpGraphics->StartFrame();
        mpGraphics->UpdateViewProjConstantBuffer(mProjectionMatrix, mViewMatrix);

		start = true;
    }

	pDrawable->Draw(this);
}

void LEWindowGC::Render()
{
    mpGraphics->EndFrame();
    mpWindow->Run(mpGraphics->GetRender());
    start = false;
}

void LETextureGC::Load(const char* path)
{
	GCGraphics* pGraphics = LEWindowGC::mpLastInstance->mpGraphics;

    pGraphics->InitializeGraphicsResourcesStart();

    //Texture creation
    auto texture = pGraphics->CreateTexture(path);
    assert(texture.success);
    
    mpTexture = texture.resource;

    pGraphics->InitializeGraphicsResourcesEnd();

    //Create shaders
    auto shaderTexture = pGraphics->CreateShaderTexture();

    //Create material
    auto material = pGraphics->CreateMaterial(shaderTexture.resource);
    material.resource->SetTexture(texture.resource);
    assert(material.success);

    mpMaterial = material.resource;

    mWidth = texture.resource->GetWidth();
    mHeight = texture.resource->GetHeight();
}


LESpriteGC::LESpriteGC()
{
    GCGraphics* pGraphics = LEWindowGC::mpLastInstance->mpGraphics;

    auto geoPlane = pGraphics->CreateGeometryPrimitive(Plane, DirectX::XMFLOAT4(DirectX::Colors::Beige));
    assert(geoPlane.success);

    mpGeometry = geoPlane.resource;

    pGraphics->InitializeGraphicsResourcesStart();

    //Mesh creation
    auto mesh = pGraphics->CreateMeshTexture(mpGeometry);
    assert(mesh.success);

    mpMesh = mesh.resource;

    pGraphics->InitializeGraphicsResourcesEnd();
}

void LESpriteGC::SetTexture(ITexture* pTexture)
{
	mpTexture = (LETextureGC*)pTexture;

	mWidth = mpTexture->mWidth;
	mHeight = mpTexture->mHeight;
}

void LESpriteGC::SetPosition(float x, float y)
{
    mWorldMatrix = DirectX::XMMatrixScaling(mWidth, mHeight, 1.0f) * DirectX::XMMatrixTranslation(x, y, 0.0f);
}

void LESpriteGC::Draw(IWindow* pWindow)
{
    LEWindowGC* pLEWindowGC = (LEWindowGC*) pWindow;
	GCGraphics* pGraphics = pLEWindowGC->mpGraphics;

	GCMaterial* mpMaterial = mpTexture->mpMaterial;

    pGraphics->UpdateWorldConstantBuffer(mpMaterial, mWorldMatrix);
    pGraphics->GetRender()->DrawObject(mpMesh, mpMaterial, true);
}