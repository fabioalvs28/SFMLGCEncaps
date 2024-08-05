// 3D DEMO

#include "pch.h"

using namespace DirectX;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) 
{
	//Graphic resources initialization
	GCGraphicsLogger& profiler = GCGraphicsLogger::GetInstance();
	profiler.InitializeConsole();
	
	Window* window = new Window(hInstance);
	window->Initialize();
	
	GCGraphics* graphics = new GCGraphics();
	graphics->Initialize(window, 1920, 1080);
	
	//Set flags
	int flagsLightColor = 0;
	SET_FLAG(flagsLightColor, VERTEX_POSITION);
	SET_FLAG(flagsLightColor, VERTEX_COLOR);
	SET_FLAG(flagsLightColor, VERTEX_NORMAL);

	int flagsTexture = 0;
	SET_FLAG(flagsTexture, VERTEX_POSITION);
	SET_FLAG(flagsTexture, VERTEX_UV);

	int flagsLightTexture = 0;
	SET_FLAG(flagsLightTexture, VERTEX_POSITION);
	SET_FLAG(flagsLightTexture, VERTEX_UV);
	SET_FLAG(flagsLightTexture, VERTEX_NORMAL);
	
	//Create geometries
	auto geoSphere = graphics->CreateGeometryPrimitive(Sphere, XMFLOAT4(Colors::Green));
	auto geoCubeSkybox = graphics->CreateGeometryPrimitive(CubeSkybox, XMFLOAT4(Colors::Blue));
	auto geoHouse = graphics->CreateGeometryModelParser("../../../src/Render/tiny_house.obj", XMFLOAT4(Colors::White), obj);
    auto geoPlane = graphics->CreateGeometryPrimitive(Plane, XMFLOAT4(Colors::White));
	//Load shaders
    std::string shaderFilePath1 = "../../../src/Render/Shaders/LightColor.hlsl";
    std::string csoDestinationPath1 = "../../../src/Render/CsoCompiled/LightColor";
    auto shaderLightColor = graphics->CreateShaderCustom(shaderFilePath1, csoDestinationPath1, flagsLightColor, D3D12_CULL_MODE_BACK);

    std::string shaderFilePath2 = "../../../src/Render/Shaders/LightTexture.hlsl";
    std::string csoDestinationPath2 = "../../../src/Render/CsoCompiled/LightTexture";
    auto shaderLightTexture = graphics->CreateShaderCustom(shaderFilePath2, csoDestinationPath2, flagsLightTexture, D3D12_CULL_MODE_BACK);

    std::string shaderfilePath3 = "../../../src/Render/Shaders/texture.hlsl";
    std::string csoDestinationPath3 = "../../../src/Render/CsoCompiled/texture";
    auto shaderTextureCullNone = graphics->CreateShaderCustom(shaderfilePath3, csoDestinationPath3, flagsTexture, D3D12_CULL_MODE_NONE);

    auto shaderTexture = graphics->CreateShaderTexture();

	//Initialize graphics resources
	graphics->InitializeGraphicsResourcesStart();

    //Create meshes
	auto meshGround = graphics->CreateMeshCustom(geoSphere.resource, flagsLightTexture);
	auto meshCubeSkybox = graphics->CreateMeshCustom(geoSphere.resource, flagsTexture);
    auto meshHouse = graphics->CreateMeshCustom(geoHouse.resource, flagsLightTexture);
    

    //Load textures
    std::string texturePath = "../../../src/Render/Textures/test_house_diff.dds";
    auto textureHouse = graphics->CreateTexture(texturePath);

    texturePath = "../../../src/Render/Textures/texture.dds";
    auto textureGround = graphics->CreateTexture(texturePath);

    texturePath = "../../../src/Render/Textures/night_sky.dds";
    auto textureSky = graphics->CreateTexture(texturePath);

    std::vector<GCTexture*> fireworkTextures;
    fireworkTextures.push_back(graphics->CreateTexture("../../../src/Render/Textures/red_firework.dds").resource);
    fireworkTextures.push_back(graphics->CreateTexture("../../../src/Render/Textures/green_firework.dds").resource);
    fireworkTextures.push_back(graphics->CreateTexture("../../../src/Render/Textures/blue_firework.dds").resource);

    //Particle systems
    std::vector<GCParticleSystem*> allFireworks;
    std::vector<GCMaterial*> fireworksMaterials;
    std::vector<GCMesh*> fireworksMeshes;
    for (int i = 0; i <= 5.0f; i++)
    {
        fireworksMeshes.push_back(graphics->CreateMeshCustom(geoPlane.resource, flagsLightTexture).resource);

        GCParticleSystem* fireworkParticleSystem = new GCParticleSystem();
        fireworkParticleSystem->InitializeCircle(30, true, 5.0f, fireworksMeshes[i]);
        allFireworks.push_back(fireworkParticleSystem);

        fireworksMaterials.push_back(graphics->CreateMaterial(shaderTextureCullNone.resource).resource);
        fireworksMaterials[i]->SetTexture(fireworkTextures[rand() % 3]);
    }

    graphics->InitializeGraphicsResourcesEnd();

    //Create materials
	auto materialGround = graphics->CreateMaterial(shaderLightTexture.resource);
    materialGround.resource->SetTexture(textureGround.resource);

	auto materialHouse = graphics->CreateMaterial(shaderLightTexture.resource);
	materialHouse.resource->SetTexture(textureHouse.resource);

    auto materialSkybox = graphics->CreateMaterial(shaderTextureCullNone.resource);
    materialSkybox.resource->SetTexture(textureSky.resource);

    //Camera variables
	GCVEC3 cameraPosition(0.0f, -10.0f, 5.0f);
    GCVEC3 cameraTarget(0.0f, 0.0f, 0.0f);
    GCVEC3 cameraUp(0.0f, 1.0f, 0.0f);

    float fieldOfView = 0.25f * XM_PI;
    float aspectRatio = window->AspectRatio();
    float nearZ = 1.0f;
    float farZ = 1000.0f;

    GCMATRIX storedProjectionMatrix, storedViewMatrix;
    graphics->CreateViewProjConstantBuffer(
        cameraPosition,
        cameraTarget,
        cameraUp,
        fieldOfView,
        aspectRatio,
        nearZ,
        farZ,
        20.0f,
        20.0f,
        PERSPECTIVE,
        storedProjectionMatrix,
        storedViewMatrix
    );

    //Particles matrixes
    std::vector<XMFLOAT3> fireworksPositions;
    std::vector<XMMATRIX> fireworksMatrixes;
    std::vector<GCMATRIX> worldFireworks;
    for (int i = 0; i < allFireworks.size(); i++)
    {
        XMFLOAT3 fireworksPos;
        fireworksPos.x = static_cast<float>(rand()) / RAND_MAX * 20.0f - 10.0f;
        fireworksPos.y = (static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f) + 20.0f;
        fireworksPos.z = static_cast<float>(rand()) / RAND_MAX * 5.0f - 2.5f;
        fireworksPositions.push_back(fireworksPos);

        fireworksMatrixes.push_back(XMMatrixScaling(10.0f, 10.0f, 10.0f) * XMMatrixTranslation(fireworksPositions[i].x, fireworksPositions[i].y, fireworksPositions[i].z));
        worldFireworks.push_back(GCUtils::XMMATRIXToGCMATRIX(fireworksMatrixes[i]));
    }

    //Setting up matrixes
    XMMATRIX worldMatrixCubeSkybox = XMMatrixScaling(1500.0f, 1500.0f, 1500.0f) * XMMatrixTranslation(0.0f, 5.0f, -30.0f);
    XMMATRIX worldMatrixGround = XMMatrixScaling(50.0f, 50.0f, 50.0f) * XMMatrixTranslation(0.0f, 5.0f, -30.0f);
    XMMATRIX worldMatrixHouse = XMMatrixScaling(8.0f, 8.0f, 8.0f) * XMMatrixTranslation(10.0f, -6.0f, -5.0f) * XMMatrixRotationY(0.2f) * XMMatrixRotationZ(2.8f);

    GCMATRIX worldCubeSkybox = GCUtils::XMMATRIXToGCMATRIX(worldMatrixCubeSkybox);
    GCMATRIX worldGround = GCUtils::XMMATRIXToGCMATRIX(worldMatrixGround);
    GCMATRIX worldHouse = GCUtils::XMMATRIXToGCMATRIX(worldMatrixHouse);

    auto startTime = std::chrono::steady_clock::now();
    auto lastFrameTime = startTime;
    float fireworkTriggerTime[5] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    int textureSelect = 0;

    while (true) {
        auto currentTime = std::chrono::steady_clock::now();
        float elapsedTime = std::chrono::duration<float>(currentTime - startTime).count();

        float deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime).count();
        lastFrameTime = currentTime;

        
        for (int i = 0; i<allFireworks.size(); i++)
        {
            fireworkTriggerTime[i] += deltaTime;

            if (fireworkTriggerTime[i] >= allFireworks[i]->GetDuration())
            {
                fireworkTriggerTime[i] = 0.0f;

                fireworksPositions[i].x = static_cast<float>(rand()) / RAND_MAX * 20.0f - 10.0f;
                fireworksPositions[i].y = (static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f) + 20.0f;
                fireworksPositions[i].z = static_cast<float>(rand()) / RAND_MAX * 5.0f - 2.5f;

                textureSelect = rand() % 3;
                fireworksMaterials[i]->SetTexture(fireworkTextures[textureSelect]);
            }
            XMMATRIX worldFireWorksBillBoard = GCUtils::CreateBillboardMatrix(XMVectorSet(fireworksPositions[i].x, fireworksPositions[i].y, fireworksPositions[i].z, 1.0f), GCUtils::GCVEC3ToXMVECTOR(cameraPosition), GCUtils::GCVEC3ToXMVECTOR(cameraUp));
            worldFireworks[i] = GCUtils::XMMATRIXToGCMATRIX(XMMatrixTranspose(worldFireWorksBillBoard));
        }

        std::vector<GCLIGHT> lights;

        //start frames
        graphics->StartFrame();
        graphics->UpdateViewProjConstantBuffer(storedProjectionMatrix, storedViewMatrix);
         
        GCLIGHT directionalLight;
        directionalLight.position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
        directionalLight.direction = DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f);
        directionalLight.color = DirectX::XMFLOAT3(0.2f, 0.2f, 1.0f);
        directionalLight.spotAngle = 0.0f;
        directionalLight.lightIntensity = 1.5f;
        directionalLight.lightType = LIGHT_TYPE_DIRECTIONAL;

        lights.push_back(directionalLight);

        graphics->UpdateLights(lights);

        //Draw meshes
        graphics->UpdateWorldConstantBuffer(materialGround.resource, worldGround, 2.0f);
        graphics->GetRender()->DrawObject(meshGround.resource, materialGround.resource, true);

        graphics->UpdateWorldConstantBuffer(materialHouse.resource, worldHouse, 2.0f);
        graphics->GetRender()->DrawObject(meshHouse.resource, materialHouse.resource, true);

        graphics->UpdateWorldConstantBuffer(materialSkybox.resource, worldCubeSkybox, 2.0f);
        graphics->GetRender()->DrawObject(meshCubeSkybox.resource, materialSkybox.resource, true);

        //Draw particle systems
        for (int i =0; i<allFireworks.size(); i++)
        {
            graphics->UpdateWorldConstantBuffer(fireworksMaterials[i], worldFireworks[i], 2.0f);
            allFireworks[i]->UpdateParticleSystem(deltaTime);
            graphics->GetRender()->DrawObject(fireworksMeshes[i], fireworksMaterials[i], true);
        }

        graphics->EndFrame();
        window->Run(graphics->GetRender());
    }

    delete graphics;

    return 0;
}

