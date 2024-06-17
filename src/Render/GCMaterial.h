#pragma once
class GCMaterial
{
public:
	GCMaterial();
	~GCMaterial();

	void Initialize(GCShader* m_pShader,GCTexture* m_pTexture);

	GCTexture* GetTexture() const { return m_pTexture; }
	GCShader* GetShader() const { return m_pShader; }

	void addObjectCB(GCShaderUploadBufferBase* pObjectCB);
	
	void UpdateConstantBuffer(const GCSHADERCB& objectData, GCShaderUploadBufferBase* uploadBufferInstance);

	// Object
	std::vector<GCShaderUploadBufferBase*> GetObjectCBData() 
	{
		return m_vpObjectCB;
	}

	int m_count = 0;
private:

	GCShader* m_pShader;
	GCTexture* m_pTexture;

	// Properties
	DirectX::XMFLOAT4 m_diffuseAlbedo;
	DirectX::XMFLOAT3 m_fresnelR0;
	float m_roughness;
	// Many cb for multiple object drawing
	std::vector<GCShaderUploadBufferBase*> m_vpObjectCB;
};

