#include "../../src/render/framework.h"

struct GCTest : GCSHADERCB {
	DirectX::XMFLOAT4X4 world; // Matrice du monde
	//DirectX::XMFLOAT4 color;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	/*
	//Window* window = new Window(hInstance);
	//window->Initialize();

	GCGraphics* graphics = new GCGraphics();
	graphics->Initialize(window,600,400);

	//graphics->GetPrimitiveFactory()->Initialize();
	////graphics->GetModelParserFactory()->Initialize();

	// Geometry (Resource)
	GCGeometry* geo = graphics->GetPrimitiveFactory()->BuildGeometryColor(L"cube", DirectX::XMFLOAT4(DirectX::Colors::White));
	GCGeometry* geo1 = graphics->GetModelParserFactory()->BuildModelTexture("../../../src/Render/monkeyUv.obj", obj);
	GCShader* shader1 = graphics->CreateShaderColor();
	GCShader* shader2 = graphics->CreateShaderTexture();

	/////// Create Render Resources
	//graphics->GetRender()->ResetCommandList(); // Reset Command List Before Resources Creation


	//shader1->Load();
	//shader2->Load();


	//// Mesh
	//GCMesh* mesh = graphics->CreateMesh(geo);
	//GCMesh* mesh1 = graphics->CreateMesh(geo1);
	////GCShader* shaderCustom = graphics->CreateShaderCustom(customShaderFile);


	//std::string texturePath = "../../../src/Render/Textures/texture.dds";
	//GCTexture* tex1 = graphics->CreateTexture(texturePath);
	//GCMaterial* material = graphics->CreateMaterial(shader1, nullptr);
	//GCMaterial* material2 = graphics->CreateMaterial(shader2, tex1);

	//graphics->GetRender()->CloseCommandList(); // Close and Execute after creation
	//graphics->GetRender()->ExecuteCommandList();// Close and Execute after creation

	///////

	//// SET CAMERA

