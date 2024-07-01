#include "pch.h"

#include <iostream>
// #include <Windows.h>
// #include "Window.h"
// #include "EventSystem.h"
// #include "Log.h"
#include "Vectors.h"
#include "GCColor.h"
#include "Quaternion.h"
#include "Matrix.h"
#include "Transform.h"
#include "Components.h"
#include "GameObject.h"
#include "SceneManager.h"
// #include "GameManager.h"
#include "Scene.h"
#include "SFML/Graphics.hpp"




int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML window");


	sf::Texture marioTexture;
	if (!marioTexture.loadFromFile("Mario.png")) return EXIT_FAILURE;
	sf::Sprite marioSprite(marioTexture);
	marioSprite.setOrigin(sf::Vector2f(marioTexture.getSize().x / 2, marioTexture.getSize().y / 2));

	sf::Texture luigiTexture;
	if (!luigiTexture.loadFromFile("Luigi.png")) return EXIT_FAILURE;
	sf::Sprite luigiSprite(luigiTexture);
	luigiSprite.setOrigin(sf::Vector2f(luigiTexture.getSize().x / 2, luigiTexture.getSize().y / 2));

	sf::Texture goombaTexture;
	if (!goombaTexture.loadFromFile("Goomba.png")) return EXIT_FAILURE;
	sf::Sprite goombaSprite(goombaTexture);
	goombaSprite.setOrigin(sf::Vector2f(goombaTexture.getSize().x / 2, goombaTexture.getSize().y / 2));




	GCScene* pTest1Scene = GCScene::Create();

	GC::

	GCGameObject* pMario = nullptr;
	GCGameObject* pLuigi = nullptr;
	GCGameObject* pGoomba = nullptr;

	GCGameObject* pSelected = nullptr;

	
	pMario->AddComponent<BoxCollider>();
	pLuigi->AddComponent<BoxCollider>();

	float angle = 0;

	for (int framesSinceStart = 0; window.isOpen(); framesSinceStart++)
	{
		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();

		window.clear();



		if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
		{
			if (pMario == nullptr)
			{
				pMario = pTest1Scene->CreateGameObject();
				pMario->SetName("Mario");
				pMario->AddTag("Bros Brother");
			}
			pSelected = pMario;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
		{
			if (pLuigi == nullptr)
			{
				pLuigi = pTest1Scene->CreateGameObject();
				pLuigi->SetName("Luigi");
				pLuigi->AddTag("Bros Brother");
			}
			pSelected = pLuigi;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
		{
			if (pGoomba == nullptr)
			{
				pGoomba = pTest1Scene->CreateGameObject();
				pGoomba->SetName("Goomba");
				pGoomba->AddTag("Enemy");
			}
			pSelected = pGoomba;
		}



		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) pSelected->m_transform.m_position += GCVEC3::Up() * 0.3f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) pSelected->m_transform.m_position += GCVEC3::Down() * 0.3f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) pSelected->m_transform.m_position += GCVEC3::Left() * 0.3f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) pSelected->m_transform.m_position += GCVEC3::Right() * 0.3f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) pSelected->m_transform.Scale(GCVEC3::One() * 1.01f);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) pSelected->m_transform.Scale(GCVEC3::One() * 0.99f);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			pSelected->m_transform.Rotate(0, 0, 0.2f * (PI / 100.f));
			angle += 0.2f;
			std::cout << "Rotation A: {" << pSelected->m_transform.m_rotation.x << " ; " << pSelected->m_transform.m_rotation.y << " ; " << pSelected->m_transform.m_rotation.z << "}" << std::endl;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		{
			pSelected->m_transform.Rotate(0, 0, -0.2f * (PI / 100.f));
			angle -= 0.2f;
		}




		if (pMario != nullptr)
		{
			//std::cout << "{" << (int)pMario->m_transform.m_rotation.x << " ; " << (int)pMario->m_transform.m_rotation.y << " ; " << (int)pMario->m_transform.m_rotation.z << "}" << std::endl;
			marioSprite.setPosition(sf::Vector2f(pMario->m_transform.m_position.x, pMario->m_transform.m_position.y));
			marioSprite.setScale(sf::Vector2f(pMario->m_transform.m_scale.x, pMario->m_transform.m_scale.y));

			marioSprite.setRotation(angle);
			window.draw(marioSprite);
		}

		if (pLuigi != nullptr)
		{
			luigiSprite.setPosition(sf::Vector2f(pLuigi->m_transform.m_position.x, pLuigi->m_transform.m_position.y));
			luigiSprite.setScale(sf::Vector2f(pLuigi->m_transform.m_scale.x, pLuigi->m_transform.m_scale.y));
			window.draw(luigiSprite);
		}

		if (pGoomba != nullptr)
		{
			goombaSprite.setPosition(sf::Vector2f(pGoomba->m_transform.m_position.x, pGoomba->m_transform.m_position.y));
			goombaSprite.setScale(sf::Vector2f(pGoomba->m_transform.m_scale.x, pGoomba->m_transform.m_scale.y));
			window.draw(goombaSprite);
		}




		window.display();
	}

	return EXIT_SUCCESS;

	/*LogEngineDebug("Engine Debug");
	LogEngineError("Engine Error");
	LogEngineWarn("Engine Warn");
	LogEngineInfo("Engine Info");
	LogEngineTrace("Engine Trace");
	LogGameDebug("Game Debug");*/
}