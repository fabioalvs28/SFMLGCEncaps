#include "pch.h"
#include <DirectXMath.h>

struct GCTest : GCSHADERCB {
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4 color;
};

using namespace DirectX;


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

    // Création des géométries
    auto geoCubeOuter = graphics->CreateGeometryPrimitiveCustom("cube", DirectX::XMFLOAT4(DirectX::Colors::Red), flagsLightColor);
    auto geoCubeInner = graphics->CreateGeometryPrimitiveCustom("cube", DirectX::XMFLOAT4(DirectX::Colors::Green), flagsLightColor);
    auto geoSphere = graphics->CreateGeometryPrimitiveCustom("sphere", DirectX::XMFLOAT4(DirectX::Colors::Yellow), flagsLightTexture);

    // Chargement des shaders personnalisés
    std::string shaderFilePath1 = "../../../src/Render/Shaders/LightColor.hlsl";
    std::string csoDestinationPath1 = "../../../src/Render/CsoCompiled/LightColor";
    auto shaderLightColor = graphics->CreateShaderCustom(shaderFilePath1, csoDestinationPath1, flagsLightColor, D3D12_CULL_MODE_BACK);

    std::string shaderFilePath2 = "../../../src/Render/Shaders/LightTexture.hlsl";
    std::string csoDestinationPath2 = "../../../src/Render/CsoCompiled/LightTexture";
    auto shaderLightTexture = graphics->CreateShaderCustom(shaderFilePath2, csoDestinationPath2, flagsLightTexture, D3D12_CULL_MODE_BACK);

    auto shaderLightSkyBox = graphics->CreateShaderCustom(shaderFilePath1, csoDestinationPath1, flagsLightColor, D3D12_CULL_MODE_FRONT);

    graphics->InitializeGraphicsResourcesStart();

    // Création des meshes
    auto meshCubeOuter = graphics->CreateMesh(geoCubeOuter.resource);
    auto meshCubeInner = graphics->CreateMesh(geoCubeInner.resource);
    auto meshSphere = graphics->CreateMesh(geoSphere.resource);

    std::string texturePath = "../../../src/Render/Textures/texture.dds";
    std::string texturePath2 = "../../../src/Render/Textures/cottage_diffuse.dds";
    auto texture = graphics->CreateTexture(texturePath);
    auto texture2 = graphics->CreateTexture(texturePath);

    graphics->InitializeGraphicsResourcesEnd();

    // Création des matériaux
    auto materialCubeOuter = graphics->CreateMaterial(shaderLightSkyBox.resource);
    //materialCubeOuter.resource->SetTexture(texture2.resource);

    auto materialCubeInner = graphics->CreateMaterial(shaderLightColor.resource);
    auto materialSphere = graphics->CreateMaterial(shaderLightTexture.resource);
    materialSphere.resource->SetTexture(texture2.resource);

    // Initialisation des matrices de vue et de projection
    DirectX::XMVECTOR cameraPosition = DirectX::XMVectorSet(0.0f, -10.0f, 5.0f, 1.0f);
    DirectX::XMVECTOR targetPosition = DirectX::XMVectorZero();
    DirectX::XMVECTOR upVector = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(0.25f * DirectX::XM_PI, window->AspectRatio(), 1.0f, 1000.0f);
    DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(cameraPosition, targetPosition, upVector);
    DirectX::XMMATRIX transposedProjectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);
    DirectX::XMMATRIX transposedViewMatrix = DirectX::XMMatrixTranspose(viewMatrix);
    DirectX::XMFLOAT4X4 storedProjectionMatrix;
    DirectX::XMFLOAT4X4 storedViewMatrix;
    DirectX::XMStoreFloat4x4(&storedProjectionMatrix, transposedProjectionMatrix);
    DirectX::XMStoreFloat4x4(&storedViewMatrix, transposedViewMatrix);

    // Définition des matrices de transformation pour chaque objet avec translation
    DirectX::XMMATRIX worldMatrixCubeOuter = DirectX::XMMatrixScaling(30.0f, 30.0f, 30.0f) * DirectX::XMMatrixTranslation(-8.0f, 0.0f, 2.0f); // Cube externe (skybox)
    DirectX::XMMATRIX worldMatrixCubeInner = DirectX::XMMatrixScaling(2.0f, 2.0f, 2.0f) * DirectX::XMMatrixTranslation(-4.0f, 0.0f, -2.0f); // Cube interne centré
    DirectX::XMMATRIX worldMatrixCubeInner2 = DirectX::XMMatrixScaling(2.0f, 2.0f, 2.0f) * DirectX::XMMatrixTranslation(-6.0f, 3.0f, -2.0f); // Cube interne centré
    DirectX::XMMATRIX worldMatrixSphere = DirectX::XMMatrixScaling(2.0f, 2.0f, 2.0f) * DirectX::XMMatrixTranslation(3.0f, 0.0f, -2.0f); // Sphère déplacée dans le cube interne

    DirectX::XMFLOAT4X4 worldCubeOuter;
    DirectX::XMFLOAT4X4 worldCubeInner;
    DirectX::XMFLOAT4X4 worldCubeInner2;
    DirectX::XMFLOAT4X4 worldSphere;

    DirectX::XMStoreFloat4x4(&worldCubeOuter, DirectX::XMMatrixTranspose(worldMatrixCubeOuter));
    DirectX::XMStoreFloat4x4(&worldCubeInner, DirectX::XMMatrixTranspose(worldMatrixCubeInner));
    DirectX::XMStoreFloat4x4(&worldCubeInner2, DirectX::XMMatrixTranspose(worldMatrixCubeInner2));
    DirectX::XMStoreFloat4x4(&worldSphere, DirectX::XMMatrixTranspose(worldMatrixSphere));

    auto startTime = std::chrono::steady_clock::now();

    // Boucle de rendu
    while (true) {
        // Mesurer le temps écoulé depuis le début de l'application
        auto currentTime = std::chrono::steady_clock::now();
        float elapsedTime = std::chrono::duration<float>(currentTime - startTime).count();

        // Calculer la rotation du cube interne autour de l'axe Y
        float rotationSpeed = 1.0f; // Vitesse de rotation en radians par seconde
        float angle = rotationSpeed * elapsedTime;
        DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationY(angle);

        // Mettre à jour la matrice de transformation du cube interne
        DirectX::XMMATRIX worldMatrixCubeInnerUpdated = rotationMatrix * worldMatrixCubeInner;

        // Extraire les données de la matrice mise à jour dans une XMFLOAT4X4
        DirectX::XMStoreFloat4x4(&worldCubeInner, DirectX::XMMatrixTranspose(worldMatrixCubeInnerUpdated));

        graphics->StartFrame();
        graphics->UpdateViewProjConstantBuffer(storedProjectionMatrix, storedViewMatrix);

        // Dessiner le cube externe
        graphics->UpdateWorldConstantBuffer(materialCubeOuter.resource, worldCubeOuter);
        graphics->GetRender()->DrawObject(meshCubeOuter.resource, materialCubeOuter.resource);

        // Dessiner le cube interne avec la matrice mise à jour
        graphics->UpdateWorldConstantBuffer(materialCubeInner.resource, worldCubeInner);
        graphics->GetRender()->DrawObject(meshCubeInner.resource, materialCubeInner.resource);

        // Dessiner la sphère interne
        graphics->UpdateWorldConstantBuffer(materialSphere.resource, worldSphere);
        graphics->GetRender()->DrawObject(meshSphere.resource, materialSphere.resource);

        graphics->UpdateWorldConstantBuffer(materialSphere.resource, worldCubeInner2);
        graphics->GetRender()->DrawObject(meshSphere.resource, materialSphere.resource);

        graphics->EndFrame();
        window->Run(graphics->GetRender());
    }

    return 0;
}