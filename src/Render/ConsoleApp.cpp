#include "framework.h"

struct GCTest : GCSHADERCB {
	DirectX::XMFLOAT4X4 world; // Matrice du monde
	DirectX::XMFLOAT4 color;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{

	GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();

	profiler.InitializeConsole();

	Window* window = new Window(hInstance);
	window->Initialize();

	GCGraphics* graphics = new GCGraphics();
	graphics->Initialize(window);

	graphics->GetPrimitiveFactory()->Initialize();
	//graphics->GetModelParserFactory()->Initialize();

	// Geometry (Resource)
	GCGeometry* geo = graphics->GetPrimitiveFactory()->BuildGeometryColor(L"cube", DirectX::XMFLOAT4(DirectX::Colors::Red));
	DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.2f); // Rouge (1.0f, 0.0f, 0.0f) avec alpha 0.5 (50% d'opacité)
	GCGeometry* geo1 = graphics->GetPrimitiveFactory()->BuildGeometryColor(L"cube", color);
	GCGeometry* geo2 = graphics->GetModelParserFactory()->BuildObjTexture("../../../src/Render/monkeyUv.obj");

	GCShader* shader1 = graphics->CreateShaderColor();

	std::string shaderFilePath = "../../../src/Render/Shaders/customTest.hlsl";
	std::string csoDestinationPath = "../../../src/Render/CsoCompiled/custom";
	GCShader* shader2 = graphics->CreateShaderCustom(shaderFilePath, csoDestinationPath, STEnum::texture);


	//GCShader * shader3 = graphics->CreateShaderCustom(shaderFilePath, csoDestinationPath);

	///// Create Render Resources
	graphics->GetRender()->ResetCommandList(); // Reset Command List Before Resources Creation


	shader1->Load();
	shader2->Load();
	//shader3->Load();


	// Mesh
	GCMesh* mesh = graphics->CreateMesh(geo);
	GCMesh* mesh1 = graphics->CreateMesh(geo1);
	GCMesh* mesh2 = graphics->CreateMesh(geo2);
	



	std::string texturePath = "../../../src/Render/Textures/texture.dds";
	GCTexture* tex1 = graphics->CreateTexture(texturePath);
	GCMaterial* material = graphics->CreateMaterial(shader1, nullptr);
	GCMaterial* material2 = graphics->CreateMaterial(shader2, tex1);

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

	// Stockez les matrices transpos�es dans des XMFLOAT4X4
	DirectX::XMFLOAT4X4 storedProjectionMatrix;
	DirectX::XMFLOAT4X4 storedViewMatrix;

	DirectX::XMStoreFloat4x4(&storedProjectionMatrix, transposedProjectionMatrix);
	DirectX::XMStoreFloat4x4(&storedViewMatrix, transposedViewMatrix);

	// SET CAMERA


	DirectX::XMFLOAT4X4 I(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		1.5f, 0.0f, 0.0f, 1.0f);

	DirectX::XMFLOAT4X4 transposedWorld;
	DirectX::XMStoreFloat4x4(&transposedWorld, DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&I)));



	graphics->GetRender()->PrepareDraw();


	//material->Update(&cb);
	//material->Draw(mesh1);

	//material->Draw(&cb, mesh1);






	////Yellow 
	//graphics->UpdateCustomCBObject<GCTest>(material, worldData);
	//graphics->UpdateViewProjConstantBuffer(storedProjectionMatrix, storedViewMatrix);
	//graphics->GetRender()->DrawObject(mesh1, material);

	//////Red
	//graphics->UpdateWorldConstantBuffer(material, transposedWorld);
	//graphics->UpdateViewProjConstantBuffer(storedProjectionMatrix, storedViewMatrix);
	//graphics->GetRender()->DrawObject(mesh, material);




	//////Red
	//graphics->UpdateWorldConstantBuffer(material, transposedWorld);
	//graphics->UpdateViewProjConstantBuffer(storedProjectionMatrix, storedViewMatrix);
	//graphics->GetRender()->DrawObject(mesh, material);

	// Singe

	GCTest worldData;
	worldData.world = MathHelper::Identity4x4();
	worldData.color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	//graphics->UpdateWorldConstantBuffer(material2, MathHelper::Identity4x4());

	////Yellow 
	//graphics->UpdateCustomCBObject<GCTest>(material, worldData);
	//graphics->UpdateViewProjConstantBuffer(storedProjectionMatrix, storedViewMatrix);
	//graphics->GetRender()->DrawObject(mesh1, material);






	graphics->GetRender()->PostDraw();

	//Resets the count of material
	
	for (int i = 0; i < graphics->GetMaterials().size(); i++)
		graphics->GetMaterials()[i]->ResetCBCount();


	//// Loop Again < |||| >

	//graphics->GetRender()->PrepareDraw();

	//	//Yellow 
	//graphics->UpdateCustomCBObject<GCTest>(material, worldData);
	//graphics->UpdateViewProjConstantBuffer(storedProjectionMatrix, storedViewMatrix);
	//graphics->GetRender()->DrawObject(mesh1, material);







	// Au début de votre boucle while, initialiser le timer
	auto start = std::chrono::high_resolution_clock::now();

	while (true) {
		GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();

		graphics->GetRender()->PrepareDraw();

		GCTest worldData;
		worldData.world = MathHelper::Identity4x4();
		worldData.color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

		graphics->UpdateCustomCBObject<GCTest>(material2, worldData);
		graphics->UpdateViewProjConstantBuffer(storedProjectionMatrix, storedViewMatrix);
		graphics->GetRender()->DrawObject(mesh2, material2);

		graphics->UpdateWorldConstantBuffer(material2, MathHelper::Identity4x4());
		graphics->UpdateViewProjConstantBuffer(storedProjectionMatrix, storedViewMatrix);
		graphics->GetRender()->DrawObject(mesh2, material2);

		//profiler.LogInfo(std::to_string(material2->GetCount()));

		graphics->GetRender()->PostDraw();

		for (int i = 0; i < graphics->GetMaterials().size(); i++) {
			graphics->GetMaterials()[i]->ResetCBCount();
		}

		profiler.LogInfo(std::to_string(material2->GetObjectCBData().size()));

		// Vérifier si 3 secondes se sont écoulées
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = duration_cast<std::chrono::seconds>(end - start).count();

		//if (duration >= 3) {
		//	profiler.LogInfo("Il faut enlever les constant buffers inutilisés du material");
		//	// Réinitialiser le timer
		//	start = std::chrono::high_resolution_clock::now();
		//}
	}


	//graphics->GetRender()->PostDraw();

	// *




}

