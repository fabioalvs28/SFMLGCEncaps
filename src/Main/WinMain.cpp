#pragma region 2DDEMO

//2D DEMO

#include "pch.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) {
    //Initialize graphics resource
    GCGraphicsLogger& profiler = GCGraphicsLogger::GetInstance();
    profiler.InitializeConsole();

    Window* window = new Window(hInstance);
    window->Initialize(L"hello world");

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

    std::string shaderTextFilePath = "../../../res/Shaders/textTexture.hlsl";
    std::string csoDestinationTextPath = "../../../res/CsoCompiled/textTexture";
    auto shaderText = graphics->CreateShaderCustom(shaderTextFilePath, csoDestinationTextPath, flagsColorTexture);

    auto shaderTexture = graphics->CreateShaderTexture();

    auto shaderColor = graphics->CreateShaderColor();

    //Init graphics ressources
    graphics->InitializeGraphicsResourcesStart();

    //Mesh creation
    auto meshBackground = graphics->CreateMeshTexture(geoPlane.resource);
    auto meshPlaneAlphabet = graphics->CreateMeshCustom(geoPlaneAlphabet, flagsColorTexture);

    std::string texturePath = "../../../res/Textures/Textures/texture.dds";
    std::string texturePath2 = "../../../res/Textures/TimesFont.dds";
    auto texture = graphics->CreateTexture(texturePath);
    auto textureText = graphics->CreateTexture(texturePath2);

    graphics->InitializeGraphicsResourcesEnd();

    auto materialText = graphics->CreateMaterial(shaderText.resource);
    materialText.resource->SetTexture(textureText.resource);

    auto materialBackground = graphics->CreateMaterial(shaderTexture.resource);
    materialBackground.resource->SetTexture(texture.resource);

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
    DirectX::XMMATRIX worldMatrixText = DirectX::XMMatrixScaling(2.0f, 2.0f, 2.0f) * DirectX::XMMatrixTranslation(-9.0f, 0.0f, 0.0f); // Texte
    DirectX::XMMATRIX worldMatrixBackground = DirectX::XMMatrixScaling(500.0f, 500.0f, 0.0f) * DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f); // Plane

    auto startTime = std::chrono::steady_clock::now();
    auto lastFrameTime = startTime;

    float fireworkTriggerTime[5] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    int textureSelect = 0;
    while (window->IsClosed() == false) {
        auto currentTime = std::chrono::steady_clock::now();
        float elapsedTime = std::chrono::duration<float>(currentTime - startTime).count();

        float deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime).count();
        lastFrameTime = currentTime;

        graphics->StartFrame();
        graphics->UpdateViewProjConstantBuffer(storedProjectionMatrix, storedViewMatrix);

        graphics->UpdateWorldConstantBuffer(materialText.resource, worldMatrixText);
        graphics->GetRender()->DrawObject(meshPlaneAlphabet.resource, materialText.resource, true);


        graphics->EndFrame();
        window->Run(graphics->GetRender());
    }

    return 0;
}
#pragma endregion