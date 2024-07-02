#pragma once
class GCMaterial
{
public:
	GCMaterial();
	~GCMaterial();

	bool Initialize(GCShader* pShader);
	/**
	*  Applies a texture to the material
	 * @brief
	 *
	 *
	 * @param Texture
	 * @return True if the texture was set successfullyfalse otherwise.
	 */
	bool SetTexture(GCTexture* pTexture);

	GCTexture* GetTexture() const { return m_pTexture; }
	GCShader* GetShader() const { return m_pShader; }

	template<typename ShaderTypeConstantBuffer>
	void CreateCBObject();

	void UpdateConstantBuffer(const GCSHADERCB& objectData, GCShaderUploadBufferBase* uploadBufferInstance);

	// Check texture and apply if exist
	bool UpdateTexture();

	// Object
	std::vector<GCShaderUploadBufferBase*> GetObjectCBData() { return m_vpObjectCB; }

	// Draw Count
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