	DirectX::XMVECTOR cameraPosition = DirectX::XMVectorSet(0.0f, -5.0f, 0.1f, 1.0f);
	DirectX::XMVECTOR targetPosition = DirectX::XMVectorZero();
	DirectX::XMVECTOR upVector = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);


	//DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, window->AspectRatio(), 1.0f, 1000.0f);
	//DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(cameraPosition, targetPosition, upVector);

	//// Transposez les matrices
	//DirectX::XMMATRIX transposedProjectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);
	//DirectX::XMMATRIX transposedViewMatrix = DirectX::XMMatrixTranspose(viewMatrix);

	//// Stockez les matrices transpos�es dans des XMFLOAT4X4
	//DirectX::XMFLOAT4X4 storedProjectionMatrix;
	//DirectX::XMFLOAT4X4 storedViewMatrix;

	//DirectX::XMStoreFloat4x4(&storedProjectionMatrix, transposedProjectionMatrix);
	//DirectX::XMStoreFloat4x4(&storedViewMatrix, transposedViewMatrix);

	//// SET CAMERA


	//DirectX::XMFLOAT4X4 I(
	//	0.5f, 0.0f, 0.0f, 0.0f,
	//	0.0f, 0.5f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 1.0f, 0.0f,
	//	3.0f, 0.0f, 0.0f, 1.0f);

	//DirectX::XMFLOAT4X4 transposedWorld;
	//DirectX::XMStoreFloat4x4(&transposedWorld, DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&I)));



	graphics->GetRender()->PrepareDraw();


	//problème? actuel -> moteur doit forcément updatebuffers des materials dans le même ordre qu'ils vont être dessiné,même si les matrices n'ont pas changé
	//material2->addObjectCB(graphics->GetRender()->CreateObjectCB<GCWORLDCB>());
	//material2->addCameraCB(graphics->GetRender()->CreateCameraCB());
	//graphics->GetRender()->UpdateBuffers(material2, MathHelper::Identity4x4(), storedProjectionMatrix, storedViewMatrix);
	//graphics->GetRender()->DrawOneObject(mesh1, material2);
	//material->addObjectCB(graphics->GetRender()->CreateObjectCB<GCWORLDCB>());
	//material->addCameraCB(graphics->GetRender()->CreateCameraCB());
	//graphics->GetRender()->UpdateBuffers(material, transposedWorld, storedProjectionMatrix, storedViewMatrix);
	//graphics->GetRender()->DrawOneObject(mesh, material);
	//material->addObjectCB(graphics->GetRender()->CreateObjectCB<GCWORLDCB>());
	//material->addCameraCB(graphics->GetRender()->CreateCameraCB());
	//graphics->GetRender()->UpdateBuffers(material, MathHelper::Identity4x4(), storedProjectionMatrix, storedViewMatrix);
	//graphics->GetRender()->DrawOneObject(mesh, material);

	material2->addObjectCB(graphics->GetRender()->CreateObjectCB<GCWORLDCB>());
	material2->addCameraCB(graphics->GetRender()->CreateCameraCB());
	graphics->GetRender()->UpdateBuffers(material2, MathHelper::Identity4x4(), storedProjectionMatrix, storedViewMatrix);
	graphics->GetRender()->DrawOneObjectPixel(mesh1, material2,300,200,projectionMatrix,viewMatrix);
	material->addObjectCB(graphics->GetRender()->CreateObjectCB<GCWORLDCB>());
	material->addCameraCB(graphics->GetRender()->CreateCameraCB());
	graphics->GetRender()->UpdateBuffers(material, transposedWorld, storedProjectionMatrix, storedViewMatrix);
	graphics->GetRender()->DrawOneObjectPixel(mesh, material,100,100, projectionMatrix, viewMatrix);
	material->addObjectCB(graphics->GetRender()->CreateObjectCB<GCWORLDCB>());
	material->addCameraCB(graphics->GetRender()->CreateCameraCB());
	graphics->GetRender()->UpdateBuffers(material, transposedWorld, storedProjectionMatrix, storedViewMatrix);
	graphics->GetRender()->DrawOneObjectPixel(mesh, material, 500, 300, projectionMatrix, viewMatrix);

	graphics->GetRender()->PostDraw();

	//Resets the count of
	for (int i = 0; i < graphics->GetMaterials().size(); i++)
		graphics->GetMaterials()[i]->m_count = 0;

	*/
	//// Loop Again < |||| >

	//graphics->GetRender()->PrepareDraw();


	////material->Update(&cb);
	////material->Draw(mesh1);

	////material->Draw(&cb, mesh1);

	////graphics->GetRender()->DrawObject(mesh1, material2);

	////problème? actuel -> moteur doit forcément updatebuffers des materials dans le même ordre qu'ils vont être dessiné,même si les matrices n'ont pas changé
	//material2->addObjectCB(graphics->GetRender()->CreateObjectCB<GCWORLDCB>());
	//material2->addCameraCB(graphics->GetRender()->CreateCameraCB());
	//graphics->GetRender()->UpdateBuffers(material2, MathHelper::Identity4x4(), storedProjectionMatrix, storedViewMatrix);
	//graphics->GetRender()->DrawOneObject(mesh1, material2);

	//material->addObjectCB(graphics->GetRender()->CreateObjectCB<GCWORLDCB>());
	//material->addCameraCB(graphics->GetRender()->CreateCameraCB());
	//graphics->GetRender()->UpdateBuffers(material, transposedWorld, storedProjectionMatrix, storedViewMatrix);
	//graphics->GetRender()->DrawOneObject(mesh, material);

	//material->addObjectCB(graphics->GetRender()->CreateObjectCB<GCWORLDCB>());
	//material->addCameraCB(graphics->GetRender()->CreateCameraCB());
	//graphics->GetRender()->UpdateBuffers(material, MathHelper::Identity4x4(), storedProjectionMatrix, storedViewMatrix);
	//graphics->GetRender()->DrawOneObject(mesh, material);


	//graphics->GetRender()->PostDraw();

	////Resets the count of 
	//for (int i = 0; i < graphics->GetMaterials().size(); i++)
	//	graphics->GetMaterials()[i]->m_count = 0;


	////// Loop Again < |||| >

	////graphics->GetRender()->PrepareDraw();

	////graphics->GetRender()->PostDraw();

	//// *


	//window->Run(graphics->GetRender());

	GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();

	profiler.InitializeConsole();

	Window* window = new Window(hInstance);
	window->Initialize();

	GCGraphics* graphics = new GCGraphics();
	graphics->Initialize(window, 800, 800);

	graphics->GetPrimitiveFactory()->Initialize();
	//graphics->GetModelParserFactory()->Initialize();

	// Geometry (Resource)
	GCGeometry* geo = graphics->GetPrimitiveFactory()->BuildGeometryColor(L"cube", DirectX::XMFLOAT4(DirectX::Colors::Red));
	DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.2f); // Rouge (1.0f, 0.0f, 0.0f) avec alpha 0.5 (50% d'opacité)
	GCGeometry* geo1 = graphics->GetPrimitiveFactory()->BuildGeometryColor(L"cube", color);
	GCGeometry* geo2 = graphics->GetModelParserFactory()->BuildModelTexture("../../../src/Render/monkeyUv.obj", obj);

	GCShader* shader1 = graphics->CreateShaderColor();

	std::string shaderFilePath = "../../../src/Render/Shaders/customTest.hlsl";
	std::string csoDestinationPath = "../../../src/Render/CsoCompiled/custom";
	//GCShader* shader2 = graphics->CreateShaderCustom(shaderFilePath, csoDestinationPath, STEnum::texture);
	GCShader* shader2 = graphics->CreateShaderTexture();

	///// Create Render Resources
	graphics->GetRender()->ResetCommandList(); // Reset Command List Before Resources Creation


	shader1->Load();
	shader2->Load();


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

	DirectX::XMVECTOR cameraPosition = DirectX::XMVectorSet(0.0f, -10.0f, 0.1f, 1.0f);
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

	// Réinitialisation des constant buffers des matériaux
	for (auto& material : graphics->GetMaterials())
	{
		for (auto& cbObject : material->GetObjectCBData())
		{
			cbObject->m_isUsed = false;
		}
	}

	// Préparation du rendu
	graphics->GetRender()->PrepareDraw();

	// Première objet
	GCTest worldData;
	worldData.world = I;
	//worldData.color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);




	graphics->UpdateViewProjConstantBuffer(storedProjectionMatrix, storedViewMatrix);
	GCWORLDCB test = graphics->ToPixel<GCWORLDCB>(400, 400, storedProjectionMatrix, storedViewMatrix);
	graphics->UpdateCustomCBObject<GCTest>(material2, test);
	graphics->GetRender()->DrawObject(mesh2, material2);
	//graphics->GetRender()->DrawObject(mesh2, material2);

	//// Deuxième objet
	//graphics->UpdateCustomCBObject<GCTest>(material2, worldData);
	//graphics->UpdateViewProjConstantBuffer(storedProjectionMatrix, storedViewMatrix);
	//graphics->GetRender()->DrawObject(mesh2, material2);

	//// Troisième objet
	//graphics->UpdateCustomCBObject<GCTest>(material2, worldData);
	//graphics->UpdateViewProjConstantBuffer(storedProjectionMatrix, storedViewMatrix);
	//graphics->GetRender()->DrawObject(mesh2, material2);

	// Post traitement du rendu
	graphics->GetRender()->PostDraw();

	// Réinitialisation des compteurs CB pour tous les matériaux
	for (int i = 0; i < graphics->GetMaterials().size(); i++) {
		graphics->GetMaterials()[i]->ResetCBCount();
	}

	// Vérification des CB inutilisés dans les matériaux
	// CHECK FOR REMOVE CB BUFFER IN MATERIALS 
	for (auto& material : graphics->GetMaterials())
	{
		for (auto& cbObject : material->GetObjectCBData())
		{
			if (cbObject->m_isUsed)
				cbObject->m_framesSinceLastUse = 0;
			if (!cbObject->m_isUsed)
			{
				cbObject->m_framesSinceLastUse++;
				if (cbObject->m_framesSinceLastUse > 180)
				{
					profiler.LogInfo("Constant buffer inutilisé trouvé dans le matériau : ");
				}
			}
		}
	}

	// Log de la taille des CB dans material2
	profiler.LogInfo(std::to_string(material2->GetObjectCBData().size()));


	//// DEUXIEME FRAME  

	//// Réinitialisation des constant buffers des matériaux
	//for (auto& material : graphics->GetMaterials())
	//{
	//	for (auto& cbObject : material->GetObjectCBData())
	//	{
	//		cbObject->m_isUsed = false;
	//	}
	//}

	//// Préparation du rendu
	//graphics->GetRender()->PrepareDraw();

	//// Premier objet
	//graphics->UpdateCustomCBObject<GCTest>(material2, worldData);
	//graphics->UpdateViewProjConstantBuffer(storedProjectionMatrix, storedViewMatrix);
	//graphics->GetRender()->DrawObject(mesh2, material2);

	//// Deuxième objet
	//graphics->UpdateCustomCBObject<GCTest>(material2, worldData);
	//graphics->UpdateViewProjConstantBuffer(storedProjectionMatrix, storedViewMatrix);
	//graphics->GetRender()->DrawObject(mesh2, material2);

	//// Post traitement du rendu
	//graphics->GetRender()->PostDraw();

	//for (auto& material : graphics->GetMaterials())
	//{
	//	for (auto& cbObject : material->GetObjectCBData())
	//	{
	//		if (cbObject->m_isUsed)
	//			cbObject->m_framesSinceLastUse = 0;
	//		if (!cbObject->m_isUsed)
	//		{
	//			cbObject->m_framesSinceLastUse++;
	//			if (cbObject->m_framesSinceLastUse > 180)
	//			{
	//				profiler.LogInfo("Constant buffer inutilisé trouvé dans le matériau : ");
	//			}
	//		}
	//	}
	//}

	//// Log de la taille des CB dans material2
	//profiler.LogInfo(std::to_string(material2->GetObjectCBData().size()));


	window->Run(graphics->GetRender());
}

