


#include "pch.h"


#include <iostream>
#include <Windows.h>
#include "ECSTesting.h"
#include "../engine/Window.h"
#include "../engine/EventSystem.h"
#include "../Engine/GameManager.h"
#include "../Engine/SceneManager.h"
#include "../Engine/InputManager.h"
#include "../Engine/Scene.h"
#include "../Engine/GameObject.h"
#include "../Engine/Components.h"
#include "../Engine/GCColor.h"


#include "../../src/render/framework.h"




void CreateConsole()
{
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	Window* window = new Window(hInstance);
	window->Initialize();

	GCGraphics* graphics = new GCGraphics();
	graphics->Initialize(window);

	graphics->GetPrimitiveFactory()->Initialize();
	graphics->GetModelParserFactory()->Initialize();

	// Geometry (Resource)
	GCGeometry* geo = graphics->GetPrimitiveFactory()->BuildGeometryColor(L"plane", DirectX::XMFLOAT4(DirectX::Colors::Red));
	GCGeometry* geo1 = graphics->GetModelParserFactory()->BuildObjTexture("../../../src/Render/monkeyUv.obj");
	GCShader* shader1 = graphics->CreateShaderColor();
	GCShader* shader2 = graphics->CreateShaderTexture();

	///// Create Render Resources
	graphics->GetRender()->ResetCommandList(); // Reset Command List Before Resources Creation


	shader1->Load<GCWORLDCB>();
	shader2->Load<GCWORLDCB>();


	// Mesh
	GCMesh* mesh = graphics->CreateMesh(geo);
	GCMesh* mesh1 = graphics->CreateMesh(geo1);
	//GCShader* shaderCustom = graphics->CreateShaderCustom(customShaderFile);


	std::string texturePath = "../../../src/Render/Textures/texture.dds";
	GCTexture* tex1 = graphics->CreateTexture(texturePath);

	graphics->GetRender()->CloseCommandList(); // Close and Execute after creation
	graphics->GetRender()->ExecuteCommandList();// Close and Execute after creation

	///// 

	// SET CAMERA 

	DirectX::XMVECTOR cameraPosition = DirectX::XMVectorSet(0.0f, -10.0f, 5.0f, 1.0f);
	DirectX::XMVECTOR targetPosition = DirectX::XMVectorZero();
	DirectX::XMVECTOR upVector = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);


	DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, window->AspectRatio(), 1.0f, 1000.0f);
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(cameraPosition, targetPosition, upVector);

	// Transposez les matrices
	DirectX::XMMATRIX transposedProjectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);
	DirectX::XMMATRIX transposedViewMatrix = DirectX::XMMatrixTranspose(viewMatrix);

	// Stockez les matrices transpos�es dans des XMFLOAT4X4
	DirectX::XMFLOAT4X4 storedProjectionMatrix;
	DirectX::XMFLOAT4X4 storedViewMatrix;

	DirectX::XMStoreFloat4x4(&storedProjectionMatrix, transposedProjectionMatrix);
	DirectX::XMStoreFloat4x4(&storedViewMatrix, transposedViewMatrix);

	// SET CAMERA


	DirectX::XMFLOAT4X4 I(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		3.0f, 0.0f, 0.0f, 1.0f);

	DirectX::XMFLOAT4X4 transposedWorld;
	DirectX::XMStoreFloat4x4(&transposedWorld, DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&I)));



	graphics->GetRender()->PrepareDraw();



	graphics->GetRender()->DrawOneObject(mesh1, shader2, tex1, MathHelper::Identity4x4(), storedProjectionMatrix, storedViewMatrix);
	graphics->GetRender()->DrawOneObject(mesh, shader1, nullptr, transposedWorld, storedProjectionMatrix, storedViewMatrix);


	graphics->GetRender()->PostDraw();

	//// Loop Again < |||| >

	//graphics->GetRender()->PrepareDraw();

	//graphics->GetRender()->PostDraw();

	// *


	//window->Run(graphics->GetRender());

    CreateConsole();
    ECSTesting::GetInstance().RunTests();
    std::cout << "Logging to console..." << std::endl;
    
    GCEventSystem* ev = new GCEventSystem();
    GCWindow *w = new GCWindow(hInstance, nCmdShow, *ev);
    w->Create(L"Testing", 800, 600);
    w->Show(nCmdShow);

	GCGameManager *GameManager = new GCGameManager();
    GameManager->Init();

	GCScene *myScene = GCSceneManager::CreateScene();
	myScene->SetActive(true);

    GCGameObject *myGoOne = myScene->CreateGameObject("go", true);
	GCGameObject *myGoTwo = myScene->CreateGameObject("go2", true);

	myGoOne->AddComponent<SpriteRenderer>();
	myGoTwo->AddComponent<SpriteRenderer>();

	GCColor color = GCColor(1.0f, 1.0f, 1.0f, 1.0f);
	myGoOne->GetComponent<SpriteRenderer>()->SetColor(color);
	myGoTwo->RemoveComponent<SpriteRenderer>();
	myGoOne->AddComponent<BoxCollider>();
    myGoOne->Destroy();

    while (w->IsRunning())
    {
        
		myGoTwo->m_transform.m_position.x += 0.01f;

		std::cout << "Position  " << myGoTwo->m_transform.m_position.x << std::endl;


	    GameManager->Update();
        w->PollEvents();
    }

    
    FreeConsole();
}