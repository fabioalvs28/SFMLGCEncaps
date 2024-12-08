#pragma once

#include "Generic.h"

namespace sf 
{
	class RenderWindow;
	class Sprite;
	class Texture;
}

class SFMLWindow : public LEWindow
{
	sf::RenderWindow* mpWindow;

public:
	virtual void Initialize(HINSTANCE hInstance, unsigned int width, unsigned int height, const char* title) override;
	virtual void Clear() override;
	virtual void Draw(LEDrawable* pDrawable) override;
	virtual void Render() override;

	friend class SFMLSprite;
};

class SFMLTexture : public LETexture
{
	sf::Texture* mpTexture;

public:
	void Load(const char* path) override;
	void GetWidth() override;
	void GetHeight() override;

	const sf::Texture& Get() { return *mpTexture; }
};

class SFMLSprite : public LESprite
{
	sf::Sprite* mpSprite;

public:
	void SetTexture(LETexture* pTexture) override;
	void SetPosition(float x, float y) override;
	void Draw(LEWindow* pWindow) override;

	const sf::Sprite& Get() { return *mpSprite; }
};
