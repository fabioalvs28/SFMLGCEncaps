#pragma once
class GCMaterial
{
public:
	GCMaterial();
	~GCMaterial();

	GC_GRAPHICS_ERROR Initialize(GCShader* pShader);
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
	void AddCbPerObject();

	void UpdateConstantBuffer(const GCSHADERCB& objectData, GCShaderUploadBufferBase* uploadBufferInstance);

	// Check texture and apply if exist
	bool UpdateTexture();

	// Object
	std::vector<GCShaderUploadBufferBase*> GetCbObjectInstance() { return m_pCbObjectInstances; }
	GCShaderUploadBufferBase* GetCbMaterialPropertiesInstance() { return m_pCbMaterialPropertiesInstance; }

	// Draw Count
    void IncrementCBCount() { m_iCount++; }
	void ResetCBCount() { m_iCount = 0; }
    int GetCount() const { return m_iCount; }

	DirectX::XMFLOAT4 ambientLightColor;
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;
	float shininess;

	// Material Id
	float m_materialId;

private:
	GCRenderContext* m_pRender;

	int m_iCount = 0;

	GCShader* m_pShader;
	GCTexture* m_pTexture;

	// Properties
	GCShaderUploadBufferBase* m_pCbMaterialPropertiesInstance;

	// Many cb for multiple object drawing
	std::vector<GCShaderUploadBufferBase*> m_pCbObjectInstances;
};

template<typename ShaderTypeConstantBuffer>
void GCMaterial::AddCbPerObject()
{
	GCShaderUploadBufferBase* pObjectCB = new GCShaderUploadBuffer<ShaderTypeConstantBuffer>(m_pRender->GetRenderResources()->Getmd3dDevice(), 1, true);
	m_pCbObjectInstances.push_back(pObjectCB);
}