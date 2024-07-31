//﻿// 2D BASIC
//
//#include "pch.h";
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
//
//    GCGraphicsLogger& profiler = GCGraphicsLogger::GetInstance();
//    profiler.InitializeConsole();
//
//
//
//    Window* window = new Window(hInstance);
//    window->Initialize();
//
//
//
//    GCGraphics* graphics = new GCGraphics();
//    graphics->Initialize(window, 1920, 1080);
//
//
//
//    int flagsLightColor = 0;
//    SET_FLAG(flagsLightColor, VERTEX_POSITION);
//    SET_FLAG(flagsLightColor, VERTEX_COLOR);
//    SET_FLAG(flagsLightColor, VERTEX_NORMAL);
//
//    int flagsLightTexture = 0;
//    SET_FLAG(flagsLightTexture, VERTEX_POSITION);
//    SET_FLAG(flagsLightTexture, VERTEX_UV);
//    SET_FLAG(flagsLightTexture, VERTEX_NORMAL);
//
//    auto geoCubeOuter = graphics->CreateGeometryPrimitive(CubeSkybox, XMFLOAT4(Colors::Red));
//    auto geoCubeInner = graphics->CreateGeometryPrimitive(Plane, XMFLOAT4(Colors::Green));
//    auto geoCubeInner3 = graphics->CreateGeometryPrimitive(Plane, XMFLOAT4(Colors::Red));
//    auto geoSphere = graphics->CreateGeometryPrimitive(Sphere, XMFLOAT4(Colors::Yellow));
//
//    graphics->m_pFontGeometryLoader->Initialize("../../../src/Render/Fonts/LetterUV.txt");
//
//    auto geoPlaneAlphabet = graphics->m_pFontGeometryLoader->CreateText("gta buy now");
//
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
//    auto shaderTexture = graphics->CreateShaderTexture();
//
//    graphics->InitializeGraphicsResourcesStart();
//
//    auto meshCubeOuter = graphics->CreateMeshCustom(geoCubeOuter.resource, flagsLightColor);
//    auto meshCubeInner = graphics->CreateMeshCustom(geoCubeInner.resource, flagsLightColor);
//    auto meshCubeInner3 = graphics->CreateMeshCustom(geoCubeInner3.resource, flagsLightColor);
//    auto meshSphere = graphics->CreateMeshCustom(geoSphere.resource, flagsLightTexture);
//
//    auto meshPlaneAlphabet = graphics->CreateMeshTexture(geoPlaneAlphabet);
//
//    std::string texturePath = "../../../src/Render/Textures/texture.dds";
//    std::string texturePath2 = "../../../src/Render/Textures/alphabet.dds";
//    auto texture = graphics->CreateTexture(texturePath);
//    auto texture2 = graphics->CreateTexture(texturePath2);
//
//
//    graphics->InitializeGraphicsResourcesEnd();
//
//    auto materialCubeOuter = graphics->CreateMaterial(shaderLightSkyBox.resource);
//
//
//    auto materialCubeInner = graphics->CreateMaterial(shaderTexture.resource);
//    materialCubeInner.resource->SetTexture(texture2.resource);
//
//
//
//    auto materialSphere = graphics->CreateMaterial(shaderLightTexture.resource);
//    materialSphere.resource->SetTexture(texture.resource);
//
//    float viewWidth = 20.0f;
//    float viewHeight = 20.0f;
//    XMMATRIX projectionMatrix = XMMatrixOrthographicLH(viewWidth, viewHeight, 1.0f, 1000.0f);
//    XMMATRIX viewMatrix = XMMatrixLookAtLH(cameraPosition, cameraTarget, cameraUp);
//    XMMATRIX transposedProjectionMatrix = XMMatrixTranspose(projectionMatrix);
//    XMMATRIX transposedViewMatrix = XMMatrixTranspose(viewMatrix);
//    GCMATRIX storedProjectionMatrix = GCUtils::XMMATRIXToGCMATRIX(transposedProjectionMatrix);
//    GCMATRIX storedViewMatrix = GCUtils::XMMATRIXToGCMATRIX(transposedViewMatrix);
//
//    XMMATRIX worldMatrixCubeOuter = XMMatrixScaling(20.0f, 20.0f, 20.0f) * XMMatrixTranslation(0.0f, -3.0f, 0.0f); // Cube externe (skybox)
//    XMMATRIX worldMatrixCubeInner = XMMatrixScaling(20.0f, 20.0f, 20.0f) * XMMatrixTranslation(-0.0f, 0.0f, -1.0f); // Cube interne centré
//    XMMATRIX worldMatrixCubeInner2 = XMMatrixScaling(5.0f, 5.0f, 5.0f) * XMMatrixTranslation(-10.0f, 0.0f, 0.0f); // Cube interne centré
//    XMMATRIX worldMatrixCubeInner3 = XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixTranslation(2.0f, -2.0f, -1.0f); // Cube interne centré
//    XMMATRIX worldMatrixSphere = XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixTranslation(3.0f, 5.0f, -2.0f); // Sphère déplacée dans le cube interne
//
//    GCMATRIX worldCubeOuter = GCUtils::XMMATRIXToGCMATRIX(worldMatrixCubeOuter);
//    GCMATRIX worldCubeInner = GCUtils::XMMATRIXToGCMATRIX(worldMatrixCubeInner);
//    GCMATRIX worldCubeInner2 = GCUtils::XMMATRIXToGCMATRIX(worldMatrixCubeInner2);
//    GCMATRIX worldCubeInner3 = GCUtils::XMMATRIXToGCMATRIX(worldMatrixCubeInner3);
//    GCMATRIX worldSphere = GCUtils::XMMATRIXToGCMATRIX(worldMatrixSphere);
//
//    auto startTime = std::chrono::steady_clock::now();
//
//    while (true) {
//
//        //graphics->GetRender()->GetRenderResources()->GetDebugController()->ReportLiveObjects();
//        auto currentTime = std::chrono::steady_clock::now();
//        float elapsedTime = std::chrono::duration<float>(currentTime - startTime).count();
//
//
//        graphics->StartFrame();
//        graphics->UpdateViewProjConstantBuffer(storedProjectionMatrix, storedViewMatrix);
//
//        std::vector<GCLIGHT> lights;
//        GCLIGHT light2;
//        light2.position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f); // Position en 2D (x, y, 0)
//        light2.direction = DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f); // Direction vers le bas en 2D
//        light2.color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f); // Couleur de la lumière
//        light2.spotAngle = 10.0f; // Angle du spot si applicable
//        light2.lightIntensity = 3.2f;
//        light2.lightType = 1; // Type de lumière ponctuelle
//        GCLIGHT pointLight;
//        pointLight.position = DirectX::XMFLOAT3(6.0f, 1.0f, 0.0f); // Position en 2D (x, y, 0)
//        pointLight.direction = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f); // Direction vers le bas en 2D
//        pointLight.color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f); // Couleur de la lumière
//        pointLight.spotAngle = 0.0f; // Angle du spot si applicable
//        pointLight.lightIntensity = 2.4f;
//        pointLight.lightType = 2; // Type de lumière ponctuelle
//
//
//
//        lights.push_back(light2);
//        lights.push_back(pointLight);
//
//        graphics->UpdateLights(lights);
//
//        graphics->UpdateWorldConstantBuffer(materialCubeInner.resource, worldCubeInner2);
//        graphics->GetRender()->DrawObject(meshPlaneAlphabet.resource, materialCubeInner.resource, false);
//
//        //graphics->UpdateWorldConstantBuffer(materialCubeInner.resource, worldCubeInner3);
//        //graphics->GetRender()->DrawObject(meshCubeInner3.resource, materialCubeInner.resource, true);
//
//
//        //graphics->UpdateWorldConstantBuffer(materialCubeInner.resource, worldCubeInner);
//        //graphics->GetRender()->DrawObject(meshCubeInner.resource, materialCubeInner.resource , true);
//
//        graphics->EndFrame();
//        window->Run(graphics->GetRender());
//
//    }
//
//
//
//    return 0;
//}

