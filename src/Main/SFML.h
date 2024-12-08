#pragma once

#include "Generic.h"

namespace sf 
{
	class RenderWindow;
	class Sprite;
	class Texture;
}

class SFMLWindow : public IWindow
{
	sf::RenderWindow* mpWindow;

public:
	virtual void Initialize(HINSTANCE hInstance, unsigned int width, unsigned int height, const char* title) override;
	virtual void Clear() override;
	virtual void Draw(IDrawable* pDrawable) override;
	virtual void Render() override;

	friend class SFMLSprite;
};

class SFMLTexture : public ITexture
{
	sf::Texture* mpTexture;

public:
	void Load(const char* path) override;
	void GetWidth() override;
	void GetHeight() override;

	const sf::Texture& Get() { return *mpTexture; }
};

class SFMLSprite : public ISprite
{
	sf::Sprite* mpSprite;

public:
	void SetTexture(ITexture* pTexture) override;
	void SetPosition(float x, float y) override;
	void Draw(IWindow* pWindow) override;

	const sf::Sprite& Get() { return *mpSprite; }
};
