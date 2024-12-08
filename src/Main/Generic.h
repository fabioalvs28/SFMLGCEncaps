#pragma once

class IDrawable;

class IWindow
{
public:
	virtual void Initialize(HINSTANCE hInstance, unsigned int width, unsigned int height, const char* title) = 0;
	virtual void Clear() = 0;
	virtual void Draw(IDrawable* pDrawable) = 0;
	virtual void Render() = 0;
};

class ITexture
{
public:
	virtual void Load(const char* path) = 0;
	virtual void GetWidth() = 0;
	virtual void GetHeight() = 0;
};

class IDrawable
{
public:
	virtual void Draw(IWindow* pWindow) = 0;
};

class ISprite : public IDrawable
{
public:
	virtual void SetTexture(ITexture* pTexture) = 0;
	virtual void SetPosition(float x, float y) = 0;
	virtual void Draw(IWindow* pWindow) = 0;

};

class ICircle : public IDrawable
{
public:
	virtual void SetPosition(float x, float y) = 0;
	virtual void SetRadius(float radius) = 0;
	virtual void SetColor(float r, float g, float b, float a) = 0;
};

