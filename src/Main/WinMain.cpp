#pragma region 2DDEMO

//2D DEMO

#include "pch.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) {
    //Initialize graphics resource
    GCGraphicsLogger& profiler = GCGraphicsLogger::GetInstance();
    profiler.InitializeConsole();

    Window* window = new Window(hInstance);
    window->Initialize();

    GCGraphics* graphics = new GCGraphics();
    graphics->Initialize(window, 1920, 1080);

    graphics->GetRender()->Set2DMode();

    int flagsTexture = 0;
    GC_SET_FLAG(flagsTexture, GC_VERTEX_POSITION);
    GC_SET_FLAG(flagsTexture, GC_VERTEX_UV);

    int flagsLightColor = 0;
    GC_SET_FLAG(flagsLightColor, GC_VERTEX_POSITION);
    GC_SET_FLAG(flagsLightColor, GC_VERTEX_COLOR);
    GC_SET_FLAG(flagsLightColor, GC_VERTEX_NORMAL);

    int flagsLightTexture = 0;
    GC_SET_FLAG(flagsLightTexture, GC_VERTEX_POSITION);
    GC_SET_FLAG(flagsLightTexture, GC_VERTEX_UV);
    GC_SET_FLAG(flagsLightTexture, GC_VERTEX_NORMAL);

    int flagsColorTexture = 0;
    GC_SET_FLAG(flagsColorTexture, GC_VERTEX_POSITION);
    GC_SET_FLAG(flagsColorTexture, GC_VERTEX_COLOR);
    GC_SET_FLAG(flagsColorTexture, GC_VERTEX_UV);

    //Create geometries

    auto geoPlane = graphics->CreateGeometryPrimitive(Plane, DirectX::XMFLOAT4(DirectX::Colors::DarkBlue));
    auto housePlane = graphics->CreateGeometryPrimitive(Plane, DirectX::XMFLOAT4(DirectX::Colors::White));
    graphics->m_pFontGeometryLoader->Initialize("../../../res/Fonts/LetterUV.txt");

    DirectX::XMFLOAT4 textColor = DirectX::XMFLOAT4( DirectX::Colors::Red);
    auto geoPlaneAlphabet = graphics->m_pFontGeometryLoader->CreateText("azertyu\biopqs\bdf\tghjklm\twxcvbn\nAZERTYUIOPQSDFGHJKLMWXCVBN\n0123456789\n, ; :!? . / !$ *  ^  % & () = \n ", textColor);

    //Load shaders
    std::string shaderFilePath1 = "../../../res/Shaders/LightColor.hlsl";
    std::string csoDestinationPath1 = "../../../res/CsoCompiled/LightColor";
    auto shaderLightColor = graphics->CreateShaderCustom(shaderFilePath1, csoDestinationPath1, flagsLightColor, D3D12_CULL_MODE_BACK);

    std::string shaderFilePath2 = "../../../res/Shaders/LightTexture.hlsl";
    std::string csoDestinationPath2 = "../../../res/CsoCompiled/LightTexture";
    auto shaderLightTexture = graphics->CreateShaderCustom(shaderFilePath2, csoDestinationPath2, flagsLightTexture, D3D12_CULL_MODE_NONE);

    std::string shaderfilePath3 = "../../../res/Shaders/texture.hlsl";
    std::string csoDestinationPath3 = "../../../res/CsoCompiled/texture";
    auto shaderTextureCullNone = graphics->CreateShaderCustom(shaderfilePath3, csoDestinationPath3, flagsTexture, D3D12_CULL_MODE_NONE);

    auto shaderLightSkyBox = graphics->CreateShaderCustom(shaderFilePath1, csoDestinationPath1, flagsLightColor, D3D12_CULL_MODE_NONE);

    std::string shaderTextFilePath = "../../../res/Shaders/textTexture.hlsl";
    std::string csoDestinationTextPath = "../../../res/CsoCompiled/textTexture";
    auto shaderText = graphics->CreateShaderCustom(shaderTextFilePath, csoDestinationTextPath, flagsColorTexture);

    auto shaderTexture = graphics->CreateShaderTexture();

    auto shaderColor = graphics->CreateShaderColor();
    graphics->InitializeGraphicsResourcesStart();

    auto meshBackground = graphics->CreateMeshCustom(geoPlane.resource, flagsLightColor);
    auto meshPlaneAlphabet = graphics->CreateMeshCustom(geoPlaneAlphabet, flagsColorTexture);

    std::string texturePath = "../../../res/Textures/texture.dds";
    std::string texturePath2 = "../../../res/Textures/TimesFont.dds";
    auto texture = graphics->CreateTexture(texturePath);
    auto textureText = graphics->CreateTexture(texturePath2);


    graphics->InitializeGraphicsResourcesEnd();

    auto materialText = graphics->CreateMaterial(shaderText.resource);
    materialText.resource->SetTexture(textureText.resource);

    auto materialBackground = graphics->CreateMaterial(shaderLightColor.resource);

    //Setting up camera
    DirectX::XMFLOAT3 cameraPosition(0.0f, 0.0f, -10.0f);
    DirectX::XMFLOAT3 cameraTarget(0.0f, 0.0f, 0.0f);
    DirectX::XMFLOAT3 cameraUp(0.0f, 1.0f, 0.0f);

    //Orthographic projection
    float viewWidth = 20.0f;   
    float viewHeight = 20.0f; 
    float nearZ = 1.0f;       
    float farZ = 1000.0f;      

    DirectX::XMMATRIX storedProjectionMatrix, storedViewMatrix;

    //Create view and projection matrixes
    graphics->CreateViewProjConstantBuffer(
        cameraPosition,
        cameraTarget,
        cameraUp,
        0.0f,                 //FOV not used for orthographic
        0.0f,                 //Aspect ration not used for orthographic
        nearZ,
        farZ,
        viewWidth,
        viewHeight,
        ORTHOGRAPHIC,         //Projection type
        storedProjectionMatrix,
        storedViewMatrix
    );

    //Setting up matrixes
    DirectX::XMMATRIX worldMatrixText = DirectX::XMMatrixScaling(2.0f, 2.0f, 2.0f) * DirectX::XMMatrixTranslation(-9.0f, 0.0f, 0.0f); // Cube interne centré
    DirectX::XMMATRIX worldMatrixBackground = DirectX::XMMatrixScaling(100.0f, 100.0f, 100.0f) * DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f); // Cube interne centré

    auto startTime = std::chrono::steady_clock::now();
    auto lastFrameTime = startTime;

    float fireworkTriggerTime[5] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    int textureSelect = 0;
    while (true) {
        auto currentTime = std::chrono::steady_clock::now();
        float elapsedTime = std::chrono::duration<float>(currentTime - startTime).count();

        float deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime).count();
        lastFrameTime = currentTime;

        graphics->StartFrame();
        graphics->UpdateViewProjConstantBuffer(storedProjectionMatrix, storedViewMatrix);

        graphics->UpdateWorldConstantBuffer(materialBackground.resource, worldMatrixBackground);
        graphics->GetRender()->DrawObject(meshBackground.resource, materialBackground.resource, true);

        graphics->UpdateWorldConstantBuffer(materialText.resource, worldMatrixText);
        graphics->GetRender()->DrawObject(meshPlaneAlphabet.resource, materialText.resource, true);


        graphics->EndFrame();
        window->Run(graphics->GetRender());
    }

    return 0;
}
#pragma endregion