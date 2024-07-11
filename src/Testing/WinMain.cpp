


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
#include "Scene.h"
#include "SceneManager.h"
#include "GameManager.h"
#include "GC.h"
#include "RenderManager.h"
#include "Components.h"
#include "InputManager.h"
//#include "SFML/Graphics.hpp"

#include "PhysicManager.h"
#include "Log.h"


#include "../../src/render/framework.h"




void CreateConsole()
{
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);
}

struct GCTest : GCSHADERCB {
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4 color;
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	//GCKeyboardInputManager keyinput; 

	GC::m_pActiveGameManager.Init();


	GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();
	profiler.InitializeConsole();

	Window* window = new Window(hInstance);
	window->Initialize();

	GC::m_pActiveGameManager.m_pRenderManager.m_pGraphics->Initialize(window, 1920, 1080);


	GC::m_pActiveGameManager.m_pRenderManager.CreateGeometry();

	GCScene* pScene = GCScene::Create();
	GCGameObject* test1 = pScene->CreateGameObject();
	GCGameObject* test2 = pScene->CreateGameObject();
	GCGameObject* test3 = pScene->CreateGameObject();


	test1->AddComponent<BoxCollider>();
	test2->AddComponent<SpriteRenderer>(); 
	test3->AddComponent<CircleCollider>();

	int flagsTexture = 0;
	SET_FLAG(flagsTexture, HAS_POSITION);
	SET_FLAG(flagsTexture, HAS_UV);

	int flagsColor = 0;
	SET_FLAG(flagsColor, HAS_POSITION);
	SET_FLAG(flagsColor, HAS_COLOR);

	test1->m_transform.m_position.x = 100;
	test1->m_transform.m_position.y = 100;

	test2->m_transform.m_position.x = 200;
	test2->m_transform.m_position.y = 200;

	test3->m_transform.m_position.x = 500;
	test3->m_transform.m_position.y = 200;


	test2->GetComponent<SpriteRenderer>()->SetSprite("../../../src/Render/Textures/texture.dds");

	//test1.pos = { 400,400 };
	//test2.pos = { 800,800 };

	//// Geometry (Resource)
	//GCGeometry* geo = graphics->CreateGeometryPrimitiveTexture("plane");
	//GCGeometry* geo1 = graphics->CreateGeometryPrimitiveTexture("cube");
	//GCGeometry* geo2 = graphics->CreateGeometryModelParserColor("../../../src/Render/monkeyUv.obj", DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f), obj);

	//std::string shaderFilePath = "../../../src/Render/Shaders/customTest.hlsl";
	//std::string csoDestinationPath = "../../../src/Render/CsoCompiled/custom";

	//GCShader* shader1 = graphics->CreateShaderColor();
	////GCShader* shader2 = graphics->CreateShaderCustom(shaderFilePath, csoDestinationPath, flagsTexture);

	//GCShader* shader2 = graphics->CreateShaderTexture();

	//graphics->InitializeGraphicsResourcesStart();

	//// Mesh (Resource)
	//GCMesh* mesh = graphics->CreateMesh(geo);
	////GCMesh* mesh1 = graphics->CreateMesh(geo1);

	//GCMesh* mesh2 = graphics->CreateMesh(geo2);

	//// Texture (Resource)
	//std::string texturePath = "../../../src/Render/Textures/texture.dds";
	//GCTexture* texture = graphics->CreateTexture(texturePath);

	//graphics->InitializeGraphicsResourcesEnd();

	//// Material (Resource)
	//GCMaterial* material = graphics->CreateMaterial(shader1);
	//GCMaterial* material2 = graphics->CreateMaterial(shader2);
	//material2->SetTexture(texture);


	//##################################


	// PERSPECTIVE 

	//DirectX::XMVECTOR cameraPosition = DirectX::XMVectorSet(0.0f, -10.0f, 5.0f, 1.0f);
	//DirectX::XMVECTOR targetPosition = DirectX::XMVectorZero();
	//DirectX::XMVECTOR upVector = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);


