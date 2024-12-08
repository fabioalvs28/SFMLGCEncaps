#pragma once

class GCGraphics;

struct GCGeometry;
class GCMesh;
class GCTexture;
class GCMaterial;

class GCSprite
{
	GCGeometry* mpGeometry;
	GCTexture* mpTexture;
	GCMesh* mpMesh;
	GCMaterial* mpMaterial;

	int mWidth, mHeight;

	DirectX::XMMATRIX mWorldMatrix;

public:
	int mX, mY;
	void LoadTexture(const char* filePath, GCGraphics* pGraphics);
	void SetPosition(float x, float y);
	void Draw(GCGraphics* pGraphics);
};

