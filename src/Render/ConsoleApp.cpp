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

	DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.2f);

	// Geometry (Resource)
	GCGeometry* geo = graphics->GetPrimitiveFactory()->BuildGeometryColor(L"cube", DirectX::XMFLOAT4(DirectX::Colors::Red));
	GCGeometry* geo1 = graphics->GetPrimitiveFactory()->BuildGeometryColor(L"cube", color);
	GCGeometry* geo2 = graphics->GetModelParserFactory()->BuildModelTexture("../../../src/Render/monkeyUv.obj", obj);

	std::string shaderFilePath = "../../../src/Render/Shaders/customTest.hlsl";
	std::string csoDestinationPath = "../../../src/Render/CsoCompiled/custom";

	GCShader* shader1 = graphics->CreateShaderColor();
	//GCShader* shader2 = graphics->CreateShaderCustom(shaderFilePath, csoDestinationPath, STEnum::texture);
	GCShader* shader2 = graphics->CreateShaderTexture();


	graphics->GetRender()->ResetCommandList(); // Reset Command List Before Resources Creation

	shader1->Load();
	shader2->Load();

	// Mesh (Resource)
	GCMesh* mesh = graphics->CreateMesh(geo);
	GCMesh* mesh1 = graphics->CreateMesh(geo1);
	GCMesh* mesh2 = graphics->CreateMesh(geo2);

	// Texture (Resource)
	std::string texturePath = "../../../src/Render/Textures/texture.dds";
	GCTexture* texture = graphics->CreateTexture(texturePath);

	// Material (Resource)
	GCMaterial* material = graphics->CreateMaterial(shader1, nullptr);
	GCMaterial* material2 = graphics->CreateMaterial(shader2, texture);

	graphics->GetRender()->CloseCommandList(); // Close and Execute after creation
	graphics->GetRender()->ExecuteCommandList();// Close and Execute after creation


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

	// Première objet
	GCTest worldData;
	worldData.world = I;

	// ***********

	graphics->StartFrame();

	// DRAW -> ONE FRAME
	graphics->UpdateViewProjConstantBuffer(storedProjectionMatrix, storedViewMatrix);
	graphics->GetRender()->DrawObjectPixel(mesh2, material2, 800, 880, projectionMatrix, viewMatrix, graphics);

	graphics->UpdateCustomCBObject<GCTest>(material2, worldData);
	graphics->GetRender()->DrawObject(mesh2, material2);

	profiler.LogInfo(std::to_string(material2->GetObjectCBData().size()));

	graphics->EndFrame();
	// ********************

	window->Run(graphics->GetRender());
}

