#pragma once

#include <SFML/System/Vector2.hpp>

#include "Generic.h"

namespace sf 
{
	class RenderWindow;

	class Drawable;
	class Transformable;
	class Texture;
	class Sprite;
	class CircleShape;
}

class SFMLWindow : public IWindow
{
	sf::RenderWindow* mpWindow;

public:
	virtual void Initialize(int width, int height, const char* title) override;
	virtual void Clear() override;
	virtual void Draw(IObject* pObject) override;
	virtual void Render() override;

	friend class SFMLSprite;
};

class SFMLObject : public IObject
{
protected:
	sf::Drawable* mpDrawable;
	sf::Transformable* mpTransformable;

public:
	sf::Drawable& Get() const { return *mpDrawable; }
	sf::Transformable& GetTransformable() const { return *mpTransformable; }
	void SetPosition(float x, float y);
};

class SFMLTexture : public ITexture
{
	sf::Texture* mpTexture;

public:
	SFMLTexture();

	void Load(const char* path) override;
	void GetWidth() override;
	void GetHeight() override;

	const sf::Texture& Get() { return *mpTexture; }
};

class SFMLSprite : public ISprite, public SFMLObject
{
public:
	SFMLSprite();

	void SetTexture(ITexture* pTexture) override;
};

class SFMLCircle : public ICircle, public SFMLObject
{
public:
	SFMLCircle();

	void SetRadius(float radius) override;
	void SetColor(unsigned char r, unsigned char g, unsigned char b) override;
};


class SFMLEntity : public IEntity, public SFMLObject
{
protected:
	struct Target 
	{
		sf::Vector2i position;
		float distance;
		bool isSet;
	};
	
	sf::Vector2f mDirection;
	Target mTarget;
	float mSpeed;

public:
	SFMLEntity();

	// enum Tag
	// {
	// 	PLAYER,
	// 	PPROJECTILES,
	// 	EPROJECTILES,
	// 	ENNEMY,
	//
	// 	TAG_COUNT
	//
	// };

	void Update() override;
	void FixedUpdate(float dt) override;
	void Initialize(const char* path) override;
	
	sf::Vector2f& GetPosition(float ratioX = 0.5f, float ratioY = 0.5f) const;
};
