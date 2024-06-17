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
	void addCameraCB(GCShaderUploadBufferBase* pCameraCB);
	
	void UpdateConstantBufferData(const GCSHADERCB& objectData, GCShaderUploadBufferBase* uploadBufferInstance);

	// Object
	std::vector<GCShaderUploadBufferBase*> GetObjectCBData() 
	{
		return m_vpObjectCB;
	}
	// Camera
	std::vector<GCShaderUploadBufferBase*> GetCameraCBData() 
	{
		return m_vpCameraCB;
	}
	int m_count = 0;
private:
	GCTexture* m_pTexture;
	GCShader* m_pShader;
	DirectX::XMFLOAT4 m_diffuseAlbedo;
	DirectX::XMFLOAT3 m_fresnelR0;
	float m_roughness;
	std::vector<GCShaderUploadBufferBase*> m_vpObjectCB;
	std::vector<GCShaderUploadBufferBase*> m_vpCameraCB;
};

