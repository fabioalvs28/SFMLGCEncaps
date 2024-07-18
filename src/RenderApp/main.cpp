//#include "../../src/render/pch.h"
//
//struct GCTest : GCSHADERCB {
//	DirectX::XMFLOAT4X4 world;
//	DirectX::XMFLOAT4 color;
//};
//
//
//
//
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
//{
//	GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();
//	profiler.InitializeConsole();
//
//	Window* window = new Window(hInstance);
//	window->Initialize();
//
//	GCGraphics* graphics = new GCGraphics();
//	graphics->Initialize(window, 1500, 800);
//
//	// Geometry (Resource)
//	auto geo = graphics->CreateGeometryPrimitiveTexture("plane");
//	auto geo1 = graphics->CreateGeometryPrimitiveTexture("cube");
//	auto geo2 = graphics->CreateGeometryModelParserTexture("../../../src/Render/monkeyUv.obj", obj);
//
//	std::string shaderFilePath = "../../../src/Render/Shaders/customTest.hlsl";
//	std::string csoDestinationPath = "../../../src/Render/CsoCompiled/custom";
//
//	auto shader1 = graphics->CreateShaderColor();
//	//GCShader* shader2 = graphics->CreateShaderCustom(shaderFilePath, csoDestinationPath, flagsTexture);
//
//
//	auto shader2 = graphics->CreateShaderTexture();
//
//	graphics->InitializeGraphicsResourcesStart();
//
//	// Mesh (Resource)
//	auto mesh = graphics->CreateMesh(geo.resource);
//
//
//	//GCMesh* mesh1 = graphics->CreateMesh(geo1);
//
//	auto mesh2 = graphics->CreateMesh(geo2.resource);
//
//	// Texture (Resource)
//	std::string texturePath = "../../../src/Render/Textures/texture.dds";
//	std::string texturePath2 = "../../../src/Render/Textures/ahah.dds";
//	auto texture8 = graphics->CreateTexture(texturePath2);
//	auto texture = graphics->CreateTexture(texturePath);
//	auto texture3 = graphics->CreateTexture(texturePath);
//
//
//
//
//	graphics->InitializeGraphicsResourcesEnd();
//
//
//
//
//	graphics->RemoveTexture(texture3.resource);
//	graphics->RemoveMesh(mesh.resource);
//
//
//
//
//	graphics->InitializeGraphicsResourcesStart();
//	graphics->InitializeGraphicsResourcesEnd();
//
//
//
//	// Material (Resource)
//	auto material = graphics->CreateMaterial(shader1.resource);
//	auto material2 = graphics->CreateMaterial(shader2.resource);
//	material2.resource->SetTexture(texture.resource);
//
//	auto material3 = graphics->CreateMaterial(shader2.resource);
//	material3.resource->SetTexture(texture8.resource);
//	ID3D12Resource* renderTargetTexture = graphics->GetRender()->CreateRTT();
//	shader2.resource->SetRenderTarget(renderTargetTexture);
//	graphics->GetRender()->DeleteRenderTarget(renderTargetTexture);
//	shader2.resource->SetRenderTarget(nullptr);
//	// PERSPECTIVE 
//
//	DirectX::XMVECTOR cameraPosition = DirectX::XMVectorSet(0.0f, -10.0f, 5.0f, 1.0f);
//	DirectX::XMVECTOR targetPosition = DirectX::XMVectorZero();
//	DirectX::XMVECTOR upVector = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
//
//
//	DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, window->AspectRatio(), 1.0f, 1000.0f);
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
//	// ***********
//
//
//	//// SET CAMERA
//	//	DirectX::XMVECTOR cameraPosition = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f);  // Position de la caméra
//	//	DirectX::XMVECTOR targetPosition = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f); // Point visé par la caméra
//	//	DirectX::XMVECTOR upVector = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);       // Vecteur "up" de la caméra
//	//
//	//	// Projection orthographique
//	//	float viewWidth = 10.0f;    // Largeur de la vue
//	//	float viewHeight = 10.0f;   // Hauteur de la vue
//	//	float nearZ = 0.1f;         // Plan proche
//	//	float farZ = 100.0f;        // Plan éloigné
//	//
//	//	DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ);
//	//	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(cameraPosition, targetPosition, upVector);
//	//
//	//	DirectX::XMMATRIX transposedProjectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);
//	//	DirectX::XMMATRIX transposedViewMatrix = DirectX::XMMatrixTranspose(viewMatrix);
//	//
//	//	DirectX::XMFLOAT4X4 storedProjectionMatrix;
//	//	DirectX::XMFLOAT4X4 storedViewMatrix;
//	//
//	//	DirectX::XMStoreFloat4x4(&storedProjectionMatrix, transposedProjectionMatrix);
//	//	DirectX::XMStoreFloat4x4(&storedViewMatrix, transposedViewMatrix);
//	//
//	//	// ****************
//
//
//	/*float left = -1.0f;
//	float right = 1.0f;
//	float bottom = -1.0f;
//	float top = 1.0f;
//
//	DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixOrthographicOffCenterLH(left, right, bottom, top, 0.0f, 1.0f);
//	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixIdentity();
//
//	DirectX::XMMATRIX transposedProjectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);
//	DirectX::XMMATRIX transposedViewMatrix = DirectX::XMMatrixTranspose(viewMatrix);
//
//	DirectX::XMFLOAT4X4 storedProjectionMatrix;
//	DirectX::XMFLOAT4X4 storedViewMatrix;
//
//	DirectX::XMStoreFloat4x4(&storedProjectionMatrix, transposedProjectionMatrix);
//	DirectX::XMStoreFloat4x4(&storedViewMatrix, transposedViewMatrix);*/
//
//
//	// SET WORLD
//
//	DirectX::XMFLOAT4X4 I(
//		0.5f, 0.0f, 0.0f, 0.0f,
//		0.0f, 0.5f, 0.0f, 0.0f,
//		0.0f, 0.0f, 1.0f, 0.0f,
//		1.5f, 0.0f, 0.0f, 1.0f);
//
//	DirectX::XMFLOAT4X4 transposedWorld;
//	DirectX::XMStoreFloat4x4(&transposedWorld, DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&I)));
//
//
//	// ***********
//	graphics->Resize(200, 200);
//	graphics->Resize(1500, 800);
//	graphics->StartFrame();
//
//	// DRAW -> ONE FRAME
//	graphics->UpdateViewProjConstantBuffer(storedProjectionMatrix, storedViewMatrix);
//
//	DirectX::XMFLOAT4X4 world = graphics->ToPixel(400, 400, storedProjectionMatrix, storedViewMatrix);
//	DirectX::XMFLOAT4X4 world2 = graphics->ToPixel(600, 600, storedProjectionMatrix, storedViewMatrix);
//
//	GCTest test;
//	test.world = world2;
//	test.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f);
//	//DirectX::XMMATRIX identityMatrix = DirectX::XMMatrixIdentity();
//	//DirectX::XMStoreFloat4x4(&test.world, identityMatrix);
//	//test.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f);
//
//	//graphics->UpdateCustomCBObject<GCTest>(material2.resource, test);
//	//graphics->GetRender()->DrawObject(mesh2.resource, material2.resource);
//
//	graphics->UpdateWorldConstantBuffer(material3.resource, world);
//	graphics->GetRender()->DrawObject(mesh2.resource, material3.resource,false);
//
//	graphics->UpdateCustomCBObject<GCTest>(material2.resource, test);
//	graphics->GetRender()->DrawObject(mesh2.resource, material2.resource,false);
//
//	//graphics->UpdateWorldConstantBuffer(material3.resource, transposedWorld);
//	//graphics->GetRender()->DrawObject(mesh2.resource, material3.resource);
//
//	//graphics->UpdateCustomCBObject<GCTest>(material, worldData);
//	//graphics->GetRender()->DrawObject(mesh, material);
//
//	//profiler.LogInfo(std::to_string(material2->GetObjectCBData().size()));
//
//
//	graphics->EndFrame();
//	// ********************
//
//	window->Run(graphics->GetRender());
//}
//
//
//

