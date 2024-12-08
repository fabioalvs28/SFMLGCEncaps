#pragma once

class LEDrawable;

class LEWindow
{
public:
	virtual void Initialize(HINSTANCE hInstance, unsigned int width, unsigned int height, const char* title) = 0;
	virtual void Clear() = 0;
	virtual void Draw(LEDrawable* pDrawable) = 0;
	virtual void Render() = 0;
};

class LETexture
{
public:
	virtual void Load(const char* path) = 0;
	virtual void GetWidth() = 0;
	virtual void GetHeight() = 0;
};

class LEDrawable
{
public:
	virtual void Draw(LEWindow* pWindow) = 0;
};

class LESprite : public LEDrawable
{
public:
	virtual void SetTexture(LETexture* pTexture) = 0;
	virtual void SetPosition(float x, float y) = 0;
	virtual void Draw(LEWindow* pWindow) = 0;

};

class GenericCircle : public LEDrawable
{
public:
	virtual void SetPosition(float x, float y) = 0;
	virtual void SetRadius(float radius) = 0;
	virtual void SetColor(float r, float g, float b, float a) = 0;
};

