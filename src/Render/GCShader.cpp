#include "pch.h"
//test
GCShader::GCShader() 
{
	m_RootSignature = nullptr;
	m_PSO = nullptr;
	m_InputLayout.clear(); 
	m_vsByteCode = nullptr;
	m_psByteCode = nullptr;
	m_vsCsoPath.clear();
	m_psCsoPath.clear();
	ZeroMemory(&psoDesc, sizeof(psoDesc));
	m_pRender = nullptr;
	m_flagEnabledBits = 0;
	m_cullMode = D3D12_CULL_MODE_NONE;
}


GCShader::~GCShader()
{
	SAFE_RELEASE(m_RootSignature);
	SAFE_RELEASE(m_PSO);
	SAFE_RELEASE(m_vsByteCode);
	SAFE_RELEASE(m_psByteCode);

	m_InputLayout.clear();
}

bool GCShader::Initialize(GCRender* pRender, const std::string& filePath, const std::string& csoDestinationPath, int& flagEnabledBits, D3D12_CULL_MODE cullMode)
{
	if (!CHECK_POINTERSNULL("Render ptr is not null", "Render pointer is null", pRender))
		return false;
	if (!CHECK_FILE(filePath, "Shader not found: " + filePath, "Shader file: " + filePath + " loaded successfully"))
		return false;

	std::wstring baseCsoPath(csoDestinationPath.begin(), csoDestinationPath.end());
	m_vsCsoPath = baseCsoPath + L"VS.cso";
	m_psCsoPath = baseCsoPath + L"PS.cso";

	m_cullMode = cullMode;
	m_pRender = pRender;
	m_flagEnabledBits = flagEnabledBits;

	PreCompile(filePath, csoDestinationPath);

	return true;
}
 
void GCShader::CompileShader()
{
	m_vsByteCode = LoadShaderFromFile(m_vsCsoPath);
	m_psByteCode = LoadShaderFromFile(m_psCsoPath);

	m_InputLayout.clear();

	UINT offset = 0; 
	if (HAS_FLAG(m_flagEnabledBits, HAS_POSITION)) {
		m_InputLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offset, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		offset += sizeof(DirectX::XMFLOAT3); 
	}

	if (HAS_FLAG(m_flagEnabledBits, HAS_COLOR)) {
		m_InputLayout.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offset, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		offset += sizeof(DirectX::XMFLOAT4); 
	}

	if (HAS_FLAG(m_flagEnabledBits, HAS_UV)) {
		m_InputLayout.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offset, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		offset += sizeof(DirectX::XMFLOAT2); // Taille des coordonnées de texture
	}

	if (HAS_FLAG(m_flagEnabledBits, HAS_NORMAL)) {
		m_InputLayout.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offset, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		offset += sizeof(DirectX::XMFLOAT3); // Taille des coordonnées de texture
	}
}


void GCShader::RootSign()
{
	// Déclaration des paramètres racine
	CD3DX12_ROOT_PARAMETER slotRootParameter[5];

	slotRootParameter[CBV_SLOT_CB0].InitAsConstantBufferView(0);
	slotRootParameter[CBV_SLOT_CB1].InitAsConstantBufferView(1);
	slotRootParameter[CBV_SLOT_CB2].InitAsConstantBufferView(2);
	slotRootParameter[CBV_SLOT_CB3].InitAsConstantBufferView(3);
	UINT numParameters = 4;


	// Configuration de l'échantillonneur statique
	CD3DX12_STATIC_SAMPLER_DESC staticSample = CD3DX12_STATIC_SAMPLER_DESC(
		0, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP, // addressW
		0.0f, // mipLODBias
		16, // maxAnisotropy
		D3D12_COMPARISON_FUNC_ALWAYS, // comparisonFunc
		D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK, // borderColor
		0.0f, // minLOD
		D3D12_FLOAT32_MAX // maxLOD
	);

	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc;


	if (HAS_FLAG(m_flagEnabledBits, HAS_UV)) {
		CD3DX12_DESCRIPTOR_RANGE srvTable;
		srvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
		slotRootParameter[DESCRIPTOR_TABLE_SLOT_TEXTURE].InitAsDescriptorTable(1, &srvTable);
		numParameters++;
	}



	rootSigDesc.Init(numParameters, slotRootParameter, 1, &staticSample, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);



	// Sérialisation de la signature racine
	ID3DBlob* serializedRootSig = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &serializedRootSig, &errorBlob);

	// Gestion des erreurs de sérialisation
	if (errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}

    // Gestion des erreurs de sérialisation
    if (errorBlob != nullptr) {
        ::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
    }

    // Création de la signature racine
    m_pRender->Getmd3dDevice()->CreateRootSignature(0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature)
    );
}
void GCShader::Pso() 
{
	// Initialize the graphics pipeline state description
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { m_InputLayout.data(), (UINT)m_InputLayout.size() };
	psoDesc.pRootSignature = m_RootSignature;

	psoDesc.VS =
	{
		reinterpret_cast<BYTE*>(m_vsByteCode->GetBufferPointer()),
		m_vsByteCode->GetBufferSize()
	};
	psoDesc.PS =
	{
		reinterpret_cast<BYTE*>(m_psByteCode->GetBufferPointer()),
		m_psByteCode->GetBufferSize()
	};

	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.RasterizerState.CullMode = m_cullMode;

	// Customize the blend state for transparency
	CD3DX12_BLEND_DESC blendDesc(D3D12_DEFAULT);
	for (int i = 0; i < 4; ++i)
	{
		blendDesc.RenderTarget[i].BlendEnable = TRUE;
		blendDesc.RenderTarget[i].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[i].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[i].BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[i].SrcBlendAlpha = D3D12_BLEND_ONE;
		blendDesc.RenderTarget[i].DestBlendAlpha = D3D12_BLEND_ZERO;
		blendDesc.RenderTarget[i].BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	}
	psoDesc.BlendState = blendDesc;

	// Use default depth stencil state
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT); 

	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 4;
	psoDesc.RTVFormats[0] = m_pRender->GetBackBufferFormat();
	psoDesc.RTVFormats[1] = m_pRender->GetBackBufferFormat();
	psoDesc.RTVFormats[2] = m_pRender->GetBackBufferFormat();
	psoDesc.RTVFormats[3] = m_pRender->GetBackBufferFormat();
	psoDesc.SampleDesc.Count = m_pRender->Get4xMsaaState() ? 4 : 1;
	psoDesc.SampleDesc.Quality = m_pRender->Get4xMsaaState() ? (m_pRender->Get4xMsaaQuality() - 1) : 0;
	psoDesc.DSVFormat = m_pRender->GetDepthStencilFormat();

	// Create the graphics pipeline state
	m_pRender->Getmd3dDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_PSO));
}

