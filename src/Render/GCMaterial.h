#pragma once
class GCMaterial
{
public:
	GCMaterial();
	~GCMaterial();
private:
	GCTexture* m_pTexture;
	DirectX::XMFLOAT4 m_diffuseAlbedo;
	DirectX::XMFLOAT3 m_fresnelR0;
	float m_roughness;
};

