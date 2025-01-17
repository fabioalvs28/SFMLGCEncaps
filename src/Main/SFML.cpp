#include "pch.h"
#include "SFML.h"

#include <SFML/Graphics.hpp>

#pragma region Window
void SFMLWindow::Initialize(int width, int height, const char* title)
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

void SFMLWindow::HandleInput()
{
	sf::Event event;
	while (mpWindow->pollEvent(event))
	{
		switch (event.type) // NOLINT(clang-diagnostic-switch-enum)
		{
		case sf::Event::Closed:
			{
				mpWindow->close();
				break;
			}
		case sf::Event::KeyPressed:
			{
				switch (event.key.scancode) // NOLINT(clang-diagnostic-switch-enum)
				{
				case (sf::Keyboard::Scancode::Escape):
					{
						mpWindow->close();
						break;
					}
				default: break;
				}
				break;
			}
		}
	}
}


void SFMLWindow::Render()
{
	mpWindow->display();
}
#pragma endregion

#pragma region Object

void SFMLObject::SetPosition(float x, float y)
{
	mpTransformable->setPosition(x, y);
}
#pragma endregion

#pragma region Sprite
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
#pragma endregion

#pragma region Texture
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

#pragma endregion

#pragma region Circle
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
#pragma endregion

#pragma region Entity
SFMLEntity::SFMLEntity()
{
	sf::Sprite* pSprite = new sf::Sprite();

	mpDrawable = pSprite;
	mpTransformable = pSprite;
}

void SFMLEntity::Initialize(const char* path)
{

	mDirection = sf::Vector2f(0.0f, 0.0f);
	mSpeed = 0.0f;
	
	SFMLTexture* pTemp = new SFMLTexture();
	pTemp->Load(path);
	static_cast<sf::Sprite*>(mpDrawable)->setTexture(pTemp->Get());
	delete pTemp;
	
	mTarget.isSet = false;
}

sf::Vector2f& SFMLEntity::GetPosition(float ratioX, float ratioY) const
{
	sf::Vector2f position;
	position.x = GetTransformable().getPosition().x;
	position.y = GetTransformable().getPosition().y;
	return position;
}

void SFMLEntity::FixedUpdate(float dt)
{
	float x = GetPosition().x / 2;
	float y = GetPosition().y / 2;
	float distance = dt * mSpeed;
	sf::Vector2f translation = distance * mDirection;
	((sf::Sprite*)mpDrawable)->move(translation);

	std::cout << GetPosition().x;
	
	if (mTarget.isSet)
	{
		mTarget.distance -= distance;

		if (mTarget.distance <= 0.f)
		{
			SetPosition(static_cast<float>(mTarget.position.x), static_cast<float>(mTarget.position.y));
			mDirection = sf::Vector2f(0.f, 0.f);
			mTarget.isSet = false;
		}
	}
}

void SFMLEntity::Update()
{
	//
}
#pragma endregion