//#include "pch.h"
//
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
//    GCGraphicsLogger& profiler = GCGraphicsLogger::GetInstance();
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
//    auto geoCubeOuter = graphics->CreateGeometryPrimitive(CubeSkybox, XMFLOAT4(Colors::Red));
//    auto geoCubeInner = graphics->CreateGeometryPrimitive(Plane, XMFLOAT4(Colors::Green));
//    auto geoCubeInner3 = graphics->CreateGeometryPrimitive(Plane, XMFLOAT4(Colors::Red));
//    auto geoSphere = graphics->CreateGeometryPrimitive(Sphere, XMFLOAT4(Colors::Yellow));
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
//    auto meshCubeOuter = graphics->CreateMeshCustom(geoCubeOuter.resource, flagsLightColor);
//    auto meshCubeInner = graphics->CreateMeshCustom(geoCubeInner.resource, flagsLightColor);
//    auto meshCubeInner3 = graphics->CreateMeshCustom(geoCubeInner3.resource, flagsLightColor);
//    auto meshSphere = graphics->CreateMeshCustom(geoSphere.resource, flagsLightTexture);
//
//    std::string texturePath = "../../../src/Render/Textures/texture.dds";
//    std::string texturePath2 = "../../../src/Render/Textures/cottage_diffuse.dds";
//    auto texture = graphics->CreateTexture(texturePath);
//    auto texture2 = graphics->CreateTexture(texturePath2);
//
//    graphics->InitializeGraphicsResourcesEnd();
//
//    auto materialCubeOuter = graphics->CreateMaterial(shaderLightSkyBox.resource);
//    //materialCubeOuter.resource->SetTexture(texture2.resource);
//
//    auto materialCubeInner = graphics->CreateMaterial(shaderLightColor.resource);
//    auto materialSphere = graphics->CreateMaterial(shaderLightTexture.resource);
//    materialSphere.resource->SetTexture(texture.resource);
//
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
//    while (true) {
//        auto currentTime = std::chrono::steady_clock::now();
//        float elapsedTime = std::chrono::duration<float>(currentTime - startTime).count();
//
//        float rotationSpeed = 1.0f; 
//        float angle = rotationSpeed * elapsedTime;
//        XMMATRIX rotationMatrix = XMMatrixRotationY(angle);
//
//        // Mettre à jour la matrice de transformation du cube interne
//        //XMMATRIX worldMatrixCubeInnerUpdated = rotationMatrix * worldMatrixCubeInner;
//
//        //// Extraire les données de la matrice mise à jour dans une XMFLOAT4X4
//        //XMStoreFloat4x4(&worldCubeInner, XMMatrixTranspose(worldMatrixCubeInnerUpdated));
//
//
//        graphics->StartFrame();
//        graphics->UpdateViewProjConstantBuffer(storedProjectionMatrix, storedViewMatrix);
//
//        //// Dessiner le cube externe
//        //graphics->UpdateWorldConstantBuffer(materialCubeOuter.resource, worldCubeOuter);
//        //graphics->GetRender()->DrawObject(meshCubeOuter.resource, materialCubeOuter.resource);
//
//
//        GCLIGHTSPROPERTIES lightData = {};
//
//        GCLIGHT light2;
//        light2.position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f); // Position en 2D (x, y, 0)
//        light2.direction = DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f); // Direction vers le bas en 2D
//        light2.color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f); // Couleur de la lumière
//        light2.spotAngle = 10.0f; // Angle du spot si applicable
//        light2.lightIntensity = 3.2f;
//        light2.lightType = 1; // Type de lumière ponctuelle
//
//        GCLIGHT pointLight;
//        pointLight.position = DirectX::XMFLOAT3(6.0f, 1.0f, 0.0f); // Position en 2D (x, y, 0)
//        pointLight.direction = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f); // Direction vers le bas en 2D
//        pointLight.color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f); // Couleur de la lumière
//        pointLight.spotAngle = 0.0f; // Angle du spot si applicable
//        pointLight.lightIntensity = 2.4f;
//        pointLight.lightType = 2; // Type de lumière ponctuelle
//
//
//        lightData.lights[1] = pointLight;
//        lightData.lights[0] = light2;
//
//        graphics->UpdateLights(lightData);
//
//
//
//        graphics->UpdateWorldConstantBuffer(materialCubeInner.resource, worldCubeInner2);
//        graphics->GetRender()->DrawObject(meshCubeInner3.resource, materialCubeInner.resource);
//
//        graphics->UpdateWorldConstantBuffer(materialCubeInner.resource, worldCubeInner3);
//        graphics->GetRender()->DrawObject(meshCubeInner3.resource, materialCubeInner.resource);
//
//
//        graphics->UpdateWorldConstantBuffer(materialCubeInner.resource, worldCubeInner);
//        graphics->GetRender()->DrawObject(meshCubeInner.resource, materialCubeInner.resource);
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
#include "../../src/Render/pch.h"
#pragma comment(lib,"Render.lib")

