#pragma once
class GCMaterial
{
public:
	GCMaterial();
	~GCMaterial();

	void Initialize(GCShader* m_pShader, GCTexture* m_pTexture, GCRender* pRender);

	GCTexture* GetTexture() const { return m_pTexture; }
	GCShader* GetShader() const { return m_pShader; }

	template<typename ShaderTypeConstantBuffer>
	void CreateCBObject();

	void UpdateConstantBuffer(const GCSHADERCB& objectData, GCShaderUploadBufferBase* uploadBufferInstance);

	void CheckForRemoveNonUsedCB();


	

	// Object
	std::vector<GCShaderUploadBufferBase*> GetObjectCBData() { return m_vpObjectCB; }

    void IncrementCBCount() { m_iCount++; }
	void ResetCBCount() { m_iCount = 0; }
    int GetCount() const { return m_iCount; }

private:
	GCRender* m_pRender;

	int m_iCount = 0;


	GCShader* m_pShader;
	GCTexture* m_pTexture;

	// Properties
	DirectX::XMFLOAT4 m_diffuseAlbedo;
	DirectX::XMFLOAT3 m_fresnelR0;
	float m_roughness;
	// Many cb for multiple object drawing
	std::vector<GCShaderUploadBufferBase*> m_vpObjectCB;
};

template<typename ShaderTypeConstantBuffer>
void GCMaterial::CreateCBObject()
{
	GCShaderUploadBufferBase* pObjectCB = new GCShaderUploadBuffer<ShaderTypeConstantBuffer>(m_pRender->Getmd3dDevice(), 1, true);
	m_vpObjectCB.push_back(pObjectCB);
}