	//DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, window->AspectRatio(), 1.0f, 1000.0f);
	//DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(cameraPosition, targetPosition, upVector);

	//DirectX::XMMATRIX transposedProjectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);
	//DirectX::XMMATRIX transposedViewMatrix = DirectX::XMMatrixTranspose(viewMatrix);

	//DirectX::XMFLOAT4X4 storedProjectionMatrix;
	//DirectX::XMFLOAT4X4 storedViewMatrix;

	//DirectX::XMStoreFloat4x4(&storedProjectionMatrix, transposedProjectionMatrix);
	//DirectX::XMStoreFloat4x4(&storedViewMatrix, transposedViewMatrix);
	// ***********

//
////// SET CAMERA
//	DirectX::XMVECTOR cameraPosition = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f);  // Position de la caméra
//	DirectX::XMVECTOR targetPosition = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f); // Point visé par la caméra
//	DirectX::XMVECTOR upVector = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);       // Vecteur "up" de la caméra
//
//	// Projection orthographique
//	float viewWidth = 10.0f;    // Largeur de la vue
//	float viewHeight = 10.0f;   // Hauteur de la vue
//	float nearZ = 0.1f;         // Plan proche
//	float farZ = 100.0f;        // Plan éloigné
//
//	DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ);
//	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(cameraPosition, targetPosition, upVector);
//
//	DirectX::XMMATRIX transposedProjectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);
//	DirectX::XMMATRIX transposedViewMatrix = DirectX::XMMatrixTranspose(viewMatrix);
//
//	DirectX::XMFLOAT4X4 storedProjectionMatrix;
//	DirectX::XMFLOAT4X4 storedViewMatrix;
//
//	DirectX::XMStoreFloat4x4(&storedProjectionMatrix, transposedProjectionMatrix);
//	DirectX::XMStoreFloat4x4(&storedViewMatrix, transposedViewMatrix);
	//
	//	// ****************

		/*
		float left = -1.0f;
		float right = 1.0f;
		float bottom = -1.0f;
		float top = 1.0f;

		DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixOrthographicOffCenterLH(left, right, bottom, top, 0.0f, 1.0f);
		DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixIdentity();

		DirectX::XMMATRIX transposedProjectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);
		DirectX::XMMATRIX transposedViewMatrix = DirectX::XMMatrixTranspose(viewMatrix);

		DirectX::XMFLOAT4X4 storedProjectionMatrix;
		DirectX::XMFLOAT4X4 storedViewMatrix;

		DirectX::XMStoreFloat4x4(&storedProjectionMatrix, transposedProjectionMatrix);
		DirectX::XMStoreFloat4x4(&storedViewMatrix, transposedViewMatrix);
		*/

		// SET WORLD

	DirectX::XMFLOAT4X4 I(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		1.5f, 0.0f, 0.0f, 1.0f);

