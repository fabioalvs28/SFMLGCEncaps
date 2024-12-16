#pragma once

class IObject;

class IWindow
{
public:
	virtual void Initialize(int width, int height, const char* title) = 0;
	virtual void Clear() = 0;
	virtual void Draw(IObject* pDrawable) = 0;
	virtual void Render() = 0;
};

class ITexture
{
public:
	virtual void Load(const char* path) = 0;
	virtual void GetWidth() = 0;
	virtual void GetHeight() = 0;
};

class IObject
{
	virtual void SetPosition(float x, float y) = 0;
};

class ISprite
{
public:
	virtual void SetTexture(ITexture* pTexture) = 0;
};

class ICircle
{
public:
	virtual void SetRadius(float radius) = 0;
	virtual void SetColor(unsigned char r, unsigned char g, unsigned char b) = 0;
};

