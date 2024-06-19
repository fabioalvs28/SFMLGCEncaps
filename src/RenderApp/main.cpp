#include "../../src/render/framework.h"

struct Test : GCSHADERCB {
	DirectX::XMFLOAT4X4 world; // Matrice du monde
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{

	Window* window = new Window(hInstance);
	window->Initialize();

	GCGraphics* graphics = new GCGraphics();
	graphics->Initialize(window,600,400);

	graphics->GetPrimitiveFactory()->Initialize();
	//graphics->GetModelParserFactory()->Initialize();

	// Geometry (Resource)
	GCGeometry* geo = graphics->GetPrimitiveFactory()->BuildGeometryColor(L"cube", DirectX::XMFLOAT4(DirectX::Colors::White));
	GCGeometry* geo1 = graphics->GetModelParserFactory()->BuildObjTexture("../../../src/Render/monkeyUv.obj");
	GCShader* shader1 = graphics->CreateShaderColor();
	GCShader* shader2 = graphics->CreateShaderTexture();

	///// Create Render Resources
	graphics->GetRender()->ResetCommandList(); // Reset Command List Before Resources Creation


	shader1->Load();
	shader2->Load();


	// Mesh
	GCMesh* mesh = graphics->CreateMesh(geo);
	GCMesh* mesh1 = graphics->CreateMesh(geo1);
	//GCShader* shaderCustom = graphics->CreateShaderCustom(customShaderFile);


	std::string texturePath = "../../../src/Render/Textures/texture.dds";
	GCTexture* tex1 = graphics->CreateTexture(texturePath);
	GCMaterial* material = graphics->CreateMaterial(shader1, nullptr);
	GCMaterial* material2 = graphics->CreateMaterial(shader2, tex1);

	graphics->GetRender()->CloseCommandList(); // Close and Execute after creation
	graphics->GetRender()->ExecuteCommandList();// Close and Execute after creation

	///// 

	// SET CAMERA 

	DirectX::XMVECTOR cameraPosition = DirectX::XMVectorSet(0.0f, -5.0f, 0.1f, 1.0f);
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
		3.0f, 0.0f, 0.0f, 1.0f);

	DirectX::XMFLOAT4X4 transposedWorld;
	DirectX::XMStoreFloat4x4(&transposedWorld, DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&I)));



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


	//// Loop Again < |||| >

	//graphics->GetRender()->PrepareDraw();

	//graphics->GetRender()->PostDraw();

	// *


	window->Run(graphics->GetRender());

}

