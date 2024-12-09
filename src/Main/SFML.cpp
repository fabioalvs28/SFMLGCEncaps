#include "pch.h"
#include "SFML.h"

#include <SFML/Graphics.hpp>

void SFMLWindow::Initialize(HINSTANCE hInstance, unsigned int width, unsigned int height, const char* title)
{
	mpWindow = new sf::RenderWindow(sf::VideoMode(width, height), "SFML works!");
}

void SFMLWindow::Clear()
{
	mpWindow->clear();
}

void SFMLWindow::Draw(IObject* pDrawable)
{
	SFMLObject* pSFMLDrawable = (SFMLObject*)pDrawable;
	mpWindow->draw(pSFMLDrawable->Get());
}

void SFMLWindow::Render()
{
	mpWindow->display();
}

void SFMLObject::SetPosition(float x, float y)
{
	mpTransformable->setPosition(x, y);
}

SFMLSprite::SFMLSprite()
{
	sf::Sprite* pSprite = new sf::Sprite();
	
	mpDrawable = pSprite;
	mpTransformable = pSprite;
}

void SFMLSprite::SetTexture(ITexture* pTexture)
{
	SFMLTexture* pSFMLTexture = (SFMLTexture*)pTexture;

	((sf::Sprite*)mpDrawable)->setTexture(pSFMLTexture->Get());
}

SFMLTexture::SFMLTexture()
{
	mpTexture = new sf::Texture();
}

void SFMLTexture::Load(const char* path)
{
	mpTexture->loadFromFile(std::string(path) + ".png");
}

void SFMLTexture::GetWidth()
{
	mpTexture->getSize().x;
}

void SFMLTexture::GetHeight()
{
	mpTexture->getSize().y;
}

SFMLCircle::SFMLCircle()
{
	sf::CircleShape* pCircle = new sf::CircleShape();

	mpDrawable = pCircle;
	mpTransformable = pCircle;
}

void SFMLCircle::SetRadius(float radius)
{
	((sf::CircleShape*)mpDrawable)->setRadius(radius);
}

void SFMLCircle::SetColor(unsigned char r, unsigned char g, unsigned char b)
{
	((sf::CircleShape*)mpDrawable)->setFillColor(sf::Color(r, g, b));
}