// 2D SPRITE SHEET

#include "pch.h"


using namespace DirectX;

// Définition des variables globales pour la caméra
XMVECTOR cameraPosition = XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f);
XMVECTOR cameraTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
XMVECTOR cameraUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

float cameraMoveSpeed = 0.05f; // Vitesse de déplacement de la caméra

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) {

    


    // Initialisation des ressources graphiques
    GCGraphicsLogger& profiler = GCGraphicsLogger::GetInstance();
    profiler.InitializeConsole();

    Window* window = new Window(hInstance);
    window->Initialize();

    GCGraphics* graphics = new GCGraphics();
    graphics->Initialize(window, 1920, 1080);

    int flagsLightColor = 0;
    GC_SET_FLAG(flagsLightColor, GC_VERTEX_POSITION);
    GC_SET_FLAG(flagsLightColor, GC_VERTEX_COLOR);
    GC_SET_FLAG(flagsLightColor, GC_VERTEX_NORMAL);

    int flagsLightTexture = 0;
    GC_SET_FLAG(flagsLightTexture, GC_VERTEX_POSITION);
    GC_SET_FLAG(flagsLightTexture, GC_VERTEX_UV);
    GC_SET_FLAG(flagsLightTexture, GC_VERTEX_NORMAL);


    auto geoCubeOuter = graphics->CreateGeometryPrimitive(CubeSkybox, XMFLOAT4(Colors::Red));
    auto geoCubeInner = graphics->CreateGeometryPrimitive(Plane, XMFLOAT4(Colors::Green));
    auto geoCubeInner3 = graphics->CreateGeometryPrimitive(Plane, XMFLOAT4(Colors::Red));
    auto geoSphere = graphics->CreateGeometryPrimitive(Sphere, XMFLOAT4(Colors::Yellow));

    auto geoPlane = graphics->CreateGeometryPrimitive(Plane, XMFLOAT4(Colors::Green));


    // Load Sprite Sheet
    GC_SPRITESHEET_DATA spriteSheetInfo = graphics->m_pSpriteSheetGeometryLoader->LoadSpriteSheet("../../../src/Render/SpriteSheet/SS_data.ssdg");
    // Set uv on plane
    graphics->m_pSpriteSheetGeometryLoader->SetSpriteUVs(geoPlane.resource, 0, 151, spriteSheetInfo);



    // Chargement des shaders personnalisés
    std::string shaderFilePath1 = "../../../src/Render/Shaders/LightColor.hlsl";
    std::string csoDestinationPath1 = "../../../src/Render/CsoCompiled/LightColor";
    auto shaderLightColor = graphics->CreateShaderCustom(shaderFilePath1, csoDestinationPath1, flagsLightColor, D3D12_CULL_MODE_BACK);

    std::string shaderFilePath2 = "../../../src/Render/Shaders/LightTexture.hlsl";
    std::string csoDestinationPath2 = "../../../src/Render/CsoCompiled/LightTexture";
    auto shaderLightTexture = graphics->CreateShaderCustom(shaderFilePath2, csoDestinationPath2, flagsLightTexture, D3D12_CULL_MODE_BACK);

    auto shaderLightSkyBox = graphics->CreateShaderCustom(shaderFilePath1, csoDestinationPath1, flagsLightColor, D3D12_CULL_MODE_NONE);

    auto shaderTexture = graphics->CreateShaderTexture();

    graphics->InitializeGraphicsResourcesStart();

    auto meshCubeOuter = graphics->CreateMeshCustom(geoCubeOuter.resource, flagsLightColor);
    auto meshCubeInner = graphics->CreateMeshCustom(geoCubeInner.resource, flagsLightColor);
    auto meshCubeInner3 = graphics->CreateMeshCustom(geoCubeInner3.resource, flagsLightColor);
    auto meshSphere = graphics->CreateMeshCustom(geoSphere.resource, flagsLightTexture);

    auto meshPlaneAlphabet = graphics->CreateMeshTexture(geoPlane.resource);
    //auto meshPlaneAlphabet2 = graphics->CreateMeshTexture(geoPlaneOneSprite2);

    std::string texturePath = "../../../src/Render/Textures/texture.dds";
    std::string texturePath2 = "../../../src/Render/Textures/sprite_sheet_0.dds";
    auto texture = graphics->CreateTexture(texturePath);
    auto texture2 = graphics->CreateTexture(texturePath2);

    graphics->InitializeGraphicsResourcesEnd();

    auto materialCubeOuter = graphics->CreateMaterial(shaderLightSkyBox.resource);


    auto materialCubeInner = graphics->CreateMaterial(shaderTexture.resource);
    materialCubeInner.resource->SetTexture(texture2.resource);



    auto materialSphere = graphics->CreateMaterial(shaderLightTexture.resource);
    materialSphere.resource->SetTexture(texture.resource);

    float viewWidth = 20.0f;
    float viewHeight = 20.0f;
    XMMATRIX projectionMatrix = XMMatrixOrthographicLH(viewWidth, viewHeight, 1.0f, 1000.0f);
    XMMATRIX viewMatrix = XMMatrixLookAtLH(cameraPosition, cameraTarget, cameraUp);
    XMMATRIX transposedProjectionMatrix = XMMatrixTranspose(projectionMatrix);
    XMMATRIX transposedViewMatrix = XMMatrixTranspose(viewMatrix);
    GCMATRIX storedProjectionMatrix = GCUtils::XMMATRIXToGCMATRIX(transposedProjectionMatrix);
    GCMATRIX storedViewMatrix = GCUtils::XMMATRIXToGCMATRIX(transposedViewMatrix);

    XMMATRIX worldMatrixCubeOuter = XMMatrixScaling(20.0f, 20.0f, 20.0f) * XMMatrixTranslation(0.0f, -3.0f, 0.0f); 
    XMMATRIX worldMatrixCubeInner = XMMatrixScaling(10.0f, 10.0f, 10.0f) * XMMatrixTranslation(1.0f, 0.0f, 0.0f); 
    XMMATRIX worldMatrixCubeInner2 = XMMatrixScaling(10.0f, 10.0f, 10.0f) * XMMatrixTranslation(-1.0f, 0.0f, 0.0f); 
    XMMATRIX worldMatrixCubeInner3 = XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixTranslation(2.0f, -2.0f, -1.0f); 
    XMMATRIX worldMatrixSphere = XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixTranslation(3.0f, 5.0f, -2.0f); 



    // #TODO Update for each object - Scaling Ratio
    float scaleX = 1.0f;
    float scaleY = 1.0f;
    float aspectRatio = window->AspectRatio();

    if (aspectRatio > 1.0f) {
        scaleX = 1.0f / aspectRatio;
    }
    else {
        scaleY = aspectRatio;
    }
    XMMATRIX additionalScaleMatrix = XMMatrixScaling(scaleX, scaleY, 1.0f); 
    XMMATRIX worldMatrixCubeInnerScaled = worldMatrixCubeInner * additionalScaleMatrix;

    GCMATRIX worldCubeOuter = GCUtils::XMMATRIXToGCMATRIX(worldMatrixCubeOuter);
    GCMATRIX worldCubeInner = GCUtils::XMMATRIXToGCMATRIX(worldMatrixCubeInnerScaled);
    GCMATRIX worldCubeInner2 = GCUtils::XMMATRIXToGCMATRIX(worldMatrixCubeInner2);
    GCMATRIX worldCubeInner3 = GCUtils::XMMATRIXToGCMATRIX(worldMatrixCubeInner3);
    GCMATRIX worldSphere = GCUtils::XMMATRIXToGCMATRIX(worldMatrixSphere);

    // #TODO FEATURE SCALING ETIREMENT SPRITE

    auto startTime = std::chrono::steady_clock::now();

    while (true) {
        auto currentTime = std::chrono::steady_clock::now();
        float elapsedTime = std::chrono::duration<float>(currentTime - startTime).count();


        graphics->StartFrame();
        graphics->UpdateViewProjConstantBuffer(storedProjectionMatrix, storedViewMatrix);

        std::vector<GCLIGHT> lights;
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


        
        lights.push_back(light2);
        lights.push_back(pointLight);

        graphics->UpdateLights(lights);

        graphics->UpdateWorldConstantBuffer(materialCubeInner.resource, worldCubeInner);
        graphics->GetRender()->DrawObject(meshPlaneAlphabet.resource, materialCubeInner.resource, false);

        //graphics->UpdateWorldConstantBuffer(materialCubeInner.resource, worldCubeInner2);
        //graphics->GetRender()->DrawObject(meshPlaneAlphabet2.resource, materialCubeInner.resource, true);
        //graphics->UpdateWorldConstantBuffer(materialCubeInner.resource, worldCubeInner3);
        //graphics->GetRender()->DrawObject(meshCubeInner3.resource, materialCubeInner.resource, true);


        //graphics->UpdateWorldConstantBuffer(materialCubeInner.resource, worldCubeInner);
        //graphics->GetRender()->DrawObject(meshCubeInner.resource, materialCubeInner.resource , true);

        graphics->EndFrame();
        window->Run(graphics->GetRender());
    }

    return 0;
}


