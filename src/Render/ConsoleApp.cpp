#include "framework.h"

struct Test : GCSHADERCB {
	DirectX::XMFLOAT4X4 world; // Matrice du monde
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{

	Window* window = new Window(hInstance);
	window->Initialize();

	GCGraphics* graphics = new GCGraphics();
	graphics->Initialize(window);

	graphics->GetPrimitiveFactory()->Initialize();
	graphics->GetModelParserFactory()->Initialize();

	// Geometry (Resource)
	GCGeometry* geo = graphics->GetPrimitiveFactory()->BuildGeometryColor(L"cube", DirectX::XMFLOAT4(DirectX::Colors::White));
	GCGeometry* geo1 = graphics->GetModelParserFactory()->BuildObjTexture("../../../src/Render/monkeyUv.obj");
	GCShader* shader1 = graphics->CreateShaderColor();
	GCShader* shader2 = graphics->CreateShaderTexture();

	///// Create Render Resources
	graphics->GetRender()->ResetCommandList(); // Reset Command List Before Resources Creation


	shader1->Load<GCWORLDCB>();
	shader2->Load<GCWORLDCB>();


	// Mesh
	GCMesh* mesh = graphics->CreateMesh(geo);
	GCMesh* mesh1 = graphics->CreateMesh(geo1);

	//GCShader* shaderCustom = graphics->CreateShaderCustom(customShaderFile);


	std::string texturePath = "../../../src/Render/Textures/texture.dds";
	GCTexture* tex1 = graphics->CreateTexture(texturePath);
		
	graphics->GetRender()->CloseCommandList(); // Close and Execute after creation
	graphics->GetRender()->ExecuteCommandList();// Close and Execute after creation

	///// 

	// SET CAMERA 

	DirectX::XMVECTOR cameraPosition = DirectX::XMVectorSet(0.0f, -10.0f, 5.0f, 1.0f);
	DirectX::XMVECTOR targetPosition = DirectX::XMVectorZero();
	DirectX::XMVECTOR upVector = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);


	DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, window->AspectRatio(), 1.0f, 1000.0f);
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(cameraPosition, targetPosition, upVector);

	// Transposez les matrices
	DirectX::XMMATRIX transposedProjectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);
	DirectX::XMMATRIX transposedViewMatrix = DirectX::XMMatrixTranspose(viewMatrix);

	// Stockez les matrices transposées dans des XMFLOAT4X4
	DirectX::XMFLOAT4X4 storedProjectionMatrix;
	DirectX::XMFLOAT4X4 storedViewMatrix;

	DirectX::XMStoreFloat4x4(&storedProjectionMatrix, transposedProjectionMatrix);
	DirectX::XMStoreFloat4x4(&storedViewMatrix, transposedViewMatrix);

	// SET CAMERA


	DirectX::XMFLOAT4X4 I(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		3.0f, 0.0f, 0.0f, 1.0f);

	DirectX::XMFLOAT4X4 transposedWorld;
	DirectX::XMStoreFloat4x4(&transposedWorld, DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&I)));



	graphics->GetRender()->PrepareDraw();



	graphics->GetRender()->DrawOneObject(mesh1, shader2, tex1, MathHelper::Identity4x4(), storedProjectionMatrix, storedViewMatrix);
	graphics->GetRender()->DrawOneObject(mesh, shader1, nullptr, transposedWorld, storedProjectionMatrix, storedViewMatrix);


	graphics->GetRender()->PostDraw();

	//// Loop Again < |||| >
	
	//graphics->GetRender()->PrepareDraw();

	//graphics->GetRender()->PostDraw();

	// *


	window->Run(graphics->GetRender());

}

