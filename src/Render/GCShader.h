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

	GC_GRAPHICS_ERROR Initialize(GCRenderContext* pRender, const std::string& filePath, const std::string& csoDestinationPath, int& flagEnabledBits, D3D12_CULL_MODE cullMode = D3D12_CULL_MODE_BACK, int flagRootParameters = DEFAULT_ROOT_PARAMETER_FLAG);

	int GetFlagEnabledBits() const { return m_flagEnabledBits; }

	ID3DBlob* CompileShaderBase(const std::wstring& filename, const D3D_SHADER_MACRO* defines, const std::string& entrypoint, const std::string& target);

	// Precompile by CSO
	void SaveShaderToFile(ID3DBlob* shaderBlob, const std::wstring& filename);
	ID3DBlob* LoadShaderFromFile(const std::wstring& filename);
	void PreCompile(const std::string& filePath, const std::string& csoDestinationPath);

	GC_GRAPHICS_ERROR Load();

	void SetRenderTarget(ID3D12Resource* rtt);
	void SetRenderTargetFormats(DXGI_FORMAT format, int i);

	GCRenderContext* m_pRender;

	//
	ID3D12Resource* m_pRtt = nullptr;
private:
	// Initialize var
	D3D12_CULL_MODE m_cullMode;

	ID3D12RootSignature* m_RootSignature;

	ID3D12PipelineState* m_pPsoAlpha;
	ID3D12PipelineState* m_pPsoNoAlpha;

	std::vector<D3D12_INPUT_ELEMENT_DESC> m_InputLayout;

	ID3DBlob* m_vsByteCode;
	ID3DBlob* m_psByteCode;
	
	//Path
	std::wstring m_vsCsoPath;
	std::wstring m_psCsoPath;

	int m_flagEnabledBits;
	int m_flagRootParameters;

	//Pso 
	DXGI_FORMAT m_rtvFormats[4]; 
};


