#pragma once

struct GCSHADERCB {
};

struct GCWORLDCB : GCSHADERCB {
	DirectX::XMFLOAT4X4 world; // Matrice du monde
};

struct GCLIGHTANDWORLD : GCSHADERCB {
	DirectX::XMFLOAT4X4 world; // Matrice du monde
	DirectX::XMFLOAT4X4 light; // Matrice du monde
	DirectX::XMFLOAT4X4 normal;
};

//
struct GCCAMERACB : GCSHADERCB {
	DirectX::XMFLOAT4X4 view; // Matrice de vue
	DirectX::XMFLOAT4X4 proj; // Matrice de projection
};

class GCShader
{
public:
	GCShader();
	~GCShader();

	virtual void CompileShader();

	ID3DBlob* GetmvsByteCode();
	ID3DBlob* GetmpsByteCode();

	void RootSign();
	void Pso();

	ID3D12RootSignature* GetRootSign();
	ID3D12PipelineState* GetPso();

	void Initialize(GCRender* pRender, const std::string& filePath, const std::string& csoDestinationPath, int type);
	void Render();

	int GetType() const { return m_type; }



	ID3DBlob* CompileShaderBase(const std::wstring& filename, const D3D_SHADER_MACRO* defines, const std::string& entrypoint, const std::string& target);


	// Precompile by CSO
	void SaveShaderToFile(ID3DBlob* shaderBlob, const std::wstring& filename);
	ID3DBlob* LoadShaderFromFile(const std::wstring& filename);
	void PreCompile(const std::string& filePath, const std::string& csoDestinationPath);


	// 
	template<typename ShaderType>
	void Load();


	// Object
	
	GCShaderUploadBufferBase* GetObjectCBData() {
		return m_pObjectCB;
	}

	// Camera
	GCShaderUploadBufferBase* GetCameraCBData() {
		return m_pCameraCB;
	}

	// Update Shader Constant Buffer Data
	void UpdateConstantBufferData(const GCSHADERCB& objectData, GCShaderUploadBufferBase* uploadBufferInstance)
	{
		uploadBufferInstance->CopyData(0, objectData);
	}

private:
	ID3D12RootSignature* m_RootSignature = nullptr;
	ID3D12PipelineState* m_PSO = nullptr;
	int m_type;

	GCShaderUploadBufferBase* m_pObjectCB;
	GCShaderUploadBufferBase* m_pCameraCB;
protected:


	std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputLayout;

	ID3DBlob* m_vsByteCode = nullptr;
	ID3DBlob* m_psByteCode = nullptr;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;

	GCRender* m_pRender;
};

template<typename ShaderTypeConstantBuffer>
void GCShader::Load() {
	CompileShader();
	RootSign();
	Pso();

	// Load in GPU CB Struct
	m_pObjectCB = new GCShaderUploadBuffer<ShaderTypeConstantBuffer>(m_pRender->Getmd3dDevice(), 1, true);
	m_pCameraCB = new GCShaderUploadBuffer<GCCAMERACB>(m_pRender->Getmd3dDevice(), 1, true);
}
