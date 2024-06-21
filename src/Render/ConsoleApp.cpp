#include "framework.h"

struct GCTest : GCSHADERCB {
	DirectX::XMFLOAT4X4 world; 
	DirectX::XMFLOAT4 color;
};




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();
	profiler.InitializeConsole();

	Window* window = new Window(hInstance);
	window->Initialize();

	GCGraphics* graphics = new GCGraphics();
	graphics->Initialize(window, 1920, 1080);

	int flagsTexture = 0;
	SET_FLAG(flagsTexture, HAS_POSITION);
	SET_FLAG(flagsTexture, HAS_UV);

	int flagsColor = 0;
	SET_FLAG(flagsColor, HAS_POSITION);
	SET_FLAG(flagsColor, HAS_COLOR);


	// Geometry (Resource)
	//GCGeometry* geo = graphics->CreateGeometryPrimitiveColor("sphere", DirectX::XMFLOAT4(DirectX::Colors::Gray));
	//GCGeometry* geo1 = graphics->CreateGeometryPrimitiveTexture("cube");
	GCGeometry* geo2 = graphics->GetModelParserFactory()->BuildModel("../../../src/Render/monkeyUv.obj", DirectX::XMFLOAT4(DirectX::Colors::Black), obj, flagsColor);

	std::string shaderFilePath = "../../../src/Render/Shaders/customTest.hlsl";
	std::string csoDestinationPath = "../../../src/Render/CsoCompiled/custom";

	GCShader* shader1 = graphics->CreateShaderColor();
	GCShader* shader2 = graphics->CreateShaderCustom(shaderFilePath, csoDestinationPath, flagsTexture);


	//GCShader* shader2 = graphics->CreateShaderTexture();

	graphics->InitializeGraphicsResourcesStart();

	// Mesh (Resource)
	//GCMesh* mesh = graphics->CreateMesh(geo);
	//GCMesh* mesh1 = graphics->CreateMesh(geo1);

	GCMesh* mesh2 = graphics->CreateMesh(geo2);

	// Texture (Resource)
	std::string texturePath = "../../../src/Render/Textures/texture.dds";
	GCTexture* texture = graphics->CreateTexture(texturePath);

	graphics->InitializeGraphicsResourcesEnd();

	// Material (Resource)
	GCMaterial* material = graphics->CreateMaterial(shader1);
	GCMaterial* material2 = graphics->CreateMaterial(shader2);
	material2->SetTexture(texture);


	// SET CAMERA 
	DirectX::XMVECTOR cameraPosition = DirectX::XMVectorSet(0.0f, -10.0f, 5.0f, 1.0f);
	DirectX::XMVECTOR targetPosition = DirectX::XMVectorZero();
	DirectX::XMVECTOR upVector = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);


	DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, window->AspectRatio(), 1.0f, 1000.0f);
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(cameraPosition, targetPosition, upVector);

	DirectX::XMMATRIX transposedProjectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);
	DirectX::XMMATRIX transposedViewMatrix = DirectX::XMMatrixTranspose(viewMatrix);

	DirectX::XMFLOAT4X4 storedProjectionMatrix;
	DirectX::XMFLOAT4X4 storedViewMatrix;

	DirectX::XMStoreFloat4x4(&storedProjectionMatrix, transposedProjectionMatrix);
	DirectX::XMStoreFloat4x4(&storedViewMatrix, transposedViewMatrix);
	// ***********

	// SET WORLD

	DirectX::XMFLOAT4X4 I(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		1.5f, 0.0f, 0.0f, 1.0f);

	DirectX::XMFLOAT4X4 transposedWorld;
	DirectX::XMStoreFloat4x4(&transposedWorld, DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&I)));





	// ***********

	graphics->StartFrame();

	// DRAW -> ONE FRAME
	graphics->UpdateViewProjConstantBuffer(storedProjectionMatrix, storedViewMatrix);

	//GCTest test = graphics->ToPixel<GCTest>(400, 400, storedProjectionMatrix, storedViewMatrix);
	GCTest test;

	DirectX::XMMATRIX identityMatrix = DirectX::XMMatrixIdentity();
	DirectX::XMStoreFloat4x4(&test.world, identityMatrix);
	test.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f);

	graphics->UpdateCustomCBObject<GCTest>(material, test);
	graphics->GetRender()->DrawObject(mesh2, material);

	//graphics->UpdateCustomCBObject<GCTest>(material, worldData);
	//graphics->GetRender()->DrawObject(mesh, material);

	//profiler.LogInfo(std::to_string(material2->GetObjectCBData().size()));


	graphics->EndFrame();
	// ********************

	window->Run(graphics->GetRender());
}

