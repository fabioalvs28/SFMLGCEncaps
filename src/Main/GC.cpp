#include "pch.h"
#include "GC.h"

#include <d3d11.h>

LEWindowGC::LEWindowGC()
{
    if (mpInstance) 
    {
		std::cerr << "GC Doesn't support multiple windows" << std::endl;
		exit(1);
    }

	mpInstance = this;
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

    mpGraphics->UpdateViewProjConstantBuffer(mProjectionMatrix, mViewMatrix);

	mWidth = width;
	mHeight = height;
}

void LEWindowGC::Draw(IObject* pDrawable)
{
    if (mStartFrame == false)
    {
        mpGraphics->StartFrame();

        mStartFrame = true;
    }

    LEObjectGC* pLEDrawableGC = (LEObjectGC*)pDrawable;

    GCMaterial* mpMaterial = pLEDrawableGC->mpMaterial;
	GCMesh* pMesh = pLEDrawableGC->mpMesh;
	DirectX::XMMATRIX& mWorldMatrix = pLEDrawableGC->mWorldMatrix;

    mpGraphics->UpdateWorldConstantBuffer(mpMaterial, mWorldMatrix);

    mpGraphics->GetRender()->DrawObject(pMesh, mpMaterial, true);
}

void LEWindowGC::Render()
{
    mpGraphics->EndFrame();
    mpWindow->Run(mpGraphics->GetRender());
    mStartFrame = false;
}

LEObjectGC::LEObjectGC()
{
	mWorldMatrix = DirectX::XMMatrixIdentity();
}

void LEObjectGC::SetPosition(float x, float y)
{
    int offsetX = -LEWindowGC::Get()->GetWidth() / 2;
    int offsetY = (LEWindowGC::Get()->GetHeight() / 2) - mHeight;

    mX = x + offsetX;
    mY = y + offsetY;

    ComputeWorldMatrix();
}

void LEObjectGC::ComputeWorldMatrix()
{
	mWorldMatrix = DirectX::XMMatrixScaling(mWidth, mHeight, 1.0f) * DirectX::XMMatrixTranslation(mX, mY, 0.0f);
}

void LETextureGC::Load(const char* path)
{
    GCGraphics* pGraphics = LEWindowGC::Get()->GetGraphics();

    pGraphics->InitializeGraphicsResourcesStart();

    //Texture creation
    auto texture = pGraphics->CreateTexture(std::string(path) + ".dds");
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
    GCGraphics* pGraphics = LEWindowGC::Get()->GetGraphics();

    auto geoPlane = pGraphics->CreateGeometryPrimitive(Plane, DirectX::XMFLOAT4(DirectX::Colors::Black));
    assert(geoPlane.success);
    mpGeometry = geoPlane.resource;

    pGraphics->InitializeGraphicsResourcesStart();

    //Mesh creation
    auto mesh = pGraphics->CreateMeshTexture(mpGeometry);
    assert(mesh.success);

    mpMesh = mesh.resource;

    pGraphics->InitializeGraphicsResourcesEnd();

	mpTexture = nullptr;
}

void LESpriteGC::SetTexture(ITexture* pTexture)
{
	mpTexture = (LETextureGC*)pTexture;
	mpMaterial = mpTexture->mpMaterial;

	mWidth = mpTexture->mWidth;
	mHeight = mpTexture->mHeight;
}

LECircleGC::LECircleGC()
{
    GCGraphics* pGraphics = LEWindowGC::Get()->GetGraphics();

	auto geoCircle = pGraphics->CreateGeometryPrimitive(Circle, DirectX::XMFLOAT4(DirectX::Colors::Beige));
	assert(geoCircle.success);
	mpGeometry = geoCircle.resource;

	pGraphics->InitializeGraphicsResourcesStart();

    //Mesh creation
	auto mesh = pGraphics->CreateMeshColor(mpGeometry);
	assert(mesh.success);
	mpMesh = mesh.resource;

    pGraphics->InitializeGraphicsResourcesEnd();

    //Create shaders
    auto shaderColor = pGraphics->CreateShaderColor();

    //Create material
    auto material = pGraphics->CreateMaterial(shaderColor.resource);
    assert(material.success);

    mpMaterial = material.resource;

	SetRadius(1.0f);
}

void LECircleGC::SetRadius(float radius)
{
	mRadius = radius;

    ComputeWorldMatrix();
}