using namespace DirectX;


XMVECTOR cameraPosition = XMVectorSet(0.0f, -10.0f, 5.0f, 1.0f);
XMVECTOR cameraTarget = XMVectorZero();
XMVECTOR cameraUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

float cameraMoveSpeed = 0.05f;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) {
    // Initialisation des ressources graphiques
    GCGraphicsLogger& profiler = GCGraphicsLogger::GetInstance();
    profiler.InitializeConsole();

    Window* window = new Window(hInstance);
    window->Initialize();

    GCGraphics* graphics = new GCGraphics();
    graphics->Initialize(window, 1920, 1064);
    //graphics->Resize(1500, 800);
    int flagsLightColor = 0;
    SET_FLAG(flagsLightColor, VERTEX_POSITION);
    SET_FLAG(flagsLightColor, VERTEX_COLOR);
    SET_FLAG(flagsLightColor, VERTEX_NORMAL);

    int flagsLightTexture = 0;
    SET_FLAG(flagsLightTexture, VERTEX_POSITION);
    SET_FLAG(flagsLightTexture, VERTEX_UV);
    SET_FLAG(flagsLightTexture, VERTEX_NORMAL);

    auto geometryPostProcessing = graphics->CreateGeometryPrimitive(Quad, DirectX::XMFLOAT4(DirectX::Colors::Yellow));
    auto geoCubeOuter = graphics->CreateGeometryPrimitive(CubeSkybox, XMFLOAT4(Colors::Red));
    auto geoCubeInner = graphics->CreateGeometryPrimitive(Cube, XMFLOAT4(Colors::Green));
    auto geoSphere = graphics->CreateGeometryPrimitive(Sphere, XMFLOAT4(Colors::Yellow));

    std::string shaderFilePath1 = "../../../src/RenderApp/Shaders/LightColor.hlsl";
    std::string csoDestinationPath1 = "../../../src/RenderApp/CsoCompiled/LightColor";
    auto shaderLightColor = graphics->CreateShaderCustom(shaderFilePath1, csoDestinationPath1, flagsLightColor, D3D12_CULL_MODE_BACK);

    std::string shaderFilePath2 = "../../../src/RenderApp/Shaders/LightTextureBloom.hlsl";
    std::string csoDestinationPath2 = "../../../src/RenderApp/CsoCompiled/LightTextureBloom";
    auto shaderLightTexture = graphics->CreateShaderCustom(shaderFilePath2, csoDestinationPath2, flagsLightTexture, D3D12_CULL_MODE_BACK);

    auto shaderLightSkyBox = graphics->CreateShaderCustom(shaderFilePath1, csoDestinationPath1, flagsLightColor, D3D12_CULL_MODE_NONE);

    graphics->InitializeGraphicsResourcesStart();

    int flags = 0;
    SET_FLAG(flags, VERTEX_POSITION);
    SET_FLAG(flags, VERTEX_UV);


    auto meshPostProcessing = graphics->CreateMeshCustom(geometryPostProcessing.resource, flags);
    // Cr�ation des meshes
    auto meshCubeOuter = graphics->CreateMeshCustom(geoCubeOuter.resource, flagsLightColor);
    auto meshCubeInner = graphics->CreateMeshCustom(geoCubeInner.resource, flagsLightColor);
    auto meshSphere = graphics->CreateMeshCustom(geoSphere.resource, flagsLightTexture);

    std::string texturePath = "../../../src/Render/Textures/texture.dds";
    std::string texturePath2 = "../../../src/Render/Textures/cottage_diffuse.dds";
    auto texture = graphics->CreateTexture(texturePath);
    auto texture2 = graphics->CreateTexture(texturePath2);

    graphics->InitializeGraphicsResourcesEnd();

    auto materialCubeOuter = graphics->CreateMaterial(shaderLightSkyBox.resource);
    //materialCubeOuter.resource->SetTexture(texture2.resource);

    auto materialCubeInner = graphics->CreateMaterial(shaderLightColor.resource);
    auto materialSphere = graphics->CreateMaterial(shaderLightTexture.resource);
    materialSphere.resource->SetTexture(texture.resource);

    XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(0.25f * XM_PI, window->AspectRatio(), 1.0f, 1000.0f);
    XMMATRIX viewMatrix = XMMatrixLookAtLH(cameraPosition, cameraTarget, cameraUp);
    XMMATRIX transposedProjectionMatrix = XMMatrixTranspose(projectionMatrix);
    XMMATRIX transposedViewMatrix = XMMatrixTranspose(viewMatrix);
    XMFLOAT4X4 storedProjectionMatrix;
    XMFLOAT4X4 storedViewMatrix;
    XMStoreFloat4x4(&storedProjectionMatrix, transposedProjectionMatrix);
    XMStoreFloat4x4(&storedViewMatrix, transposedViewMatrix);

    XMMATRIX worldMatrixCubeOuter = XMMatrixScaling(20.0f, 20.0f, 20.0f) * XMMatrixTranslation(0.0f, -3.0f, 0.0f); // Cube externe (skybox)
    XMMATRIX worldMatrixCubeInner = XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixTranslation(0.0f, -4.0f, 3.0f); // Cube interne centr�
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

    while (true) {
        auto currentTime = std::chrono::steady_clock::now();
        float elapsedTime = std::chrono::duration<float>(currentTime - startTime).count();


        float rotationSpeed = 1.0f;
        float angle = rotationSpeed * elapsedTime;
        XMMATRIX rotationMatrix = XMMatrixRotationY(angle);


        XMMATRIX worldMatrixCubeInnerUpdated = rotationMatrix * worldMatrixCubeInner;


        XMStoreFloat4x4(&worldCubeInner, XMMatrixTranspose(worldMatrixCubeInnerUpdated));

        // Gestion des entr�es utilisateur pour le d�placement de la cam�ra
        //if (window->IsKeyDown('Z')) {
        //    cameraPosition += cameraMoveSpeed * XMVector3Normalize(XMVectorSubtract(cameraTarget, cameraPosition));
        //}
        //if (window->IsKeyDown('S')) {
        //    cameraPosition -= cameraMoveSpeed * XMVector3Normalize(XMVectorSubtract(cameraTarget, cameraPosition));
        //}

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

        GCLIGHT pointLight;
        pointLight.position = DirectX::XMFLOAT3(0.0f, 4.0f, 0.0f); // Position en 2D (x, y, 0)
        pointLight.direction = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
        pointLight.color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f); // Couleur de la lumière
        pointLight.spotAngle = 0.0f; // Angle du spot si applicable
        pointLight.lightIntensity = 20.4f;
        pointLight.lightType = 2; // Type de lumière ponctuelle


        lightData.lights[1] = pointLight;

        lightData.lights[0] = directionalLight;

        graphics->UpdateLights(lightData);


        graphics->StartFrame();
        graphics->UpdateViewProjConstantBuffer(storedProjectionMatrix, storedViewMatrix);

        graphics->UpdateWorldConstantBuffer(materialCubeOuter.resource, worldCubeOuter, 1.0f);
        graphics->GetRender()->DrawObject(meshCubeOuter.resource, materialCubeOuter.resource, true);

        graphics->UpdateWorldConstantBuffer(materialCubeInner.resource, worldCubeInner, 2.0f);
        graphics->GetRender()->DrawObject(meshCubeInner.resource, materialCubeInner.resource, false);

        graphics->UpdateWorldConstantBuffer(materialSphere.resource, worldSphere, 3.0f);
        graphics->GetRender()->DrawObject(meshSphere.resource, materialSphere.resource, false);

        graphics->UpdateWorldConstantBuffer(materialSphere.resource, worldCubeInner2, 4.0f);
        graphics->GetRender()->DrawObject(meshSphere.resource, materialSphere.resource, false);

        graphics->EndFrame();
        window->Run(graphics->GetRender());
        }

    return 0;
}

