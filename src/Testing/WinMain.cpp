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

#include "PhysicManager.h"
#include "Log.h"




int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML window");


	sf::Texture marioTexture;
	if (!marioTexture.loadFromFile("Circle.png")) return EXIT_FAILURE;
	sf::Sprite marioSprite(marioTexture);
	//marioSprite.setOrigin(sf::Vector2f(marioTexture.getSize().x / 2, marioTexture.getSize().y / 2));
	
	//sf::CircleShape marioCircle(marioTexture.getSize().x / 2);
	////marioCircle.setOrigin(sf::Vector2f(marioTexture.getSize().x / 2, marioTexture.getSize().y / 2));
	//marioCircle.setFillColor(sf::Color::Transparent);
	//marioCircle.setOutlineColor(sf::Color::Red);
	//marioCircle.setOutlineThickness(1.f);
	
	sf::RectangleShape marioBox(sf::Vector2f(marioTexture.getSize().x, marioTexture.getSize().y));
	//marioBox.setOrigin(sf::Vector2f(marioTexture.getSize().x / 2, marioTexture.getSize().y / 2));
	marioBox.setFillColor(sf::Color::Transparent);
	marioBox.setOutlineColor(sf::Color::Red);
	marioBox.setOutlineThickness(1.f);

	sf::Texture luigiTexture;
	if (!luigiTexture.loadFromFile("Circle.png")) return EXIT_FAILURE;
	sf::Sprite luigiSprite(luigiTexture);
	//luigiSprite.setOrigin(sf::Vector2f(luigiTexture.getSize().x / 2, luigiTexture.getSize().y / 2));
	sf::CircleShape luigiCircle(luigiTexture.getSize().x / 2);
	//luigiCircle.setOrigin(sf::Vector2f(luigiTexture.getSize().x / 2, luigiTexture.getSize().y / 2));
	luigiCircle.setFillColor(sf::Color::Transparent);
	luigiCircle.setOutlineColor(sf::Color::Green);
	luigiCircle.setOutlineThickness(1.f);
	//sf::RectangleShape luigiBox(sf::Vector2f(luigiTexture.getSize().x, luigiTexture.getSize().y));
	////luigiBox.setOrigin(sf::Vector2f(luigiTexture.getSize().x / 2, luigiTexture.getSize().y / 2));
	//luigiBox.setFillColor(sf::Color::Transparent);
	//luigiBox.setOutlineColor(sf::Color::Green);
	//luigiBox.setOutlineThickness(1.f);

	sf::Texture goombaTexture;
	if (!goombaTexture.loadFromFile("Goomba.png")) return EXIT_FAILURE;
	sf::Sprite goombaSprite(goombaTexture);
	//goombaSprite.setOrigin(sf::Vector2f(goombaTexture.getSize().x / 2, goombaTexture.getSize().y / 2));
	sf::RectangleShape goombaBox(sf::Vector2f(goombaTexture.getSize().x, goombaTexture.getSize().y));
	//goombaBox.setOrigin(sf::Vector2f(goombaTexture.getSize().x / 2, goombaTexture.getSize().y / 2));
	goombaBox.setFillColor(sf::Color::Transparent);
	goombaBox.setOutlineColor(sf::Color::Red);
	goombaBox.setOutlineThickness(1.f);




	GCScene* pTest1Scene = GCScene::Create();

	GCGameObject* pMario = pTest1Scene->CreateGameObject();
	pMario->SetName("Mario");
	pMario->AddTag("Bros Brother");
	pMario->AddComponent<BoxCollider>()->SetSize({ static_cast<float>(marioTexture.getSize().x), static_cast<float>(marioTexture.getSize().y) });
	//pMario->AddComponent<CircleCollider>()->SetRadius(marioTexture.getSize().x / 2);

	GCGameObject* pLuigi = pTest1Scene->CreateGameObject();
	pLuigi->SetName("Luigi");
	pLuigi->AddTag("Bros Brother");
	//pLuigi->AddComponent<BoxCollider>()->SetSize({ static_cast<float>(luigiTexture.getSize().x), static_cast<float>(luigiTexture.getSize().y) });
	pLuigi->AddComponent<CircleCollider>()->SetRadius(luigiTexture.getSize().x / 2);

	GCGameObject* pGoomba = nullptr;

	GCGameObject* pSelected = pLuigi;

	
	// They should be registered to the physic manager to work
	//pMario->AddComponent<BoxCollider>();
	//pLuigi->AddComponent<BoxCollider>();

	float angle = 0;

	for (int framesSinceStart = 0; window.isOpen(); framesSinceStart++)
	{
		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();

		window.clear();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
		{
			if (pMario == nullptr || pLuigi == nullptr)
				continue;

			//LogGameDebug(GCPhysic::CheckBox2DvsBox2D(*pMario->GetComponent<BoxCollider>(), *pLuigi->GetComponent<BoxCollider>()) ? "Collision\n\n\n\n" : "No collision\n\n\n\n");
			//LogGameDebug(GCPhysic::CheckCirclevsCircle(*pMario->GetComponent<CircleCollider>(), *pLuigi->GetComponent<CircleCollider>()) ? "Collision\n\n\n\n" : "No collision\n\n\n\n");
			LogGameDebug(GCPhysic::CheckBox2DvsCircle(*pMario->GetComponent<BoxCollider>(), *pLuigi->GetComponent<CircleCollider>()) ? "Collision\n\n\n\n" : "No collision\n\n\n\n");
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
		{
			if (pMario == nullptr)
			{
				pMario = pTest1Scene->CreateGameObject();
				pMario->SetName("Mario");
				pMario->AddTag("Bros Brother");

				pMario->AddComponent<BoxCollider>();
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

				pLuigi->AddComponent<BoxCollider>();
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



		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) pSelected->m_transform.m_position += GCVEC3::Up() * 0.03f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) pSelected->m_transform.m_position += GCVEC3::Down() * 0.03f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) pSelected->m_transform.m_position += GCVEC3::Left() * 0.03f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) pSelected->m_transform.m_position += GCVEC3::Right() * 0.03f;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) pSelected->m_transform.Scale(GCVEC3::One() * 1.01f);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) pSelected->m_transform.Scale(GCVEC3::One() * 0.99f);

		marioBox.setPosition(sf::Vector2f(pMario->m_transform.m_position.x, pMario->m_transform.m_position.y));
		luigiCircle.setPosition(sf::Vector2f(pLuigi->m_transform.m_position.x, pLuigi->m_transform.m_position.y));
		//goombaBox.setPosition(sf::Vector2f(pGoomba->m_transform.m_position.x, pGoomba->m_transform.m_position.y));

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
			window.draw(marioBox);
			//window.draw(marioCircle);
		}

		if (pLuigi != nullptr)
		{
			luigiSprite.setPosition(sf::Vector2f(pLuigi->m_transform.m_position.x, pLuigi->m_transform.m_position.y));
			luigiSprite.setScale(sf::Vector2f(pLuigi->m_transform.m_scale.x, pLuigi->m_transform.m_scale.y));
			window.draw(luigiSprite);
			//window.draw(luigiBox);
			window.draw(luigiCircle);
		}

		if (pGoomba != nullptr)
		{
			goombaSprite.setPosition(sf::Vector2f(pGoomba->m_transform.m_position.x, pGoomba->m_transform.m_position.y));
			goombaSprite.setScale(sf::Vector2f(pGoomba->m_transform.m_scale.x, pGoomba->m_transform.m_scale.y));
			window.draw(goombaSprite);
			window.draw(goombaBox);
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