#pragma once



class GCShader
{
public:
	GCShader();
	~GCShader();

	void CompileShader();

	ID3DBlob* GetmvsByteCode();
	ID3DBlob* GetmpsByteCode();

	void RootSign();
	void Pso();

	ID3D12RootSignature* GetRootSign();
	ID3D12PipelineState* GetPso(bool alpha);

	void Initialize(GCRender* pRender, const std::string& filePath, const std::string& csoDestinationPath, int& flagEnabledBits);
	void Render();

	int GetFlagEnabledBits() const { return m_flagEnabledBits; }

	ID3DBlob* CompileShaderBase(const std::wstring& filename, const D3D_SHADER_MACRO* defines, const std::string& entrypoint, const std::string& target);

	// Precompile by CSO
	void SaveShaderToFile(ID3DBlob* shaderBlob, const std::wstring& filename);
	ID3DBlob* LoadShaderFromFile(const std::wstring& filename);
	void PreCompile(const std::string& filePath, const std::string& csoDestinationPath);

	void Load();
	void SetRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE rtt);
	GCRender* m_pRender;
	D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandle = nullptr;
protected:

	ID3D12RootSignature* m_RootSignature;
	ID3D12PipelineState* m_PSO1;
	ID3D12PipelineState* m_PSO2;

	std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputLayout;

	ID3DBlob* m_vsByteCode;
	ID3DBlob* m_psByteCode;

	// Var used in Compile Shader override func
	std::wstring m_vsCsoPath;
	std::wstring m_psCsoPath;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc2;
	//int m_type;
	int m_flagEnabledBits;
};