//GCLIGHT directionalLight;
//directionalLight.position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f); // Pas de position pour une lumière directionnelle
//directionalLight.direction = DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f); // Direction du soleil (exemple)
//directionalLight.color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f); // Couleur de la lumière
//directionalLight.spotAngle = 0.0f; // Pas d'angle pour une lumière directionnelle
//directionalLight.lightIntensity = 1.3f;
//directionalLight.lightType = 0; // Type de lumière directionnelle

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

//// Lumière ponctuelle 2
////GCLIGHT light2;
////light2.position = DirectX::XMFLOAT3(-5.0f, 0.0f, 0.0f); // Position en 2D (x, y, 0)
////light2.direction = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f); // Direction vers le bas en 2D
////light2.color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f); // Couleur de la lumière
////light2.spotAngle = 10.0f; // Angle du spot si applicable
////light2.lightIntensity = 5.2f;
////light2.lightType = 1; // Type de lumière ponctuelle

////GCLIGHT pointLight;
////pointLight.position = DirectX::XMFLOAT3(6.0f, 1.0f, 0.0f); // Position en 2D (x, y, 0)
////pointLight.direction = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f); // Direction vers le bas en 2D
////pointLight.color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f); // Couleur de la lumière
////pointLight.spotAngle = 0.0f; // Angle du spot si applicable
////pointLight.lightIntensity = 2.4f;
////pointLight.lightType = 2; // Type de lumière ponctuelle


//lightData.lights[0] = directionalLight;
//lightData.lights[1] = light2;
//lightData.lights[2] = light1;
////lightData.lights[0] = pointLight;

//GCGraphicsLogger& profiler = GCGraphicsLogger::GetInstance();
