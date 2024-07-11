
//
//

//
//#include <iostream>
//// #include <Windows.h>
//// #include "Window.h"
//// #include "EventSystem.h"
//// #include "Log.h"
//#include "Vectors.h"
//#include "GCColor.h"
//#include "Quaternion.h"
//#include "Matrix.h"
//#include "Transform.h"
//#include "Components.h"
#include "GameObject.h"
#include "Scene.h"
//#include "SceneManager.h"
//#include "GameManager.h"
#include "GC.h"
//#include "RenderManager.h"
#include "Components.h"
//#include "InputManager.h"
////#include "SFML/Graphics.hpp"
//
//#include "PhysicManager.h"
//#include "Log.h"
//
//
//void CreateConsole()
//{
//    AllocConsole();
//    FILE* fp;
//    freopen_s(&fp, "CONOUT$", "w", stdout);
//    freopen_s(&fp, "CONOUT$", "w", stderr);
//    freopen_s(&fp, "CONIN$", "r", stdin);
//}

#include "pch.h"

#include "../../src/Render/pch.h"

using namespace DirectX;

// Définition des variables globales pour la caméra
XMVECTOR cameraPosition = XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f);
XMVECTOR cameraTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
XMVECTOR cameraUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

float cameraMoveSpeed = 0.05f; // Vitesse de déplacement de la caméra

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    // Initialisation des ressources graphiques

    GC::m_pActiveGameManager.Init();

    GCGraphicsLogger& profiler = GCGraphicsLogger::GetInstance();
    profiler.InitializeConsole();

    Window* window = new Window(hInstance);
    window->Initialize();

    GC::m_pActiveGameManager.m_pRenderManager.m_pGraphics->Initialize(window, 1920, 1080);


    int flagsLightColor = 0;
    SET_FLAG(flagsLightColor, HAS_POSITION);
    SET_FLAG(flagsLightColor, HAS_COLOR);
    SET_FLAG(flagsLightColor, HAS_NORMAL);

    int flagsLightTexture = 0;
    SET_FLAG(flagsLightTexture, HAS_POSITION);
    SET_FLAG(flagsLightTexture, HAS_UV);
    SET_FLAG(flagsLightTexture, HAS_NORMAL);

    GCScene* pScene = GCScene::Create();
    GCGameObject* test1 = pScene->CreateGameObject();
    GCGameObject* test2 = pScene->CreateGameObject();
    GCGameObject* test3 = pScene->CreateGameObject();


    test1->AddComponent<BoxCollider>();
    test2->AddComponent<SpriteRenderer>();
    test3->AddComponent<CircleCollider>();

    test1->m_transform.m_position.x = 100;
    test1->m_transform.m_position.y = 100;

    test2->m_transform.m_position.x = 200;
    test2->m_transform.m_position.y = 200;

    test3->m_transform.m_position.x = 500;
    test3->m_transform.m_position.y = 200;

    float viewWidth = 20.0f;
    float viewHeight = 20.0f;
    XMMATRIX projectionMatrix = XMMatrixOrthographicLH(viewWidth, viewHeight, 1.0f, 1000.0f);
    XMMATRIX viewMatrix = XMMatrixLookAtLH(cameraPosition, cameraTarget, cameraUp);
    XMMATRIX transposedProjectionMatrix = XMMatrixTranspose(projectionMatrix);
    XMMATRIX transposedViewMatrix = XMMatrixTranspose(viewMatrix);
    XMFLOAT4X4 storedProjectionMatrix;
    XMFLOAT4X4 storedViewMatrix;
    XMStoreFloat4x4(&storedProjectionMatrix, transposedProjectionMatrix);
    XMStoreFloat4x4(&storedViewMatrix, transposedViewMatrix);

    XMMATRIX worldMatrixCubeOuter = XMMatrixScaling(20.0f, 20.0f, 20.0f) * XMMatrixTranslation(0.0f, -3.0f, 0.0f); // Cube externe (skybox)
    XMMATRIX worldMatrixCubeInner = XMMatrixScaling(20.0f, 20.0f, 20.0f) * XMMatrixTranslation(-0.0f, 0.0f, -1.0f); // Cube interne centré
    XMMATRIX worldMatrixCubeInner2 = XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixTranslation(4.0f, 2.0f, -1.0f); // Cube interne centré
    XMMATRIX worldMatrixCubeInner3 = XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixTranslation(2.0f, -2.0f, -1.0f); // Cube interne centré
    XMMATRIX worldMatrixSphere = XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixTranslation(3.0f, 5.0f, -2.0f); // Sphère déplacée dans le cube interne

    XMFLOAT4X4 worldCubeOuter;
    XMFLOAT4X4 worldCubeInner;
    XMFLOAT4X4 worldCubeInner2;
    XMFLOAT4X4 worldCubeInner3;
    XMFLOAT4X4 worldSphere;

    XMStoreFloat4x4(&worldCubeOuter, XMMatrixTranspose(worldMatrixCubeOuter));
    XMStoreFloat4x4(&worldCubeInner, XMMatrixTranspose(worldMatrixCubeInner));
    XMStoreFloat4x4(&worldCubeInner2, XMMatrixTranspose(worldMatrixCubeInner2));
    XMStoreFloat4x4(&worldCubeInner3, XMMatrixTranspose(worldMatrixCubeInner3));
    XMStoreFloat4x4(&worldSphere, XMMatrixTranspose(worldMatrixSphere));

    auto startTime = std::chrono::steady_clock::now();

    while (true) {
        auto currentTime = std::chrono::steady_clock::now();
        float elapsedTime = std::chrono::duration<float>(currentTime - startTime).count();

        float rotationSpeed = 1.0f;
        float angle = rotationSpeed * elapsedTime;
        XMMATRIX rotationMatrix = XMMatrixRotationY(angle);

        // Mettre à jour la matrice de transformation du cube interne
        //XMMATRIX worldMatrixCubeInnerUpdated = rotationMatrix * worldMatrixCubeInner;

        //// Extraire les données de la matrice mise à jour dans une XMFLOAT4X4
        //XMStoreFloat4x4(&worldCubeInner, XMMatrixTranspose(worldMatrixCubeInnerUpdated));


        graphics->StartFrame();
        graphics->UpdateViewProjConstantBuffer(storedProjectionMatrix, storedViewMatrix);

        //// Dessiner le cube externe
        //graphics->UpdateWorldConstantBuffer(materialCubeOuter.resource, worldCubeOuter);
        //graphics->GetRender()->DrawObject(meshCubeOuter.resource, materialCubeOuter.resource);


        GCLIGHTSPROPERTIES lightData = {};

        GCLIGHT light2;
        light2.position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f); // Position en 2D (x, y, 0)
        light2.direction = DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f); // Direction vers le bas en 2D
        light2.color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f); // Couleur de la lumière
        light2.spotAngle = 10.0f; // Angle du spot si applicable
        light2.lightIntensity = 3.2f;
        light2.lightType = 1; // Type de lumière ponctuelle

        GCLIGHT pointLight;
        pointLight.position = DirectX::XMFLOAT3(6.0f, 1.0f, 0.0f); // Position en 2D (x, y, 0)
        pointLight.direction = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f); // Direction vers le bas en 2D
        pointLight.color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f); // Couleur de la lumière
        pointLight.spotAngle = 0.0f; // Angle du spot si applicable
        pointLight.lightIntensity = 2.4f;
        pointLight.lightType = 2; // Type de lumière ponctuelle


        lightData.lights[1] = pointLight;
        lightData.lights[0] = light2;

        graphics->UpdateLights(lightData);



        graphics->UpdateWorldConstantBuffer(materialCubeInner.resource, worldCubeInner2);
        graphics->GetRender()->DrawObject(meshCubeInner3.resource, materialCubeInner.resource);

        graphics->UpdateWorldConstantBuffer(materialCubeInner.resource, worldCubeInner3);
        graphics->GetRender()->DrawObject(meshCubeInner3.resource, materialCubeInner.resource);


        graphics->UpdateWorldConstantBuffer(materialCubeInner.resource, worldCubeInner);
        graphics->GetRender()->DrawObject(meshCubeInner.resource, materialCubeInner.resource);

        //// Dessiner la sphère interne
        //graphics->UpdateWorldConstantBuffer(materialSphere.resource, worldSphere);
        //graphics->GetRender()->DrawObject(meshSphere.resource, materialSphere.resource);

        //// Dessiner le deuxième cube interne (worldCubeInner2) avec la matrice mise à jour
        //graphics->UpdateWorldConstantBuffer(materialSphere.resource, worldCubeInner2);
        //graphics->GetRender()->DrawObject(meshSphere.resource, materialSphere.resource);

        graphics->EndFrame();
        window->Run(graphics->GetRender());
    }

    return 0;
}