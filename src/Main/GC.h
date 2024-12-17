#pragma once

#include "Generic.h"

class Window;
class GCGraphics;

class GCTexture;
class GCMaterial;

struct GCGeometry;
class GCMesh;

namespace DirectX 
{
	struct XMMATRIX;
}

class LEWindowGC : public IWindow
{
	Window* mpWindow;
	GCGraphics* mpGraphics;

	DirectX::XMMATRIX mProjectionMatrix, mViewMatrix;

	int mWidth, mHeight;

	bool mStartFrame = false;

	inline static LEWindowGC* mpInstance = nullptr;

public:
	LEWindowGC();

	static LEWindowGC* Get() { return mpInstance; }
	GCGraphics* GetGraphics() { return mpGraphics; }

	int GetWidth() { return mWidth; }
	int GetHeight() { return mHeight; }

	void Initialize(int width, int height, const char* title) override;
	void Clear() override {};
	void Draw(IObject* pDrawable) override;
	void Render() override;
};

class LEObjectGC : public IObject
{
protected:
	GCGeometry* mpGeometry;
	GCMesh* mpMesh;
	GCMaterial* mpMaterial;

	DirectX::XMMATRIX mWorldMatrix;
	int mX = 0, mY = 0;
	int mWidth = 0, mHeight = 0;

public:
	LEObjectGC();

	void SetPosition(float x, float y) override;
	void ComputeWorldMatrix();

	friend LEWindowGC;
};

class LETextureGC : public ITexture
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

class LESpriteGC : public ISprite, public LEObjectGC
{
public:
	LESpriteGC();

public:
	void SetTexture(ITexture* pTexture) override;
};

class LECircleGC : public ICircle, public LEObjectGC
{
	float mRadius;

public:
	LECircleGC();

	void SetRadius(float radius) override;
	void SetColor(unsigned char r, unsigned char g, unsigned char b) override {};
};

class LEEntityGC : public IEntity, public LEObjectGC
{
protected:
	struct Target 
	{
		DirectX::XMINT2 position;
		float distance;
		bool isSet;
	};
	
	DirectX::XMFLOAT2 mDirection;
	Target mTarget;
	float mSpeed;
	
public:
	LEEntityGC();

	void Update() override;
	void FixedUpdate(float dt) override;
	void Initialize(const char* path) override;
	
};