#include "pch.h"
#include "GC.h"

#include <d3d11.h>

#include "GCSprite.h"

LEWindowGC::LEWindowGC()
{
    if (mpInstance) 
    {
		std::cerr << "GC Doesn't support multiple windows" << std::endl;
		exit(1);
    }

	mpInstance = this;
}

void LEWindowGC::Initialize(int width, int height, const char* title)
{
	GCGraphicsLogger::GetInstance().InitializeConsole();

	mpWindow = new Window(GetModuleHandle(nullptr));
	mpWindow->Initialize(L"hello world");

	mpGraphics = new GCGraphics();
    mpGraphics->Initialize(mpWindow, width, height);
    mpGraphics->GetRender()->Set2DMode();

    //Setting up camera
	int halfHeight = -1 * (height / 2);

    DirectX::XMFLOAT3 cameraPosition(width / 2, halfHeight, -10.0f);
    DirectX::XMFLOAT3 cameraTarget(width / 2, halfHeight, 0.0f);
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
        0.0f,                 //Aspect ratio not used for orthographic
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
    mX = x;
    mY = y;

    ComputeWorldMatrix();
}

void LEObjectGC::ComputeWorldMatrix()
{
	mWorldMatrix = DirectX::XMMatrixScaling(mWidth, mHeight, 1.0f) * DirectX::XMMatrixTranslation(mX, -mY, 0.0f);
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
}

void LESpriteGC::SetTexture(ITexture* pTexture)
{
    LETextureGC* pLETextureGC = (LETextureGC*)pTexture;
	mpMaterial = pLETextureGC->mpMaterial;

	mWidth = pLETextureGC->mWidth;
	mHeight = pLETextureGC->mHeight;
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

LEEntityGC::LEEntityGC()
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
	
}

void LEEntityGC::Initialize(const char* path)
{
	GCGraphics* pGraphics = LEWindowGC::Get()->GetGraphics();

	pGraphics->InitializeGraphicsResourcesStart();

	//Texture creation
	auto texture = pGraphics->CreateTexture(std::string(path) + ".dds");
	assert(texture.success);
    
	GCTexture* mpTexture = texture.resource;

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
	
	mTarget.isSet = false;
	mDirection = DirectX::XMFLOAT2(5.0f, 0.0f);
	mSpeed = 50.0f;
}

void LEEntityGC::Update()
{
	//
}

DirectX::XMFLOAT2& LEEntityGC::GetPosition(float ratioX, float ratioY) const
{
	DirectX::XMFLOAT2 position;
	position.x = mX;
	position.y = mY;
	return position;
}

void LEEntityGC::Move(DirectX::XMFLOAT2 offset)
{
	mX += offset.x;
	mY += offset.y;
}

void LEEntityGC::FixedUpdate(float dt)
{
	float x = GetPosition().x / 2;
	float y = GetPosition().y / 2;
	float distance = dt * mSpeed;
	DirectX::XMFLOAT2 translation = { mDirection.x * distance, mDirection.y * distance };
	Move(translation);
	
	std::cout << mX << " " << mY << "\n";
	if (mTarget.isSet)
	{
		mTarget.distance -= distance;

		if (mTarget.distance <= 0.f)
		{
			SetPosition(static_cast<float>(mTarget.position.x), static_cast<float>(mTarget.position.y));
			mDirection = DirectX::XMFLOAT2(0.f, 0.f);
			mTarget.isSet = false;
		}
	}

	ComputeWorldMatrix();
}