	DirectX::XMFLOAT4X4 transposedWorld;
	DirectX::XMStoreFloat4x4(&transposedWorld, DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&I)));


		//// ***********

		//graphics->StartFrame();

		//// DRAW -> ONE FRAME
		//graphics->UpdateViewProjConstantBuffer(storedProjectionMatrix, storedViewMatrix);

		//GCTest test = graphics->ToPixel<GCTest>(800, 800, storedProjectionMatrix, storedViewMatrix);

		////GCTest test;

		////DirectX::XMMATRIX identityMatrix = DirectX::XMMatrixIdentity();
		////DirectX::XMStoreFloat4x4(&test.world, identityMatrix);
		////test.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f);

		//graphics->UpdateCustomCBObject<GCTest>(material2, test);
		//graphics->GetRender()->DrawObject(mesh, material2);

		////graphics->UpdateCustomCBObject<GCTest>(material, worldData);
		////graphics->GetRender()->DrawObject(mesh, material);

		////profiler.LogInfo(std::to_string(material2->GetObjectCBData().size()));


		//graphics->EndFrame();
	//	//// ********************

	GC::m_pActiveGameManager.m_pRenderManager.Render();

	window->Run(GC::m_pActiveGameManager.m_pRenderManager.m_pGraphics->GetRender());

	//// Loop Again < |||| >

	//graphics->GetRender()->PrepareDraw();

	//graphics->GetRender()->PostDraw();

	// *


	//window->Run(graphics->GetRender());

	//CreateConsole();
	//ECSTesting::GetInstance().RunTests();
	//std::cout << "Logging to console..." << std::endl;

	//GCEventSystem* ev = new GCEventSystem();
	//GCWindow* w = new GCWindow(hInstance, nCmdShow, *ev);
	//w->Create(L"Testing", 800, 600);
	//w->Show(nCmdShow);

	//GCGameManager* GameManager = new GCGameManager();
	//GameManager->Init();

	//GCScene* myScene = GCSceneManager::CreateScene();
	//myScene->SetActive(true);

	//GCGameObject* myGoOne = myScene->CreateGameObject("go", true);
	//GCGameObject* myGoTwo = myScene->CreateGameObject("go2", true);

	//myGoOne->AddComponent<SpriteRenderer>();
	//myGoTwo->AddComponent<SpriteRenderer>();

	//GCColor color = GCColor(1.0f, 1.0f, 1.0f, 1.0f);
	//myGoOne->GetComponent<SpriteRenderer>()->SetColor(color);
	//myGoTwo->RemoveComponent<SpriteRenderer>();
	//myGoOne->AddComponent<BoxCollider>();
	//myGoOne->Destroy();

	//while (w->IsRunning())
	//{

	//	myGoTwo->m_transform.m_position.x += 0.01f;

	//	std::cout << "Position  " << myGoTwo->m_transform.m_position.x << std::endl;


	//	GameManager->Update();
	//	w->PollEvents();
	//}


	//FreeConsole();

}

