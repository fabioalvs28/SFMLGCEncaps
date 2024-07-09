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
private:
	GCRender* m_pRender;

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
	GCShaderUploadBufferBase* pObjectCB = new GCShaderUploadBuffer<ShaderTypeConstantBuffer>(m_pRender->Getmd3dDevice(), 1, true);
	m_pCbObjectInstances.push_back(pObjectCB);
}