#include "pch.h"

struct GCTest : GCSHADERCB {
	DirectX::XMFLOAT4X4 world; 
	DirectX::XMFLOAT4 color;
};




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	//GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();
	//profiler.InitializeConsole();

	//Window* window = new Window(hInstance);
	//window->Initialize();

	//GCGraphics* graphics = new GCGraphics();
	//graphics->Initialize(window, 1920, 1080);

	//// Geometry (Resource)
	//auto geo = graphics->CreateGeometryPrimitiveTexture("plane");
	//auto geo1 = graphics->CreateGeometryPrimitiveTexture("cube");
	//auto geo2 = graphics->CreateGeometryModelParserTexture("../../../src/Render/monkeyUv.obj", obj);

	//std::string shaderFilePath = "../../../src/Render/Shaders/customTest.hlsl";
	//std::string csoDestinationPath = "../../../src/Render/CsoCompiled/custom";

	//auto shader1 = graphics->CreateShaderColor();
	////GCShader* shader2 = graphics->CreateShaderCustom(shaderFilePath, csoDestinationPath, flagsTexture);


	//auto shader2 = graphics->CreateShaderTexture();

	//graphics->InitializeGraphicsResourcesStart();

	//// Mesh (Resource)
	//auto mesh = graphics->CreateMesh(geo.resource);


	////GCMesh* mesh1 = graphics->CreateMesh(geo1);

	//auto mesh2 = graphics->CreateMesh(geo2.resource);

	//// Texture (Resource)
	//std::string texturePath = "../../../src/Render/Textures/texture.dds";
	//std::string texturePath2 = "../../../src/Render/Textures/cottage_diffuse.dds";
	//auto texture = graphics->CreateTexture(texturePath);
	//auto texture3 = graphics->CreateTexture(texturePath);





	//graphics->InitializeGraphicsResourcesEnd();




	//graphics->RemoveTexture(texture3.resource);
	//graphics->RemoveMesh(mesh.resource);




	//graphics->InitializeGraphicsResourcesStart();
	//auto texture8 = graphics->CreateTexture(texturePath2);
	//graphics->InitializeGraphicsResourcesEnd();



	//// Material (Resource)
	//auto material = graphics->CreateMaterial(shader1.resource);
	//auto material2 = graphics->CreateMaterial(shader2.resource);
	//material2.resource->SetTexture(texture.resource);

	//auto material3 = graphics->CreateMaterial(shader2.resource);
	//material3.resource->SetTexture(texture8.resource);

	//// PERSPECTIVE 
	//
	//DirectX::XMVECTOR cameraPosition = DirectX::XMVectorSet(0.0f, -10.0f, 5.0f, 1.0f);
	//DirectX::XMVECTOR targetPosition = DirectX::XMVectorZero();
	//DirectX::XMVECTOR upVector = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);


	//DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, window->AspectRatio(), 1.0f, 1000.0f);
	//DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(cameraPosition, targetPosition, upVector);

	//DirectX::XMMATRIX transposedProjectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);
	//DirectX::XMMATRIX transposedViewMatrix = DirectX::XMMatrixTranspose(viewMatrix);

	//DirectX::XMFLOAT4X4 storedProjectionMatrix;
	//DirectX::XMFLOAT4X4 storedViewMatrix;

	//DirectX::XMStoreFloat4x4(&storedProjectionMatrix, transposedProjectionMatrix);
	//DirectX::XMStoreFloat4x4(&storedViewMatrix, transposedViewMatrix);
	//// ***********


	////// SET CAMERA
	////	DirectX::XMVECTOR cameraPosition = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f);  // Position de la caméra
	////	DirectX::XMVECTOR targetPosition = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f); // Point visé par la caméra
	////	DirectX::XMVECTOR upVector = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);       // Vecteur "up" de la caméra
	////
	////	// Projection orthographique
	////	float viewWidth = 10.0f;    // Largeur de la vue
	////	float viewHeight = 10.0f;   // Hauteur de la vue
	////	float nearZ = 0.1f;         // Plan proche
	////	float farZ = 100.0f;        // Plan éloigné
	////
	////	DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ);
	////	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(cameraPosition, targetPosition, upVector);
	////
	////	DirectX::XMMATRIX transposedProjectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);
	////	DirectX::XMMATRIX transposedViewMatrix = DirectX::XMMatrixTranspose(viewMatrix);
	////
	////	DirectX::XMFLOAT4X4 storedProjectionMatrix;
	////	DirectX::XMFLOAT4X4 storedViewMatrix;
	////
	////	DirectX::XMStoreFloat4x4(&storedProjectionMatrix, transposedProjectionMatrix);
	////	DirectX::XMStoreFloat4x4(&storedViewMatrix, transposedViewMatrix);
	////
	////	// ****************

	//
	///*float left = -1.0f;
	//float right = 1.0f;
	//float bottom = -1.0f;
	//float top = 1.0f;

	//DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixOrthographicOffCenterLH(left, right, bottom, top, 0.0f, 1.0f);
	//DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixIdentity();

	//DirectX::XMMATRIX transposedProjectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);
	//DirectX::XMMATRIX transposedViewMatrix = DirectX::XMMatrixTranspose(viewMatrix);

	//DirectX::XMFLOAT4X4 storedProjectionMatrix;
	//DirectX::XMFLOAT4X4 storedViewMatrix;

	//DirectX::XMStoreFloat4x4(&storedProjectionMatrix, transposedProjectionMatrix);
	//DirectX::XMStoreFloat4x4(&storedViewMatrix, transposedViewMatrix);*/


	//// SET WORLD

	//DirectX::XMFLOAT4X4 I(
	//	0.5f, 0.0f, 0.0f, 0.0f,
	//	0.0f, 0.5f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 1.0f, 0.0f,
	//	1.5f, 0.0f, 0.0f, 1.0f);

	//DirectX::XMFLOAT4X4 transposedWorld;
	//DirectX::XMStoreFloat4x4(&transposedWorld, DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&I)));


	//// ***********

	//graphics->StartFrame();

	//// DRAW -> ONE FRAME
	//graphics->UpdateViewProjConstantBuffer(storedProjectionMatrix, storedViewMatrix);

	//DirectX::XMFLOAT4X4 world = graphics->ToPixel(400, 400, storedProjectionMatrix, storedViewMatrix);
	//DirectX::XMFLOAT4X4 world2 = graphics->ToPixel(600, 600, storedProjectionMatrix, storedViewMatrix);

	//GCTest test;
	//test.world = world2;
	//test.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f);


	////DirectX::XMMATRIX identityMatrix = DirectX::XMMatrixIdentity();
	////DirectX::XMStoreFloat4x4(&test.world, identityMatrix);
	////test.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f);

	////graphics->UpdateCustomCBObject<GCTest>(material2.resource, test);
	////graphics->GetRender()->DrawObject(mesh2.resource, material2.resource);

	//graphics->UpdateWorldConstantBuffer(material3.resource, world);
	//graphics->GetRender()->DrawObject(mesh2.resource, material3.resource);

	//graphics->UpdateCustomCBObject<GCTest>(material2.resource, test);
	//graphics->GetRender()->DrawObject(mesh2.resource, material2.resource);

	////graphics->UpdateWorldConstantBuffer(material3.resource, transposedWorld);
	////graphics->GetRender()->DrawObject(mesh2.resource, material3.resource);

	////graphics->UpdateCustomCBObject<GCTest>(material, worldData);
	////graphics->GetRender()->DrawObject(mesh, material);

	////profiler.LogInfo(std::to_string(material2->GetObjectCBData().size()));


	//graphics->EndFrame();
	//// ********************

	//window->Run(graphics->GetRender());
}