//// 3D DEFERRED SHADING
//
//#include "pch.h"
//
//using namespace DirectX;
//
//
//
//
//float cameraMoveSpeed = 0.05f;
//
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) {
//    // Initialisation des ressources graphiques
//    GCGraphicsLogger& profiler = GCGraphicsLogger::GetInstance();
//    profiler.InitializeConsole();
//
//    Window* window = new Window(hInstance);
//    window->Initialize();
//
//    std::string PPFilePath = "../../../src/Render/Shaders/test.hlsl";
//    std::string PPcsoDestinationPath = "../../../src/Render/CsoCompiled/test";
//
//    GCGraphics* graphics = new GCGraphics();
//    graphics->Initialize(window, 1920, 1080);
//
//    //graphics->GetRender()->CreatePostProcessingResources("../../../src/Render/Shaders/test.hlsl", "../../../src/Render/CsoCompiled/test");
//    //graphics->GetRender()->ActiveCSPostProcessing();
//    //graphics->GetRender()->ActiveDeferredLightPass();
//
//    int flagsLightColor = 0;
//    SET_FLAG(flagsLightColor, VERTEX_POSITION);
//    SET_FLAG(flagsLightColor, VERTEX_COLOR);
//    SET_FLAG(flagsLightColor, VERTEX_NORMAL);
//
//    int flagsLightTexture = 0;
//    SET_FLAG(flagsLightTexture, VERTEX_POSITION);
//    SET_FLAG(flagsLightTexture, VERTEX_UV);
//    SET_FLAG(flagsLightTexture, VERTEX_NORMAL);
//
//    auto geometryPostProcessing = graphics->CreateGeometryPrimitive(Quad, DirectX::XMFLOAT4(DirectX::Colors::Yellow));
//    auto geoCubeOuter = graphics->CreateGeometryPrimitive(CubeSkybox, XMFLOAT4(Colors::Red));
//    auto geoCubeInner = graphics->CreateGeometryPrimitive(Cube, XMFLOAT4(Colors::Green));
//    auto geoSphere = graphics->CreateGeometryPrimitive(Sphere, XMFLOAT4(Colors::Yellow));
//
//    std::string shaderFilePath1 = "../../../src/Render/Shaders/LightColor.hlsl";
//    std::string csoDestinationPath1 = "../../../src/Render/CsoCompiled/LightColor";
//
//    std::string shaderFilePath3 = "../../../src/Render/Shaders/colorDeferredLight.hlsl";
//    std::string csoDestinationPath3 = "../../../src/Render/CsoCompiled/colorDeferredLight";
//
//    auto shaderLightColor = graphics->CreateShaderCustom(shaderFilePath3, csoDestinationPath3, flagsLightColor);
//
//    std::string shaderFilePath2 = "../../../src/Render/Shaders/LightTexture.hlsl";
//    std::string csoDestinationPath2 = "../../../src/Render/CsoCompiled/LightTexture";
//    auto shaderLightTexture = graphics->CreateShaderCustom(shaderFilePath2, csoDestinationPath2, flagsLightTexture, D3D12_CULL_MODE_BACK);
//
//    auto shaderLightSkyBox = graphics->CreateShaderCustom(shaderFilePath1, csoDestinationPath1, flagsLightColor, D3D12_CULL_MODE_NONE);
//
//    graphics->InitializeGraphicsResourcesStart();
//
//    int flags = 0;
//    SET_FLAG(flags, VERTEX_POSITION);
//    SET_FLAG(flags, VERTEX_UV);
//
//    auto meshPostProcessing = graphics->CreateMeshCustom(geometryPostProcessing.resource, flags);
//
//    // Cr�ation des meshes
//    auto meshCubeOuter = graphics->CreateMeshCustom(geoCubeOuter.resource, flagsLightColor);
//    auto meshCubeInner = graphics->CreateMeshCustom(geoCubeInner.resource, flagsLightColor);
//    auto meshSphere = graphics->CreateMeshCustom(geoSphere.resource, flagsLightColor);
//
//    //meshCubeInner.resource->AddGeometry(geoCubeInner.resource, XMFLOAT3(1.0f, 1.0f, 1.0f));
//    std::string texturePath = "../../../src/Render/Textures/texture.dds";
//    std::string texturePath2 = "../../../src/Render/Textures/cottage_diffuse.dds";
//    auto texture = graphics->CreateTexture(texturePath);
//    auto texture2 = graphics->CreateTexture(texturePath2);
//
//    graphics->InitializeGraphicsResourcesEnd();
//
//    // Cr�ation des mat�riaux
//    auto materialCubeOuter = graphics->CreateMaterial(shaderLightSkyBox.resource);
//    //materialCubeOuter.resource->SetTexture(texture2.resource);
//
//    auto materialCubeInner = graphics->CreateMaterial(shaderLightColor.resource);
//    auto materialCubeInner2 = graphics->CreateMaterial(shaderLightColor.resource);
//    auto materialSphere = graphics->CreateMaterial(shaderLightTexture.resource);
//    materialSphere.resource->SetTexture(texture.resource);
//
//    GCVEC3 cameraPosition(0.0f, -10.0f, 5.0f);
//    GCVEC3 cameraTarget(0.0f, 0.0f, 0.0f);
//    GCVEC3 cameraUp(0.0f, 1.0f, 0.0f);
//    float fieldOfView = 0.25f * XM_PI; // Exemple de champ de vision pour la perspective
//    float aspectRatio = window->AspectRatio(); // Assurez-vous que `window` est défini
//    float nearZ = 1.0f;
//    float farZ = 1000.0f;
//
//    GCMATRIX storedProjectionMatrix, storedViewMatrix;
//    graphics->CreateViewProjConstantBuffer(
//        cameraPosition,
//        cameraTarget,
//        cameraUp,
//        fieldOfView,
//        aspectRatio,
//        nearZ,
//        farZ,
//        20.0f,
//        20.0f,
//        PERSPECTIVE,
//        storedProjectionMatrix,
//        storedViewMatrix
//    );
//
//    XMMATRIX worldMatrixCubeOuter = XMMatrixScaling(20.0f, 20.0f, 20.0f) * XMMatrixTranslation(0.0f, -3.0f, 0.0f); // Cube externe (skybox)
//    XMMATRIX worldMatrixCubeInner = XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixTranslation(0.0f, 10.0f, 0.0f); // Cube interne centr�
//    XMMATRIX worldMatrixCubeInner2 = XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixTranslation(-6.0f, 0.0f, 0.0f); // Cube interne centr�
//    XMMATRIX worldMatrixSphere = XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixTranslation(3.0f, 5.0f, -2.0f); // Sph�re d�plac�e dans le cube interne
//
//    GCMATRIX worldCubeInner = GCUtils::XMMATRIXToGCMATRIX(worldMatrixCubeInner);
//    GCMATRIX worldCubeInner2 = GCUtils::XMMATRIXToGCMATRIX(worldMatrixCubeInner2);
//
//
//    auto startTime = std::chrono::steady_clock::now();
//    auto lastFrameTime = startTime;
//
//    while (true) {
//        auto currentTime = std::chrono::steady_clock::now();
//        float elapsedTime = std::chrono::duration<float>(currentTime - startTime).count();
//
//
//        float rotationSpeed = 1.0f;
//        float angle = rotationSpeed * elapsedTime;
//        XMMATRIX rotationMatrix = XMMatrixRotationY(angle);
//
//
//        GCMATERIALPROPERTIES materialProperties;
//        materialProperties.ambientLightColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//        materialProperties.ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
//        materialProperties.diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//        materialProperties.specular = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
//        materialProperties.shininess = 128.0f;
//        graphics->UpdateMaterialProperties(materialCubeInner.resource, materialProperties);
//
//        GCMATERIALPROPERTIES materialProperties2;
//        materialProperties2.ambientLightColor = DirectX::XMFLOAT4(0.5f, 0.2f, 1.0f, 1.0f);
//        materialProperties2.ambient = DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
//        materialProperties2.diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//        materialProperties2.specular = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//        materialProperties2.shininess = 128.0f;
//        graphics->UpdateMaterialProperties(materialCubeInner2.resource, materialProperties2);
//
//        std::vector<GCLIGHT> lights;
//
//        GCLIGHT directionalLight;
//        directionalLight.position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
//        directionalLight.direction = DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f);
//        directionalLight.color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
//        directionalLight.spotAngle = 0.0f;
//        directionalLight.lightIntensity = 1.3f;
//        directionalLight.lightType = LIGHT_TYPE_DIRECTIONAL;
//        GCLIGHT pointLight;
//        pointLight.position = DirectX::XMFLOAT3(0.0f, 4.0f, 0.0f);
//        pointLight.direction = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
//        pointLight.color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
//        pointLight.spotAngle = 0.0f;
//        pointLight.lightIntensity = 20.4f;
//        pointLight.lightType = LIGHT_TYPE_POINT;
//
//        GCLIGHT light1;
//        light1.position = DirectX::XMFLOAT3(-5.0f, 20.0f, -2.0f);
//        light1.direction = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
//        light1.color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
//        light1.spotAngle = 10.0f;
//        light1.lightIntensity = 1.2f;
//        light1.lightType = 1;
//
//        //lights.push_back(directionalLight);
//        //lights.push_back(pointLight);
//        lights.push_back(light1);
//
//        graphics->UpdateLights(lights);
//
//        graphics->StartFrame();
//
//
//        graphics->UpdateWorldConstantBuffer(materialCubeInner.resource, worldCubeInner2, 2.0f);
//        graphics->GetRender()->DrawObject(meshSphere.resource, materialCubeInner.resource, true);
//
//        //graphics->UpdateWorldConstantBuffer(materialCubeInner2.resource, worldCubeInner, 2.0f);
//        //graphics->GetRender()->DrawObject(meshSphere.resource, materialCubeInner2.resource, true);
//
//        graphics->EndFrame();
//        window->Run(graphics->GetRender());
//    }
//
//    delete graphics;
//
//    return 0;
//}

// 3D LIGHT

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



//
//// 2D DEFERRED SHADING
//
//#include "pch.h"
//
//using namespace DirectX;
//
//XMMATRIX CreateBillboardMatrix(XMVECTOR objectPosition, XMVECTOR cameraPosition, XMVECTOR cameraUp) {
//    XMVECTOR lookAt = XMVector3Normalize(cameraPosition - objectPosition);
//    XMVECTOR right = XMVector3Normalize(XMVector3Cross(cameraUp, lookAt));
//    XMVECTOR up = XMVector3Cross(lookAt, right);
//
//    XMMATRIX billboardMatrix = XMMatrixIdentity();
//    billboardMatrix.r[0] = right;
//    billboardMatrix.r[1] = up;
//    billboardMatrix.r[2] = lookAt;
//    billboardMatrix.r[3] = objectPosition;
//
//    return XMMatrixTranspose(billboardMatrix);
//}
//
//// D�finition des variables globales pour la cam�ra
//XMVECTOR cameraPosition = XMVectorSet(0.0f, -10.0f, 5.0f, 1.0f);
//XMVECTOR cameraTarget = XMVectorZero();
//XMVECTOR cameraUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
//
//float cameraMoveSpeed = 0.05f;
//
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) {
//    // Initialisation des ressources graphiques
//    GCGraphicsLogger& profiler = GCGraphicsLogger::GetInstance();
//    profiler.InitializeConsole();
//
//    Window* window = new Window(hInstance);
//    window->Initialize();
//
//    std::string PPFilePath = "../../../src/Render/Shaders/test.hlsl";
//    std::string PPcsoDestinationPath = "../../../src/Render/CsoCompiled/test";
//
//    GCGraphics* graphics = new GCGraphics();
//    graphics->Initialize(window, 1920, 1080);
//
//    graphics->GetRender()->CreatePostProcessingResources("../../../src/Render/Shaders/test.hlsl", "../../../src/Render/CsoCompiled/test");
//    //graphics->GetRender()->ActiveCSPostProcessing();
//    //graphics->GetRender()->ActiveDeferredLightPass();
//
//    int flagsLightColor = 0;
//    SET_FLAG(flagsLightColor, VERTEX_POSITION);
//    SET_FLAG(flagsLightColor, VERTEX_COLOR);
//    SET_FLAG(flagsLightColor, VERTEX_NORMAL);
//
//    int flagsLightTexture = 0;
//    SET_FLAG(flagsLightTexture, VERTEX_POSITION);
//    SET_FLAG(flagsLightTexture, VERTEX_UV);
//    SET_FLAG(flagsLightTexture, VERTEX_NORMAL);
//
//    auto geometryPostProcessing = graphics->CreateGeometryPrimitive(Quad, DirectX::XMFLOAT4(DirectX::Colors::Yellow));
//    auto geoCubeOuter = graphics->CreateGeometryPrimitive(CubeSkybox, XMFLOAT4(Colors::Red));
//    auto geoCubeInner = graphics->CreateGeometryPrimitive(Cube, XMFLOAT4(Colors::Green));
//    auto geoSphere = graphics->CreateGeometryPrimitive(Plane, XMFLOAT4(Colors::Yellow));
//
//    std::string shaderFilePath1 = "../../../src/Render/Shaders/LightColor.hlsl";
//    std::string csoDestinationPath1 = "../../../src/Render/CsoCompiled/LightColor";
//
//    std::string shaderFilePath3 = "../../../src/Render/Shaders/LightColor.hlsl";
//    std::string csoDestinationPath3 = "../../../src/Render/CsoCompiled/LightColor";
//
//    auto shaderLightColor = graphics->CreateShaderCustom(shaderFilePath3, csoDestinationPath3, flagsLightColor);
//
//    std::string shaderFilePath2 = "../../../src/Render/Shaders/LightTexture.hlsl";
//    std::string csoDestinationPath2 = "../../../src/Render/CsoCompiled/LightTexture";
//    auto shaderLightTexture = graphics->CreateShaderCustom(shaderFilePath2, csoDestinationPath2, flagsLightTexture, D3D12_CULL_MODE_BACK);
//
//    auto shaderLightSkyBox = graphics->CreateShaderCustom(shaderFilePath1, csoDestinationPath1, flagsLightColor, D3D12_CULL_MODE_NONE);
//
//    graphics->InitializeGraphicsResourcesStart();
//
//    int flags = 0;
//    SET_FLAG(flags, VERTEX_POSITION);
//    SET_FLAG(flags, VERTEX_UV);
//
//    auto meshPostProcessing = graphics->CreateMeshCustom(geometryPostProcessing.resource, flags);
//
//    // Cr�ation des meshes
//    auto meshCubeOuter = graphics->CreateMeshCustom(geoCubeOuter.resource, flagsLightColor);
//    auto meshCubeInner = graphics->CreateMeshCustom(geoCubeInner.resource, flagsLightColor);
//    auto meshSphere = graphics->CreateMeshCustom(geoSphere.resource, flagsLightColor);
//
//    //meshCubeInner.resource->AddGeometry(geoCubeInner.resource, XMFLOAT3(1.0f, 1.0f, 1.0f));
//    std::string texturePath = "../../../src/Render/Textures/texture.dds";
//    std::string texturePath2 = "../../../src/Render/Textures/cottage_diffuse.dds";
//    auto texture = graphics->CreateTexture(texturePath);
//    auto texture2 = graphics->CreateTexture(texturePath2);
//
//    graphics->InitializeGraphicsResourcesEnd();
//
//    // Cr�ation des mat�riaux
//    auto materialCubeOuter = graphics->CreateMaterial(shaderLightSkyBox.resource);
//    //materialCubeOuter.resource->SetTexture(texture2.resource);
//
//    auto materialCubeInner = graphics->CreateMaterial(shaderLightColor.resource);
//    auto materialCubeInner2 = graphics->CreateMaterial(shaderLightColor.resource);
//    auto materialSphere = graphics->CreateMaterial(shaderLightTexture.resource);
//    materialSphere.resource->SetTexture(texture.resource);
//
//
//    // PERSPECTIVE 
//    
//    //GCVEC3 cameraPosition(0.0f, -10.0f, 5.0f);
//    //GCVEC3 cameraTarget(0.0f, 0.0f, 0.0f);
//    //GCVEC3 cameraUp(0.0f, 1.0f, 0.0f);
//    //float fieldOfView = 0.25f * XM_PI; // Exemple de champ de vision pour la perspective
//    //float aspectRatio = window->AspectRatio(); // Assurez-vous que `window` est défini
//    //float nearZ = 1.0f;
//    //float farZ = 1000.0f;
//    //float viewWidth = 20.0f;
//    //float viewHeight = 20.0f;
//
//    //GCMATRIX storedProjectionMatrix, storedViewMatrix;
//    //graphics->CreateViewProjConstantBuffer(
//    //    cameraPosition,
//    //    cameraTarget,
//    //    cameraUp,
//    //    fieldOfView,
//    //    aspectRatio,
//    //    nearZ,
//    //    farZ,
//    //    viewWidth,
//    //    viewHeight,
//    //    PERSPECTIVE,
//    //    storedProjectionMatrix,
//    //    storedViewMatrix
//    //);
//
//    // ORTHO
//
//    // Définir les paramètres de la caméra
//    GCVEC3 cameraPosition(0.0f, 0.0f, -10.0f);
//    GCVEC3 cameraTarget(0.0f, 0.0f, 0.0f);
//    GCVEC3 cameraUp(0.0f, 1.0f, 0.0f);
//
//    // Paramètres pour la projection orthographique
//    float viewWidth = 20.0f;   // Largeur de la vue (modifiez selon vos besoins)
//    float viewHeight = 20.0f;  // Hauteur de la vue (modifiez selon vos besoins)
//    float nearZ = 1.0f;        // Plan proche
//    float farZ = 1000.0f;      // Plan éloigné
//
//    // Matrices pour stocker les résultats
//    GCMATRIX storedProjectionMatrix, storedViewMatrix;
//
//    // Appeler la fonction pour créer les matrices de vue et de projection
//    graphics->CreateViewProjConstantBuffer(
//        cameraPosition,
//        cameraTarget,
//        cameraUp,
//        0.0f,                 // Champ de vision non utilisé pour l'orthographique
//        0.0f,                 // Rapport d'aspect non utilisé pour l'orthographique
//        nearZ,
//        farZ,
//        viewWidth,
//        viewHeight,
//        ORTHOGRAPHIC,         // Type de projection
//        storedProjectionMatrix,
//        storedViewMatrix
//    );
//
//    XMMATRIX worldMatrixCubeOuter = XMMatrixScaling(20.0f, 20.0f, 20.0f) * XMMatrixTranslation(0.0f, -3.0f, 0.0f); // Cube externe (skybox)
//    XMMATRIX worldMatrixCubeInner = XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixTranslation(0.0f, 10.0f, 0.0f); // Cube interne centr�
//    XMMATRIX worldMatrixCubeInner2 = XMMatrixScaling(15.0f, 15.0f, 15.0f) * XMMatrixTranslation(0.0f, 0.0f, -1.0f); // Cube interne centr�
//    XMMATRIX worldMatrixSphere = XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixTranslation(3.0f, 5.0f, -2.0f); // Sph�re d�plac�e dans le cube interne
//
//    GCMATRIX worldCubeInner = GCUtils::XMMATRIXToGCMATRIX(worldMatrixCubeInner);
//    GCMATRIX worldCubeInner2 = GCUtils::XMMATRIXToGCMATRIX(worldMatrixCubeInner2);
//
//
//    auto startTime = std::chrono::steady_clock::now();
//    auto lastFrameTime = startTime;
//
//    while (true) {
//        auto currentTime = std::chrono::steady_clock::now();
//        float elapsedTime = std::chrono::duration<float>(currentTime - startTime).count();
//
//
//        float rotationSpeed = 1.0f;
//        float angle = rotationSpeed * elapsedTime;
//        XMMATRIX rotationMatrix = XMMatrixRotationY(angle);
//
//
//        GCMATERIALPROPERTIES materialProperties;
//        materialProperties.ambientLightColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//        materialProperties.ambient = DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
//        materialProperties.diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//        materialProperties.specular = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
//        materialProperties.shininess = 128.0f;
//        graphics->UpdateMaterialProperties(materialCubeInner.resource, materialProperties);
//
//        GCMATERIALPROPERTIES materialProperties2;
//        materialProperties2.ambientLightColor = DirectX::XMFLOAT4(0.5f, 0.2f, 1.0f, 1.0f);
//        materialProperties2.ambient = DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
//        materialProperties2.diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//        materialProperties2.specular = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//        materialProperties2.shininess = 128.0f;
//        graphics->UpdateMaterialProperties(materialCubeInner2.resource, materialProperties2);
//
//        std::vector<GCLIGHT> lights;
//
//        //GCLIGHT light2;
//        //light2.position = DirectX::XMFLOAT3(0.0f, 0.0f, 8.0f); // Position en 2D (x, y, 0)
//        //light2.direction = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f); // Direction vers le bas en 2D
//        //light2.color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f); // Couleur de la lumière
//        //light2.spotAngle = 30.0f; // Angle du spot en degrés (par exemple, 30 degrés)
//        //light2.lightIntensity = 5.2f;
//        //light2.lightType = LIGHT_TYPE_SPOT; // Type de lumière spot
//
//        GCLIGHT light1;
//        light1.position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f); // Position en 2D n'est pas utilisée pour les lumières directionnelles
//        light1.direction = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f); // Direction vers le bas en 2D
//        light1.color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f); // Couleur de la lumière
//        light1.spotAngle = 0.0f; // Non utilisé pour les lumières directionnelles
//        light1.lightIntensity = 1.0f; // Intensité de la lumière
//        light1.lightType = LIGHT_TYPE_DIRECTIONAL; // Type de lumière directionnelle
//
//        GCLIGHT pointLight;
//        pointLight.position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f); // Position en 2D (x, y, 0)
//        pointLight.direction = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f); // Direction vers le bas en 2D
//        pointLight.color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f); // Couleur de la lumière
//        pointLight.spotAngle = 0.0f; // Angle du spot si applicable
//        pointLight.lightIntensity = 20.4f;
//        pointLight.lightType = 2; // Type de lumière ponctuelle
//
//        //lights.push_back(directionalLight);
//        //lights.push_back(pointLight);
//        lights.push_back(light1);
//
//        graphics->UpdateLights(lights);
//
//        graphics->StartFrame();
//
//
//        graphics->UpdateWorldConstantBuffer(materialCubeInner.resource, worldCubeInner2, 2.0f);
//        graphics->GetRender()->DrawObject(meshSphere.resource, materialCubeInner.resource, true);
//
//        //graphics->UpdateWorldConstantBuffer(materialCubeInner2.resource, worldCubeInner, 2.0f);
//        //graphics->GetRender()->DrawObject(meshSphere.resource, materialCubeInner2.resource, true);
//
//        graphics->EndFrame();
//        window->Run(graphics->GetRender());
//    }
//
//    delete graphics;
//
//    return 0;
//}