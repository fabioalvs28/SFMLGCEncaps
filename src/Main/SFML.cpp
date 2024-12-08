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

void SFMLWindow::Draw(LEDrawable* pDrawable)
{
	pDrawable->Draw(this);
}

void SFMLWindow::Render()
{
	mpWindow->display();
}

void SFMLSprite::SetTexture(LETexture* pTexture)
{
	mpSprite = new sf::Sprite();

	SFMLTexture* pSFMLTexture = (SFMLTexture*)pTexture;

	mpSprite->setTexture(pSFMLTexture->Get());
}

void SFMLSprite::SetPosition(float x, float y)
{
	mpSprite->setPosition(x, y);
}

void SFMLSprite::Draw(LEWindow* pWindow)
{
	SFMLWindow* pSFMLWindow = (SFMLWindow*)pWindow;
	pSFMLWindow->mpWindow->draw(*mpSprite);
}

void SFMLTexture::Load(const char* path)
{
	mpTexture = new sf::Texture();
	mpTexture->loadFromFile(path);
}

void SFMLTexture::GetWidth()
{

}

void SFMLTexture::GetHeight()
{
}