ID3D12RootSignature* GCShader::GetRootSign() 
{
	return m_RootSignature;
}

ID3D12PipelineState* GCShader::GetPso() 
{
	return m_PSO;
}

ID3DBlob* GCShader::GetmvsByteCode()
{
	return m_vsByteCode;
}

ID3DBlob* GCShader::GetmpsByteCode()
{
	return m_psByteCode;
}

ID3DBlob* GCShader::CompileShaderBase(const std::wstring& filename, const D3D_SHADER_MACRO* defines, const std::string& entrypoint,const std::string& target)
{
	UINT compileFlags = 0;
	#if defined(DEBUG) || defined(_DEBUG)  
		compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	#endif

	HRESULT hr = S_OK;

	ID3DBlob* byteCode = nullptr;
	ID3DBlob* errors;
	hr = D3DCompileFromFile(filename.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, entrypoint.c_str(), target.c_str(), compileFlags, 0, &byteCode, &errors);

	if (errors != nullptr)
	{
		OutputDebugStringA((char*)errors->GetBufferPointer());
	}

	return byteCode;
}

void GCShader::SaveShaderToFile(ID3DBlob* shaderBlob, const std::wstring& filename) 
{
	std::ofstream file(filename, std::ios::binary);
	if (!file.is_open()) 
	{
		throw std::runtime_error("Failed to open file for writing");
	}
	file.write(static_cast<const char*>(shaderBlob->GetBufferPointer()), shaderBlob->GetBufferSize());
	file.close();
}

ID3DBlob* GCShader::LoadShaderFromFile(const std::wstring& filename) 
{
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	if (!file.is_open()) 
	{
		throw std::runtime_error("Failed to open file for reading");
	}
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	ID3DBlob* shaderBlob = nullptr;
	HRESULT hr = D3DCreateBlob(size, &shaderBlob);
	if (FAILED(hr)) 
	{
		throw std::runtime_error("Failed to create shader blob");
	}

	if (!file.read(static_cast<char*>(shaderBlob->GetBufferPointer()), size)) 
	{
		shaderBlob->Release();
		throw std::runtime_error("Failed to read file");
	}

	return shaderBlob;
}

void GCShader::PreCompile(const std::string& filePath, const std::string& csoDestinationPath) 
{
	std::wstring wideFilePath(filePath.begin(), filePath.end());
	std::wstring wideCsoDestinationPath(csoDestinationPath.begin(), csoDestinationPath.end());

	ID3DBlob* vsByteCode = CompileShaderBase(wideFilePath, nullptr, "VS", "vs_5_0");
	ID3DBlob* psByteCode = CompileShaderBase(wideFilePath, nullptr, "PS", "ps_5_0");

	SaveShaderToFile(vsByteCode, wideCsoDestinationPath + L"VS.cso");
	SaveShaderToFile(psByteCode, wideCsoDestinationPath + L"PS.cso");
}

bool GCShader::Load() {
	CompileShader();
	RootSign();
	Pso();

	if (!CHECK_POINTERSNULL("All shader ptr are loaded", "Shader pointers are not correctly loaded", m_RootSignature, m_PSO, m_vsByteCode, m_psByteCode))
		return false;

	return true;
}