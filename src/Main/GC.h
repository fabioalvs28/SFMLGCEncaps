#pragma once

#include "Generic.h"

class Window;
class GCTexture;
class GCMaterial;

class LEWindowGC : public LEWindow
{
	Window* mpWindow; 
	GCGraphics* mpGraphics;
	inline static LEWindowGC* mpLastInstance = nullptr; //#UGLY

	DirectX::XMMATRIX mProjectionMatrix, mViewMatrix;


public:
	LEWindowGC();

	void Initialize(HINSTANCE hInstance, unsigned int width, unsigned int height, const char* title) override;
	void Clear() override;
	void Draw(LEDrawable* pDrawable) override;
	void Render() override;

	friend class LETextureGC;
	friend class LESpriteGC;
};

class LETextureGC : public LETexture
{
	GCTexture* mpTexture;
	GCMaterial* mpMaterial;

	int mWidth;
	int mHeight;

public:
	void Load(const char* path) override;
	void GetWidth() override {};
	void GetHeight() override {};

	friend class LESpriteGC;
};


class LESpriteGC : public LESprite
{
	GCGeometry* mpGeometry;
	GCMesh* mpMesh;

	DirectX::XMMATRIX mWorldMatrix;

	LETextureGC* mpTexture;
	
	int mWidth;
	int mHeight;

public:
	LESpriteGC();


public:
	void SetTexture(LETexture* pTexture) override;
	void SetPosition(float x, float y) override;
	void Draw(LEWindow* pWindow) override;
};

