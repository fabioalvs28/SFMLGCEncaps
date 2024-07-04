//#include "pch.h"
//#include <DirectXMath.h>
//#include <chrono> // Pour la gestion du temps
//
//// Assurez-vous d'inclure vos fichiers nécessaires pour la gestion de la fenêtre et des graphiques
//#include "Window.h" // Supposons que vous avez une classe Window pour la gestion de la fenêtre
//#include "GCGraphics.h" // Supposons que vous avez une classe GCGraphics pour la gestion des graphiques
//
//using namespace DirectX;
//
//// Définition des variables globales pour la caméra
//XMVECTOR cameraPosition = XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f);
//XMVECTOR cameraTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
//XMVECTOR cameraUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
//
//float cameraMoveSpeed = 0.05f; // Vitesse de déplacement de la caméra
//
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) {
//    // Initialisation des ressources graphiques
//    GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();
//    profiler.InitializeConsole();
//
//    Window* window = new Window(hInstance);
//    window->Initialize();
//
//    GCGraphics* graphics = new GCGraphics();
//    graphics->Initialize(window, 1920, 1080);
//
//    int flagsLightColor = 0;
//    SET_FLAG(flagsLightColor, HAS_POSITION);
//    SET_FLAG(flagsLightColor, HAS_COLOR);
//    SET_FLAG(flagsLightColor, HAS_NORMAL);
//
//    int flagsLightTexture = 0;
//    SET_FLAG(flagsLightTexture, HAS_POSITION);
//    SET_FLAG(flagsLightTexture, HAS_UV);
//    SET_FLAG(flagsLightTexture, HAS_NORMAL);
//
//    // Création des géométries
//    auto geoCubeOuter = graphics->CreateGeometryPrimitiveCustom("cubeSkybox", XMFLOAT4(Colors::Red), flagsLightColor);
//    auto geoCubeInner = graphics->CreateGeometryPrimitiveCustom("plane", XMFLOAT4(Colors::Green), flagsLightColor);
//    auto geoCubeInner3 = graphics->CreateGeometryPrimitiveCustom("plane", XMFLOAT4(Colors::Red), flagsLightColor);
//    auto geoSphere = graphics->CreateGeometryPrimitiveCustom("sphere", XMFLOAT4(Colors::Yellow), flagsLightTexture);
//
//    // Chargement des shaders personnalisés
//    std::string shaderFilePath1 = "../../../src/Render/Shaders/LightColor.hlsl";
//    std::string csoDestinationPath1 = "../../../src/Render/CsoCompiled/LightColor";
//    auto shaderLightColor = graphics->CreateShaderCustom(shaderFilePath1, csoDestinationPath1, flagsLightColor, D3D12_CULL_MODE_BACK);
//
//    std::string shaderFilePath2 = "../../../src/Render/Shaders/LightTexture.hlsl";
//    std::string csoDestinationPath2 = "../../../src/Render/CsoCompiled/LightTexture";
//    auto shaderLightTexture = graphics->CreateShaderCustom(shaderFilePath2, csoDestinationPath2, flagsLightTexture, D3D12_CULL_MODE_BACK);
//
//    auto shaderLightSkyBox = graphics->CreateShaderCustom(shaderFilePath1, csoDestinationPath1, flagsLightColor, D3D12_CULL_MODE_NONE);
//
//    graphics->InitializeGraphicsResourcesStart();
//
//    // Création des meshes
//    auto meshCubeOuter = graphics->CreateMesh(geoCubeOuter.resource);
//    auto meshCubeInner = graphics->CreateMesh(geoCubeInner.resource);
//    auto meshCubeInner3 = graphics->CreateMesh(geoCubeInner3.resource);
//    auto meshSphere = graphics->CreateMesh(geoSphere.resource);
//
//    std::string texturePath = "../../../src/Render/Textures/texture.dds";
//    std::string texturePath2 = "../../../src/Render/Textures/cottage_diffuse.dds";
//    auto texture = graphics->CreateTexture(texturePath);
//    auto texture2 = graphics->CreateTexture(texturePath2);
//
//    graphics->InitializeGraphicsResourcesEnd();
//
//    // Création des matériaux
//    auto materialCubeOuter = graphics->CreateMaterial(shaderLightSkyBox.resource);
//    //materialCubeOuter.resource->SetTexture(texture2.resource);
//
//    auto materialCubeInner = graphics->CreateMaterial(shaderLightColor.resource);
//    auto materialSphere = graphics->CreateMaterial(shaderLightTexture.resource);
//    materialSphere.resource->SetTexture(texture.resource);
//
//    // Initialisation des matrices de vue et de projection
//    float viewWidth = 20.0f;
//    float viewHeight = 20.0f;
//    XMMATRIX projectionMatrix = XMMatrixOrthographicLH(viewWidth, viewHeight, 1.0f, 1000.0f);
//    XMMATRIX viewMatrix = XMMatrixLookAtLH(cameraPosition, cameraTarget, cameraUp);
//    XMMATRIX transposedProjectionMatrix = XMMatrixTranspose(projectionMatrix);
//    XMMATRIX transposedViewMatrix = XMMatrixTranspose(viewMatrix);
//    XMFLOAT4X4 storedProjectionMatrix;
//    XMFLOAT4X4 storedViewMatrix;
//    XMStoreFloat4x4(&storedProjectionMatrix, transposedProjectionMatrix);
//    XMStoreFloat4x4(&storedViewMatrix, transposedViewMatrix);
//
//    // Définition des matrices de transformation pour chaque objet avec translation
//    XMMATRIX worldMatrixCubeOuter = XMMatrixScaling(20.0f, 20.0f, 20.0f) * XMMatrixTranslation(0.0f, -3.0f, 0.0f); // Cube externe (skybox)
//    XMMATRIX worldMatrixCubeInner = XMMatrixScaling(20.0f, 20.0f, 20.0f) * XMMatrixTranslation(-0.0f, 0.0f, -1.0f); // Cube interne centré
//    XMMATRIX worldMatrixCubeInner2 = XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixTranslation(4.0f, 2.0f, -1.0f); // Cube interne centré
//    XMMATRIX worldMatrixCubeInner3 = XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixTranslation(2.0f, -2.0f, -1.0f); // Cube interne centré
//    XMMATRIX worldMatrixSphere = XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixTranslation(3.0f, 5.0f, -2.0f); // Sphère déplacée dans le cube interne
//
//    XMFLOAT4X4 worldCubeOuter;
//    XMFLOAT4X4 worldCubeInner;
//    XMFLOAT4X4 worldCubeInner2;
//    XMFLOAT4X4 worldCubeInner3;
//    XMFLOAT4X4 worldSphere;
//
//    XMStoreFloat4x4(&worldCubeOuter, XMMatrixTranspose(worldMatrixCubeOuter));
//    XMStoreFloat4x4(&worldCubeInner, XMMatrixTranspose(worldMatrixCubeInner));
//    XMStoreFloat4x4(&worldCubeInner2, XMMatrixTranspose(worldMatrixCubeInner2));
//    XMStoreFloat4x4(&worldCubeInner3, XMMatrixTranspose(worldMatrixCubeInner3));
//    XMStoreFloat4x4(&worldSphere, XMMatrixTranspose(worldMatrixSphere));
//
//    auto startTime = std::chrono::steady_clock::now();
//
//    // Boucle de rendu
//    while (true) {
//        // Mesurer le temps écoulé depuis le début de l'application
//        auto currentTime = std::chrono::steady_clock::now();
//        float elapsedTime = std::chrono::duration<float>(currentTime - startTime).count();
//
//        // Calculer la rotation du cube interne autour de l'axe Y
//        float rotationSpeed = 1.0f; // Vitesse de rotation en radians par seconde
//        float angle = rotationSpeed * elapsedTime;
//        XMMATRIX rotationMatrix = XMMatrixRotationY(angle);
//
//        // Mettre à jour la matrice de transformation du cube interne
//        //XMMATRIX worldMatrixCubeInnerUpdated = rotationMatrix * worldMatrixCubeInner;
//
//        //// Extraire les données de la matrice mise à jour dans une XMFLOAT4X4
//        //XMStoreFloat4x4(&worldCubeInner, XMMatrixTranspose(worldMatrixCubeInnerUpdated));
//
//        // Gestion des entrées utilisateur pour le déplacement de la caméra
//        if (window->IsKeyDown('Z')) {
//            cameraPosition += cameraMoveSpeed * XMVector3Normalize(XMVectorSubtract(cameraTarget, cameraPosition));
//        }
//        if (window->IsKeyDown('S')) {
//            cameraPosition -= cameraMoveSpeed * XMVector3Normalize(XMVectorSubtract(cameraTarget, cameraPosition));
//        }
//
//        // Mettre à jour la matrice de vue avec la nouvelle position de la caméra
//        viewMatrix = XMMatrixLookAtLH(cameraPosition, cameraTarget, cameraUp);
//        transposedViewMatrix = XMMatrixTranspose(viewMatrix);
//        XMStoreFloat4x4(&storedViewMatrix, transposedViewMatrix);
//
//        // Rendu des objets
//        graphics->StartFrame();
//        graphics->UpdateViewProjConstantBuffer(storedProjectionMatrix, storedViewMatrix);
//
//        //// Dessiner le cube externe
//        //graphics->UpdateWorldConstantBuffer(materialCubeOuter.resource, worldCubeOuter);
//        //graphics->GetRender()->DrawObject(meshCubeOuter.resource, materialCubeOuter.resource);
//
//        // Dessiner le cube interne avec la matrice mise à jour
//
//
//
//        graphics->UpdateWorldConstantBuffer(materialCubeInner.resource, worldCubeInner2);
//        graphics->GetRender()->DrawObject(meshCubeInner3.resource, materialCubeInner.resource);
//
//
//
//
//        graphics->UpdateWorldConstantBuffer(materialCubeInner.resource, worldCubeInner3);
//        graphics->GetRender()->DrawObject(meshCubeInner3.resource, materialCubeInner.resource);
//
//
//        graphics->UpdateWorldConstantBuffer(materialCubeInner.resource, worldCubeInner);
//        graphics->GetRender()->DrawObject(meshCubeInner.resource, materialCubeInner.resource);
//
//
//
//
//        
//
//        //// Dessiner la sphère interne
//        //graphics->UpdateWorldConstantBuffer(materialSphere.resource, worldSphere);
//        //graphics->GetRender()->DrawObject(meshSphere.resource, materialSphere.resource);
//
//        //// Dessiner le deuxième cube interne (worldCubeInner2) avec la matrice mise à jour
//        //graphics->UpdateWorldConstantBuffer(materialSphere.resource, worldCubeInner2);
//        //graphics->GetRender()->DrawObject(meshSphere.resource, materialSphere.resource);
//
//        graphics->EndFrame();
//        window->Run(graphics->GetRender());
//    }
//
//    return 0;
//}

#include "pch.h"
#include <DirectXMath.h>
#include <chrono> // Pour la gestion du temps

// Assurez-vous d'inclure vos fichiers n�cessaires pour la gestion de la fen�tre et des graphiques
#include "Window.h" // Supposons que vous avez une classe Window pour la gestion de la fen�tre
#include "GCGraphics.h" // Supposons que vous avez une classe GCGraphics pour la gestion des graphiques

using namespace DirectX;

// D�finition des variables globales pour la cam�ra
XMVECTOR cameraPosition = XMVectorSet(0.0f, -10.0f, 5.0f, 1.0f);
XMVECTOR cameraTarget = XMVectorZero();
XMVECTOR cameraUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

float cameraMoveSpeed = 0.05f; // Vitesse de d�placement de la cam�ra

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) {
    // Initialisation des ressources graphiques
    GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();
    profiler.InitializeConsole();

    Window* window = new Window(hInstance);
    window->Initialize();

    GCGraphics* graphics = new GCGraphics();
    graphics->Initialize(window, 1920, 1080);

    int flagsLightColor = 0;
    SET_FLAG(flagsLightColor, HAS_POSITION);
    SET_FLAG(flagsLightColor, HAS_COLOR);
    SET_FLAG(flagsLightColor, HAS_NORMAL);

    int flagsLightTexture = 0;
    SET_FLAG(flagsLightTexture, HAS_POSITION);
    SET_FLAG(flagsLightTexture, HAS_UV);
    SET_FLAG(flagsLightTexture, HAS_NORMAL);

    int flagsColor = 0;
    SET_FLAG(flagsColor, HAS_POSITION);
    SET_FLAG(flagsColor, HAS_COLOR);

    // Cr�ation des g�om�tries
    auto geoCubeOuter = graphics->CreateGeometryPrimitive(CubeSkybox, XMFLOAT4(Colors::Red));
    auto geoCubeInner = graphics->CreateGeometryPrimitive(Cube, XMFLOAT4(Colors::Green));
    auto geoSphere = graphics->CreateGeometryPrimitive(Sphere, XMFLOAT4(Colors::Yellow));
    
    // Chargement des shaders personnalis�s
    std::string shaderFilePath1 = "../../../src/Render/Shaders/LightColor.hlsl";
    std::string csoDestinationPath1 = "../../../src/Render/CsoCompiled/LightColor";
    auto shaderLightColor = graphics->CreateShaderCustom(shaderFilePath1, csoDestinationPath1, flagsLightColor, D3D12_CULL_MODE_BACK);

    std::string shaderFilePath2 = "../../../src/Render/Shaders/LightTexture.hlsl";
    std::string csoDestinationPath2 = "../../../src/Render/CsoCompiled/LightTexture";
    auto shaderLightTexture = graphics->CreateShaderCustom(shaderFilePath2, csoDestinationPath2, flagsLightTexture, D3D12_CULL_MODE_BACK);

    std::string shaderFilePath3 = "../../../src/Render/Shaders/LightColor.hlsl";
    std::string csoDestinationPath3 = "../../../src/Render/CsoCompiled/LightColor";
    auto shaderParticle = graphics->CreateShaderCustom(shaderFilePath3, csoDestinationPath3, flagsColor, D3D12_CULL_MODE_NONE);

    auto shaderLightSkyBox = graphics->CreateShaderCustom(shaderFilePath1, csoDestinationPath1, flagsLightColor, D3D12_CULL_MODE_NONE);

    graphics->InitializeGraphicsResourcesStart();

    // Cr�ation des meshes
    auto meshCubeOuter = graphics->CreateMeshCustom(geoCubeOuter.resource, flagsLightColor);
    auto meshCubeInner = graphics->CreateMeshCustom(geoCubeInner.resource, flagsLightColor);
    auto meshSphere = graphics->CreateMeshCustom(geoSphere.resource, flagsLightTexture);
    
    meshSphere.resource->InitializeParticleSystem(10);

    std::string texturePath = "../../../src/Render/Textures/texture.dds";
    std::string texturePath2 = "../../../src/Render/Textures/cottage_diffuse.dds";
    auto texture = graphics->CreateTexture(texturePath);
    auto texture2 = graphics->CreateTexture(texturePath2);

    graphics->InitializeGraphicsResourcesEnd();

    // Cr�ation des mat�riaux
    auto materialCubeOuter = graphics->CreateMaterial(shaderLightSkyBox.resource);
    //materialCubeOuter.resource->SetTexture(texture2.resource);

    auto materialCubeInner = graphics->CreateMaterial(shaderLightColor.resource);
    auto materialSphere = graphics->CreateMaterial(shaderLightTexture.resource);
    materialSphere.resource->SetTexture(texture.resource);

    // Initialisation des matrices de vue et de projection
    XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(0.25f * XM_PI, window->AspectRatio(), 1.0f, 1000.0f);
    XMMATRIX viewMatrix = XMMatrixLookAtLH(cameraPosition, cameraTarget, cameraUp);
    XMMATRIX transposedProjectionMatrix = XMMatrixTranspose(projectionMatrix);
    XMMATRIX transposedViewMatrix = XMMatrixTranspose(viewMatrix);
    XMFLOAT4X4 storedProjectionMatrix;
    XMFLOAT4X4 storedViewMatrix;
    XMStoreFloat4x4(&storedProjectionMatrix, transposedProjectionMatrix);
    XMStoreFloat4x4(&storedViewMatrix, transposedViewMatrix);

    // D�finition des matrices de transformation pour chaque objet avec translation
    XMMATRIX worldMatrixCubeOuter = XMMatrixScaling(20.0f, 20.0f, 20.0f) * XMMatrixTranslation(0.0f, -3.0f, 0.0f); // Cube externe (skybox)
    XMMATRIX worldMatrixCubeInner = XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixTranslation(-4.0f, 5.0f, -2.0f); // Cube interne centr�
    XMMATRIX worldMatrixCubeInner2 = XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixTranslation(-6.0f, 5.0f, -2.0f); // Cube interne centr�
    XMMATRIX worldMatrixSphere = XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixTranslation(3.0f, 5.0f, -2.0f); // Sph�re d�plac�e dans le cube interne

    XMFLOAT4X4 worldCubeOuter;
    XMFLOAT4X4 worldCubeInner;
    XMFLOAT4X4 worldCubeInner2;
    XMFLOAT4X4 worldSphere;

    XMStoreFloat4x4(&worldCubeOuter, XMMatrixTranspose(worldMatrixCubeOuter));
    XMStoreFloat4x4(&worldCubeInner, XMMatrixTranspose(worldMatrixCubeInner));
    XMStoreFloat4x4(&worldCubeInner2, XMMatrixTranspose(worldMatrixCubeInner2));
    XMStoreFloat4x4(&worldSphere, XMMatrixTranspose(worldMatrixSphere));

    auto startTime = std::chrono::steady_clock::now();
    auto lastFrameTime = startTime;

    // Boucle de rendu
    while (true) {
        // Mesurer le temps �coul� depuis le d�but de l'application
        auto currentTime = std::chrono::steady_clock::now();
        float elapsedTime = std::chrono::duration<float>(currentTime - startTime).count();
        float deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime).count();
        lastFrameTime = currentTime;

        // Calculer la rotation du cube interne autour de l'axe Y
        float rotationSpeed = 1.0f; // Vitesse de rotation en radians par seconde
        float angle = rotationSpeed * elapsedTime;
        XMMATRIX rotationMatrix = XMMatrixRotationY(angle);

        // Mettre � jour la matrice de transformation du cube interne
        XMMATRIX worldMatrixCubeInnerUpdated = rotationMatrix * worldMatrixCubeInner;

        // Extraire les donn�es de la matrice mise � jour dans une XMFLOAT4X4
        XMStoreFloat4x4(&worldCubeInner, XMMatrixTranspose(worldMatrixCubeInnerUpdated));

         //Gestion des entr�es utilisateur pour le d�placement de la cam�ra
        if (window->IsKeyDown('Z')) {
            cameraPosition += cameraMoveSpeed * XMVector3Normalize(XMVectorSubtract(cameraTarget, cameraPosition));
        }
        if (window->IsKeyDown('S')) {
            cameraPosition -= cameraMoveSpeed * XMVector3Normalize(XMVectorSubtract(cameraTarget, cameraPosition));
        }

        // Mettre � jour la matrice de vue avec la nouvelle position de la cam�ra
        viewMatrix = XMMatrixLookAtLH(cameraPosition, cameraTarget, cameraUp);
        transposedViewMatrix = XMMatrixTranspose(viewMatrix);
        XMStoreFloat4x4(&storedViewMatrix, transposedViewMatrix);

        GCMATERIALPROPERTIES materialProperties;
        materialProperties.ambientLightColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        materialProperties.ambient = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
        materialProperties.diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        materialProperties.specular = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
        materialProperties.shininess = 1.0f;

        graphics->UpdateMaterialProperties(materialCubeOuter.resource, materialProperties);

        GCLIGHTSPROPERTIES lightData = {};

        GCLIGHT directionalLight;
        directionalLight.position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f); 
        directionalLight.direction = DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f);
        directionalLight.color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f); 
        directionalLight.spotAngle = 0.0f; 
        directionalLight.lightIntensity = 1.3f;
        directionalLight.lightType = 0; 

        //GCLIGHT light1;
        //light1.position = DirectX::XMFLOAT3(-5.0f, 20.0f, -2.0f);
        //light1.direction = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
        //light1.color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
        //light1.spotAngle = 10.0f;
        //light1.lightIntensity = 1.2f;
        //light1.lightType = 1;

        //GCLIGHT light2;
        //light2.position = DirectX::XMFLOAT3(2.0f, 20.0f, -2.0f);
        //light2.direction = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
        //light2.color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
        //light2.spotAngle = 10.0f;
        //light2.lightIntensity = 1.2f;
        //light2.lightType = 1;

        GCLIGHT pointLight;
        pointLight.position = DirectX::XMFLOAT3(0.0f, 4.0f, 0.0f); // Position en 2D (x, y, 0)
        pointLight.direction = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
        pointLight.color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f); // Couleur de la lumière
        pointLight.spotAngle = 0.0f; // Angle du spot si applicable
        pointLight.lightIntensity = 20.4f;
        pointLight.lightType = 2; // Type de lumière ponctuelle

        //lightData.lights[0] = directionalLight;
        //lightData.lights[1] = light2;
        //lightData.lights[2] = light1;
        lightData.lights[1] = pointLight;

        graphics->UpdateLights(lightData);

        graphics->StartFrame();
        graphics->UpdateViewProjConstantBuffer(storedProjectionMatrix, storedViewMatrix);

        //graphics->UpdateWorldConstantBuffer(materialCubeOuter.resource, worldCubeOuter);
        //graphics->GetRender()->DrawObject(meshCubeOuter.resource, materialCubeOuter.resource);

        graphics->UpdateWorldConstantBuffer(materialCubeInner.resource, worldCubeInner);
        graphics->GetRender()->DrawObject(meshCubeInner.resource, materialCubeInner.resource);

        meshSphere.resource->UpdateParticles(deltaTime);
        meshSphere.resource->RenderParticles(graphics, materialSphere.resource, worldMatrixSphere);

        graphics->UpdateWorldConstantBuffer(materialSphere.resource, worldSphere);
        graphics->GetRender()->DrawObject(meshSphere.resource, materialSphere.resource);


        //graphics->UpdateWorldConstantBuffer(materialSphere.resource, worldCubeInner2);
        //graphics->GetRender()->DrawObject(meshSphere.resource, materialSphere.resource);

        graphics->EndFrame();
        window->Run(graphics->GetRender());
    }

    return 0;
}