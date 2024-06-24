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
	ID3D12PipelineState* GetPso();

	void Initialize(GCRender* pRender, const std::string& filePath, const std::string& csoDestinationPath, int& flagEnabledBits);
	void Render();

	int GetFlagEnabledBits() const { return m_flagEnabledBits; }

	ID3DBlob* CompileShaderBase(const std::wstring& filename, const D3D_SHADER_MACRO* defines, const std::string& entrypoint, const std::string& target);

	// Precompile by CSO
	void SaveShaderToFile(ID3DBlob* shaderBlob, const std::wstring& filename);
	ID3DBlob* LoadShaderFromFile(const std::wstring& filename);
	void PreCompile(const std::string& filePath, const std::string& csoDestinationPath);

	void Load();

	GCRender* m_pRender;
protected:

	ID3D12RootSignature* m_RootSignature;
	ID3D12PipelineState* m_PSO;

	std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputLayout;

	ID3DBlob* m_vsByteCode;
	ID3DBlob* m_psByteCode;

	// Var used in Compile Shader override func
	std::wstring m_vsCsoPath;
	std::wstring m_psCsoPath;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;

	//int m_type;
	int m_flagEnabledBits;
};