//
//int main()
//{
//	sf::RenderWindow window(sf::VideoMode(960, 540), "SFML window");
//
//
//	sf::Texture marioTexture;
//	if (!marioTexture.loadFromFile("Circle.png")) return EXIT_FAILURE;
//	sf::Sprite marioSprite(marioTexture);
//	//marioSprite.setOrigin(sf::Vector2f(marioTexture.getSize().x / 2, marioTexture.getSize().y / 2));
//	
//	//sf::CircleShape marioCircle(marioTexture.getSize().x / 2);
//	////marioCircle.setOrigin(sf::Vector2f(marioTexture.getSize().x / 2, marioTexture.getSize().y / 2));
//	//marioCircle.setFillColor(sf::Color::Transparent);
//	//marioCircle.setOutlineColor(sf::Color::Red);
//	//marioCircle.setOutlineThickness(1.f);
//	
//	sf::RectangleShape marioBox(sf::Vector2f(marioTexture.getSize().x, marioTexture.getSize().y));
//	//marioBox.setOrigin(sf::Vector2f(marioTexture.getSize().x / 2, marioTexture.getSize().y / 2));
//	marioBox.setFillColor(sf::Color::Transparent);
//	marioBox.setOutlineColor(sf::Color::Red);
//	marioBox.setOutlineThickness(1.f);
//
//	sf::Texture luigiTexture;
//	if (!luigiTexture.loadFromFile("Circle.png")) return EXIT_FAILURE;
//	sf::Sprite luigiSprite(luigiTexture);
//	//luigiSprite.setOrigin(sf::Vector2f(luigiTexture.getSize().x / 2, luigiTexture.getSize().y / 2));
//	sf::CircleShape luigiCircle(luigiTexture.getSize().x / 2);
//	//luigiCircle.setOrigin(sf::Vector2f(luigiTexture.getSize().x / 2, luigiTexture.getSize().y / 2));
//	luigiCircle.setFillColor(sf::Color::Transparent);
//	luigiCircle.setOutlineColor(sf::Color::Green);
//	luigiCircle.setOutlineThickness(1.f);
//	//sf::RectangleShape luigiBox(sf::Vector2f(luigiTexture.getSize().x, luigiTexture.getSize().y));
//	////luigiBox.setOrigin(sf::Vector2f(luigiTexture.getSize().x / 2, luigiTexture.getSize().y / 2));
//	//luigiBox.setFillColor(sf::Color::Transparent);
//	//luigiBox.setOutlineColor(sf::Color::Green);
//	//luigiBox.setOutlineThickness(1.f);
//
//	sf::Texture goombaTexture;
//	if (!goombaTexture.loadFromFile("Goomba.png")) return EXIT_FAILURE;
//	sf::Sprite goombaSprite(goombaTexture);
//	//goombaSprite.setOrigin(sf::Vector2f(goombaTexture.getSize().x / 2, goombaTexture.getSize().y / 2));
//	sf::RectangleShape goombaBox(sf::Vector2f(goombaTexture.getSize().x, goombaTexture.getSize().y));
//	//goombaBox.setOrigin(sf::Vector2f(goombaTexture.getSize().x / 2, goombaTexture.getSize().y / 2));
//	goombaBox.setFillColor(sf::Color::Transparent);
//	goombaBox.setOutlineColor(sf::Color::Red);
//	goombaBox.setOutlineThickness(1.f);
//
//
//
//
//	GCScene* pTest1Scene = GCScene::Create();
//
//	GCGameObject* pMario = pTest1Scene->CreateGameObject();
//	pMario->SetName("Mario");
//	pMario->AddTag("Bros Brother");
//	pMario->AddComponent<BoxCollider>()->SetSize({ static_cast<float>(marioTexture.getSize().x), static_cast<float>(marioTexture.getSize().y) });
//	//pMario->AddComponent<CircleCollider>()->SetRadius(marioTexture.getSize().x / 2);
//
//	GCGameObject* pLuigi = pTest1Scene->CreateGameObject();
//	pLuigi->SetName("Luigi");
//	pLuigi->AddTag("Bros Brother");
//	//pLuigi->AddComponent<BoxCollider>()->SetSize({ static_cast<float>(luigiTexture.getSize().x), static_cast<float>(luigiTexture.getSize().y) });
//	pLuigi->AddComponent<CircleCollider>()->SetRadius(luigiTexture.getSize().x / 2);
//
//	GCGameObject* pGoomba = nullptr;
//
//	GCGameObject* pSelected = pLuigi;
//
//	
//	// They should be registered to the physic manager to work
//	//pMario->AddComponent<BoxCollider>();
//	//pLuigi->AddComponent<BoxCollider>();
//
//	float angle = 0;
//
//	for (int framesSinceStart = 0; window.isOpen(); framesSinceStart++)
//	{
//		sf::Event event;
//		while (window.pollEvent(event))
//			if (event.type == sf::Event::Closed)
//				window.close();
//
//		window.clear();
//
//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
//		{
//			if (pMario == nullptr || pLuigi == nullptr)
//				continue;
//
//			//LogGameDebug(GCPhysic::CheckBox2DvsBox2D(*pMario->GetComponent<BoxCollider>(), *pLuigi->GetComponent<BoxCollider>()) ? "Collision\n\n\n\n" : "No collision\n\n\n\n");
//			//LogGameDebug(GCPhysic::CheckCirclevsCircle(*pMario->GetComponent<CircleCollider>(), *pLuigi->GetComponent<CircleCollider>()) ? "Collision\n\n\n\n" : "No collision\n\n\n\n");
//			LogGameDebug(GCPhysic::CheckBox2DvsCircle(*pMario->GetComponent<BoxCollider>(), *pLuigi->GetComponent<CircleCollider>()) ? "Collision\n\n\n\n" : "No collision\n\n\n\n");
//		}
//
//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
//		{
//			if (pMario == nullptr)
//			{
//				pMario = pTest1Scene->CreateGameObject();
//				pMario->SetName("Mario");
//				pMario->AddTag("Bros Brother");
//
//				pMario->AddComponent<BoxCollider>();
//			}
//			pSelected = pMario;
//		}
//
//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
//		{
//			if (pLuigi == nullptr)
//			{
//				pLuigi = pTest1Scene->CreateGameObject();
//				pLuigi->SetName("Luigi");
//				pLuigi->AddTag("Bros Brother");
//
//				pLuigi->AddComponent<BoxCollider>();
//			}
//			pSelected = pLuigi;
//		}
//
//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
//		{
//			if (pGoomba == nullptr)
//			{
//				pGoomba = pTest1Scene->CreateGameObject();
//				pGoomba->SetName("Goomba");
//				pGoomba->AddTag("Enemy");
//			}
//			pSelected = pGoomba;
//		}
//
//
//
//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) pSelected->m_transform.m_position += GCVEC3::Up() * 0.03f;
//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) pSelected->m_transform.m_position += GCVEC3::Down() * 0.03f;
//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) pSelected->m_transform.m_position += GCVEC3::Left() * 0.03f;
//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) pSelected->m_transform.m_position += GCVEC3::Right() * 0.03f;
//
//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) pSelected->m_transform.Scale(GCVEC3::One() * 1.01f);
//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) pSelected->m_transform.Scale(GCVEC3::One() * 0.99f);
//
//		marioBox.setPosition(sf::Vector2f(pMario->m_transform.m_position.x, pMario->m_transform.m_position.y));
//		luigiCircle.setPosition(sf::Vector2f(pLuigi->m_transform.m_position.x, pLuigi->m_transform.m_position.y));
//		//goombaBox.setPosition(sf::Vector2f(pGoomba->m_transform.m_position.x, pGoomba->m_transform.m_position.y));
//
//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
//		{
//			pSelected->m_transform.Rotate(0, 0, 0.2f * (PI / 100.f));
//			angle += 0.2f;
//			std::cout << "Rotation A: {" << pSelected->m_transform.m_rotation.x << " ; " << pSelected->m_transform.m_rotation.y << " ; " << pSelected->m_transform.m_rotation.z << "}" << std::endl;
//		}
//		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
//		{
//			pSelected->m_transform.Rotate(0, 0, -0.2f * (PI / 100.f));
//			angle -= 0.2f;
//		}
//
//
//
//
//		if (pMario != nullptr)
//		{
//			//std::cout << "{" << (int)pMario->m_transform.m_rotation.x << " ; " << (int)pMario->m_transform.m_rotation.y << " ; " << (int)pMario->m_transform.m_rotation.z << "}" << std::endl;
//			marioSprite.setPosition(sf::Vector2f(pMario->m_transform.m_position.x, pMario->m_transform.m_position.y));
//			marioSprite.setScale(sf::Vector2f(pMario->m_transform.m_scale.x, pMario->m_transform.m_scale.y));
//
//			marioSprite.setRotation(angle);
//			window.draw(marioSprite);
//			window.draw(marioBox);
//			//window.draw(marioCircle);
//		}
//
//		if (pLuigi != nullptr)
//		{
//			luigiSprite.setPosition(sf::Vector2f(pLuigi->m_transform.m_position.x, -pLuigi->m_transform.m_position.y));
//			luigiSprite.setScale(sf::Vector2f(pLuigi->m_transform.m_scale.x, pLuigi->m_transform.m_scale.y));
//			window.draw(luigiSprite);
//			//window.draw(luigiBox);
//			window.draw(luigiCircle);
//		}
//
//		if (pGoomba != nullptr)
//		{
//			goombaSprite.setPosition(sf::Vector2f(pGoomba->m_transform.m_position.x, -pGoomba->m_transform.m_position.y));
//			goombaSprite.setScale(sf::Vector2f(pGoomba->m_transform.m_scale.x, pGoomba->m_transform.m_scale.y));
//			window.draw(goombaSprite);
//			window.draw(goombaBox);
//		}
//
//
//
//
//		GC::m_pActiveGameManager.Update();
//		window.display();
//	}
//
//	return EXIT_SUCCESS;
//
//	/*LogEngineDebug("Engine Debug");
//	LogEngineError("Engine Error");
//	LogEngineWarn("Engine Warn");
//	LogEngineInfo("Engine Info");
//	LogEngineTrace("Engine Trace");
//	LogGameDebug("Game Debug");*/
//}