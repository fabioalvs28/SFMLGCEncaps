#include "pch.h"
#include "GCSprite.h"

void GCSprite::LoadTexture(const char* filePath, GCGraphics* pGraphics)
{
    auto geoPlane = pGraphics->CreateGeometryPrimitive(Plane, DirectX::XMFLOAT4(DirectX::Colors::Beige));
    assert(geoPlane.success);

	mpGeometry = geoPlane.resource;

    pGraphics->InitializeGraphicsResourcesStart();

    //Mesh creation
    auto mesh = pGraphics->CreateMeshTexture(mpGeometry);
    assert(mesh.success);

	mpMesh = mesh.resource;

    //Texture creation
    auto texture = pGraphics->CreateTexture(filePath);
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

	mX = 0;
	mY = 0;
    mWidth = texture.resource->GetWidth();
    mHeight = texture.resource->GetHeight();
}

void GCSprite::SetPosition(float x, float y)
{
	mX = x;
	mY = y;

    mWorldMatrix = DirectX::XMMatrixScaling(mWidth, mHeight, 1.0f) * DirectX::XMMatrixTranslation(x, y, 0.0f);
}

void GCSprite::Draw(GCGraphics* pGraphics)
{
    pGraphics->UpdateWorldConstantBuffer(mpMaterial, mWorldMatrix);
    pGraphics->GetRender()->DrawObject(mpMesh, mpMaterial, true);
}