////2D DEMO
//
//#include "pch.h"
//
//using namespace DirectX;
//
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
//    SET_FLAG(flagsLightColor, VERTEX_POSITION);
//    SET_FLAG(flagsLightColor, VERTEX_COLOR);
//    SET_FLAG(flagsLightColor, VERTEX_NORMAL);
//
//    int flagsLightTexture = 0;
//    SET_FLAG(flagsLightTexture, VERTEX_POSITION);
//    SET_FLAG(flagsLightTexture, VERTEX_UV);
//    SET_FLAG(flagsLightTexture, VERTEX_NORMAL);
//
//    // Création des géométries
//    auto geoPlane = graphics->CreateGeometryPrimitive(Plane, XMFLOAT4(Colors::DarkBlue));
//
//    //graphics->m_pFontGeometryLoader->Initialize("../../../src/Render/Fonts/LetterUV.txt");
//    //auto geoPlaneAlphabet = graphics->m_pFontGeometryLoader->CreateText("Hello");
//
//    // Chargement des shaders personnalisés
//    std::string shaderFilePath1 = "../../../src/Render/Shaders/LightColor.hlsl";
//    std::string csoDestinationPath1 = "../../../src/Render/CsoCompiled/LightColor";
//    auto shaderLightColor = graphics->CreateShaderCustom(shaderFilePath1, csoDestinationPath1, flagsLightColor, D3D12_CULL_MODE_BACK);
//
//    std::string shaderFilePath2 = "../../../src/Render/Shaders/LightTexture.hlsl";
//    std::string csoDestinationPath2 = "../../../src/Render/CsoCompiled/LightTexture";
//    auto shaderLightTexture = graphics->CreateShaderCustom(shaderFilePath2, csoDestinationPath2, flagsLightTexture, D3D12_CULL_MODE_NONE);
//
//    auto shaderLightSkyBox = graphics->CreateShaderCustom(shaderFilePath1, csoDestinationPath1, flagsLightColor, D3D12_CULL_MODE_NONE);
//
//    auto shaderTexture = graphics->CreateShaderTexture();
//
//    auto shaderColor = graphics->CreateShaderColor();
//    graphics->InitializeGraphicsResourcesStart();
//
//    auto meshBackground = graphics->CreateMeshColor(geoPlane.resource);
//
//    //auto meshPlaneAlphabet = graphics->CreateMeshTexture(geoPlaneAlphabet);
//
//    std::string texturePath = "../../../src/Render/Textures/texture.dds";
//    std::string texturePath2 = "../../../src/Render/Textures/alphabet.dds";
//    auto texture = graphics->CreateTexture(texturePath);
//    auto textureText = graphics->CreateTexture(texturePath2);
//
//    std::vector<GCTexture*> fireworkTextures;
//    fireworkTextures.push_back(graphics->CreateTexture("../../../src/Render/Textures/red_firework.dds").resource);
//    fireworkTextures.push_back(graphics->CreateTexture("../../../src/Render/Textures/green_firework.dds").resource);
//    fireworkTextures.push_back(graphics->CreateTexture("../../../src/Render/Textures/blue_firework.dds").resource);
//    
//    //Particle systems
//    std::vector<GCParticleSystem*> allFireworks;
//    std::vector<GCMaterial*> fireworksMaterials;
//    std::vector<GCMesh*> fireworksMeshes;
//    for (int i = 0; i <= 5.0f; i++)
//    {
//        fireworksMeshes.push_back(graphics->CreateMeshCustom(geoPlane.resource, flagsLightTexture).resource);
//    
//        GCParticleSystem* fireworkParticleSystem = new GCParticleSystem();
//        fireworkParticleSystem->InitializeCircle(30, true, 5.0f, fireworksMeshes[i]);
//        allFireworks.push_back(fireworkParticleSystem);
//    
//        fireworksMaterials.push_back(graphics->CreateMaterial(shaderLightTexture.resource).resource);
//        fireworksMaterials[i]->SetTexture(fireworkTextures[rand() % 3]);
//    }
//    
//    graphics->InitializeGraphicsResourcesEnd();
//
//    auto materialText = graphics->CreateMaterial(shaderTexture.resource);
//    materialText.resource->SetTexture(textureText.resource);
//
//    auto materialBackground = graphics->CreateMaterial(shaderColor.resource);
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
//    //Particles matrixes
//    std::vector<XMFLOAT3> fireworksPositions;
//    std::vector<XMMATRIX> fireworksMatrixes;
//    std::vector<GCMATRIX> worldFireworks;
//    for (int i = 0; i < allFireworks.size(); i++)
//    {
//        XMFLOAT3 fireworksPos;
//        fireworksPos.x = static_cast<float>(rand()) / RAND_MAX * 20.0f - 10.0f;
//        fireworksPos.y = (static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f) + 20.0f;
//        fireworksPos.z = static_cast<float>(rand()) / RAND_MAX * 5.0f - 2.5f;
//        fireworksPositions.push_back(fireworksPos);
//
//        fireworksMatrixes.push_back(XMMatrixScaling(1.0f, 1.0f, 1.0f) * XMMatrixTranslation(fireworksPositions[i].x, fireworksPositions[i].y, fireworksPositions[i].z));
//        worldFireworks.push_back(GCUtils::XMMATRIXToGCMATRIX(fireworksMatrixes[i]));
//    }
//
//    //Setting up matrixes
//    XMMATRIX worldMatrixText = XMMatrixScaling(2.0f, 2.0f, 2.0f) * XMMatrixTranslation(-10.0f, 0.0f, 0.0f); // Cube interne centré
//    XMMATRIX worldMatrixBackground = XMMatrixScaling(100.0f, 100.0f, 100.0f) * XMMatrixTranslation(0.0f, 0.0f, 0.0f); // Cube interne centré
//
//    GCMATRIX worldText = GCUtils::XMMATRIXToGCMATRIX(worldMatrixText);
//    GCMATRIX worldBackground = GCUtils::XMMATRIXToGCMATRIX(worldMatrixBackground);
//
//    auto startTime = std::chrono::steady_clock::now();
//    auto lastFrameTime = startTime;
//
//    float fireworkTriggerTime[5] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
//    int textureSelect = 0;
//    while (true) {
//        auto currentTime = std::chrono::steady_clock::now();
//        float elapsedTime = std::chrono::duration<float>(currentTime - startTime).count();
//
//        float deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime).count();
//        lastFrameTime = currentTime;
//        
//        for (int i = 0; i < allFireworks.size(); i++)
//        {
//            fireworkTriggerTime[i] += deltaTime;
//            
//            if (fireworkTriggerTime[i] >= allFireworks[i]->GetDuration())
//            {
//                fireworkTriggerTime[i] = 0.0f;
//            
//                fireworksPositions[i].x = static_cast<float>(rand()) / RAND_MAX * 20.0f - 10.0f;
//                fireworksPositions[i].y = (static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f) + 20.0f;
//                fireworksPositions[i].z = 0.0f;
//            
//                textureSelect = rand() % 3;
//                fireworksMaterials[i]->SetTexture(fireworkTextures[textureSelect]);
//            }
//            XMMATRIX worldFireWorksBillBoard = GCUtils::CreateBillboardMatrix(XMVectorSet(fireworksPositions[i].x, fireworksPositions[i].y, fireworksPositions[i].z, 1.0f), GCUtils::GCVEC3ToXMVECTOR(cameraPosition), GCUtils::GCVEC3ToXMVECTOR(cameraUp));
//            worldFireworks[i] = GCUtils::XMMATRIXToGCMATRIX(XMMatrixTranspose(worldFireWorksBillBoard));
//        }
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
//
//        GCLIGHT pointLight;
//        pointLight.position = DirectX::XMFLOAT3(6.0f, 1.0f, 0.0f); // Position en 2D (x, y, 0)
//        pointLight.direction = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f); // Direction vers le bas en 2D
//        pointLight.color = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f); // Couleur de la lumière
//        pointLight.spotAngle = 0.0f; // Angle du spot si applicable
//        pointLight.lightIntensity = 2.4f;
//        pointLight.lightType = 2; // Type de lumière ponctuelle
//
//        lights.push_back(light2);
//        lights.push_back(pointLight);
//
//        graphics->UpdateLights(lights);
//
//        //graphics->UpdateWorldConstantBuffer(materialText.resource, worldText);
//        //graphics->GetRender()->DrawObject(meshPlaneAlphabet.resource, materialText.resource, true);
//
//        /*graphics->UpdateWorldConstantBuffer(materialBackground.resource, worldBackground);
//        graphics->GetRender()->DrawObject(meshBackground.resource, materialBackground.resource, true);
//        */
//        //Draw particle systems
//        for (int i =0; i<allFireworks.size(); i++)
//        {
//            graphics->UpdateWorldConstantBuffer(fireworksMaterials[i], worldFireworks[i], 2.0f);
//            allFireworks[i]->UpdateParticleSystem(deltaTime);
//            graphics->GetRender()->DrawObject(fireworksMeshes[i], fireworksMaterials[i], true);
//        }
//
//        graphics->EndFrame();
//        window->Run(graphics->GetRender());
//    }
//
//    return 0;
//}