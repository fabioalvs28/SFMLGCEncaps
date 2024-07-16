#include "pch.h"
//test
GCShader::GCShader()
	: m_RootSignature(nullptr),

	m_pPsoAlpha(nullptr),
	m_pPsoNoAlpha(nullptr),

	m_vsByteCode(nullptr),
	m_psByteCode(nullptr),

	m_pRender(nullptr),

	m_flagRootParameters(0),
	m_flagEnabledBits(0),
	m_cullMode(D3D12_CULL_MODE_NONE),

	m_pRtt(nullptr),

	m_rootParameter_ConstantBuffer_0(-1),
	m_rootParameter_ConstantBuffer_1(-1),
	m_rootParameter_ConstantBuffer_2(-1),
	m_rootParameter_ConstantBuffer_3(-1),
	m_rootParameter_DescriptorTable_1(-1),
	m_rootParameter_DescriptorTable_2(-1)

{
	m_psCsoPath.clear();
	m_vsCsoPath.clear();
	m_InputLayout.clear(); 

	for (int i = 0; i < 4; ++i) {
		m_rtvFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM;
	}
}


GCShader::~GCShader()
{
	SAFE_RELEASE(&m_RootSignature);
	SAFE_RELEASE(&m_pPsoAlpha);
	SAFE_RELEASE(&m_pPsoNoAlpha);
	SAFE_RELEASE(&m_vsByteCode);
	SAFE_RELEASE(&m_psByteCode);

	m_InputLayout.clear();
}

GC_GRAPHICS_ERROR GCShader::Initialize(GCRenderContext* pRender, const std::string& filePath, const std::string& csoDestinationPath, int& flagEnabledBits, D3D12_CULL_MODE cullMode, int flagRootParameters)
{
	if (!CHECK_POINTERSNULL("Render ptr is not null", "Render pointer is null", pRender))
		return GCRENDER_ERROR_POINTER_NULL;
	if (!CHECK_FILE(filePath, "Shader not found: " + filePath, "Shader file: " + filePath + " loaded successfully"))
		return GCRENDER_ERROR_SHADER_CREATION_FAILED;

	std::wstring baseCsoPath(csoDestinationPath.begin(), csoDestinationPath.end());
	m_vsCsoPath = baseCsoPath + L"VS.cso";
	m_psCsoPath = baseCsoPath + L"PS.cso";

	m_cullMode = cullMode;
	m_pRender = pRender;

	// Vertex Input Layout
	m_flagEnabledBits = flagEnabledBits;
	m_flagRootParameters = flagRootParameters;

	PreCompile(filePath, csoDestinationPath);

	return GCRENDER_SUCCESS_OK;
}
 
void GCShader::CompileShader()
{
	m_vsByteCode = LoadShaderFromFile(m_vsCsoPath);
	m_psByteCode = LoadShaderFromFile(m_psCsoPath);

	m_InputLayout.clear();

	UINT offset = 0; 
	if (HAS_FLAG(m_flagEnabledBits, VERTEX_POSITION)) {
		m_InputLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offset, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		offset += sizeof(DirectX::XMFLOAT3); 
	}

	if (HAS_FLAG(m_flagEnabledBits, VERTEX_COLOR)) {
		m_InputLayout.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offset, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		offset += sizeof(DirectX::XMFLOAT4); 
	}

	if (HAS_FLAG(m_flagEnabledBits, VERTEX_UV)) {
		m_InputLayout.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offset, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		offset += sizeof(DirectX::XMFLOAT2); // Taille des coordonnées de texture
	}

	if (HAS_FLAG(m_flagEnabledBits, VERTEX_NORMAL)) {
		m_InputLayout.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offset, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		offset += sizeof(DirectX::XMFLOAT3); // Taille des coordonnées de texture
	}
}


void GCShader::RootSign()
{
	CD3DX12_ROOT_PARAMETER slotRootParameter[6]; // Max parameters for a shader

	UINT numParameters = 0; // Dynamic param attribution

	if (HAS_FLAG(m_flagRootParameters, ROOT_PARAMETER_CB0)) {
		m_rootParameter_ConstantBuffer_0 = numParameters;
		slotRootParameter[numParameters++].InitAsConstantBufferView(0);
	}
	if (HAS_FLAG(m_flagRootParameters, ROOT_PARAMETER_CB1)) {
		m_rootParameter_ConstantBuffer_1 = numParameters;
		slotRootParameter[numParameters++].InitAsConstantBufferView(1);
	}
	if (HAS_FLAG(m_flagRootParameters, ROOT_PARAMETER_CB2))  {
		m_rootParameter_ConstantBuffer_2 = numParameters;
		slotRootParameter[numParameters++].InitAsConstantBufferView(2);
	}
	if (HAS_FLAG(m_flagRootParameters, ROOT_PARAMETER_CB3)) {
		m_rootParameter_ConstantBuffer_3 = numParameters;
		slotRootParameter[numParameters++].InitAsConstantBufferView(3);
	}

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

	CD3DX12_ROOT_SIGNATURE_DESC rootSignDesc;

	if (HAS_FLAG(m_flagEnabledBits, VERTEX_UV)) {

		if (HAS_FLAG(m_flagRootParameters, ROOT_PARAMETER_DESCRIPTOR_TABLE_SLOT1)) {
			m_rootParameter_DescriptorTable_1 = numParameters;
			CD3DX12_DESCRIPTOR_RANGE srvTable;
			srvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
			slotRootParameter[numParameters++].InitAsDescriptorTable(1, &srvTable);
		}
		
		if (HAS_FLAG(m_flagRootParameters, ROOT_PARAMETER_DESCRIPTOR_TABLE_SLOT2)) {
			m_rootParameter_DescriptorTable_2 = numParameters;
			CD3DX12_DESCRIPTOR_RANGE srvTable2;
			srvTable2.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);
			slotRootParameter[numParameters++].InitAsDescriptorTable(1, &srvTable2);
		}
	}

	rootSignDesc.Init(numParameters, slotRootParameter, 1, &staticSample, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ID3DBlob* serializedRootSig = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSignDesc, D3D_ROOT_SIGNATURE_VERSION_1, &serializedRootSig, &errorBlob);

	if (errorBlob != nullptr) ::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
    if (errorBlob != nullptr) ::OutputDebugStringA((char*)errorBlob->GetBufferPointer());

    m_pRender->GetRenderResources()->Getmd3dDevice()->CreateRootSignature(0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature));
}

void GCShader::Pso() 
{
	// Initialize the graphics pipeline state description
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDescAlpha = {};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDescNoAlpha = {};

	psoDescAlpha.InputLayout = { m_InputLayout.data(), (UINT)m_InputLayout.size() };
	psoDescAlpha.pRootSignature = m_RootSignature;

	psoDescNoAlpha.InputLayout = { m_InputLayout.data(), (UINT)m_InputLayout.size() };
	psoDescNoAlpha.pRootSignature = m_RootSignature;

	psoDescAlpha.VS = { reinterpret_cast<BYTE*>(m_vsByteCode->GetBufferPointer()), m_vsByteCode->GetBufferSize()};
	psoDescAlpha.PS ={ reinterpret_cast<BYTE*>(m_psByteCode->GetBufferPointer()), m_psByteCode->GetBufferSize()};

	psoDescNoAlpha.VS = { reinterpret_cast<BYTE*>(m_vsByteCode->GetBufferPointer()), m_vsByteCode->GetBufferSize() };
	psoDescNoAlpha.PS = { reinterpret_cast<BYTE*>(m_psByteCode->GetBufferPointer()), m_psByteCode->GetBufferSize() };

	psoDescAlpha.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDescAlpha.RasterizerState.CullMode = m_cullMode;

	psoDescNoAlpha.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDescNoAlpha.RasterizerState.CullMode = m_cullMode;

	// Customize the blend state for transparency
	CD3DX12_BLEND_DESC blendDesc1(D3D12_DEFAULT);
	blendDesc1.RenderTarget[0].BlendEnable = TRUE;
	blendDesc1.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc1.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc1.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc1.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc1.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc1.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc1.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	CD3DX12_BLEND_DESC blendDesc2(D3D12_DEFAULT);
	blendDesc2.RenderTarget[0].BlendEnable = FALSE; // Disable blending
	blendDesc2.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
	blendDesc2.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
	blendDesc2.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc2.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc2.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc2.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc2.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	psoDescAlpha.BlendState = blendDesc1;
	psoDescNoAlpha.BlendState = blendDesc2;

	int rtvSize = sizeof(m_rtvFormats) / sizeof(DXGI_FORMAT);

	psoDescAlpha.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT); // #TOTHINK Phenomene etrange dans l'ordre de priorité
	psoDescAlpha.SampleMask = UINT_MAX;
	psoDescAlpha.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDescAlpha.NumRenderTargets = rtvSize;
	psoDescAlpha.SampleDesc.Count = m_pRender->GetRenderResources()->Get4xMsaaState() ? 4 : 1;
	psoDescAlpha.SampleDesc.Quality = m_pRender->GetRenderResources()->Get4xMsaaState() ? (m_pRender->GetRenderResources()->Get4xMsaaQuality() - 1) : 0;
	psoDescAlpha.DSVFormat = m_pRender->GetRenderResources()->GetDepthStencilFormat();

	psoDescNoAlpha.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT); // #TOTHINK Phenomene etrange dans l'ordre de priorité
	psoDescNoAlpha.SampleMask = UINT_MAX;
	psoDescNoAlpha.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDescNoAlpha.NumRenderTargets = rtvSize;
	psoDescNoAlpha.SampleDesc.Count = m_pRender->GetRenderResources()->Get4xMsaaState() ? 4 : 1;
	psoDescNoAlpha.SampleDesc.Quality = m_pRender->GetRenderResources()->Get4xMsaaState() ? (m_pRender->GetRenderResources()->Get4xMsaaQuality() - 1) : 0;
	psoDescNoAlpha.DSVFormat = m_pRender->GetRenderResources()->GetDepthStencilFormat();

	for (UINT i = 0; i < rtvSize; ++i) {
		psoDescAlpha.RTVFormats[i] = m_rtvFormats[i];
		psoDescNoAlpha.RTVFormats[i] = m_rtvFormats[i];
	}

	// Create the graphics pipeline state
	m_pRender->GetRenderResources()->Getmd3dDevice()->CreateGraphicsPipelineState(&psoDescAlpha, IID_PPV_ARGS(&m_pPsoAlpha));
	m_pRender->GetRenderResources()->Getmd3dDevice()->CreateGraphicsPipelineState(&psoDescNoAlpha, IID_PPV_ARGS(&m_pPsoNoAlpha));
}

ID3D12RootSignature* GCShader::GetRootSign() 
{
	return m_RootSignature;
}

ID3D12PipelineState* GCShader::GetPso(bool alpha) 
{
	if (alpha == true)
		return m_pPsoAlpha;
	return m_pPsoNoAlpha;
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

GC_GRAPHICS_ERROR GCShader::Load() {
	CompileShader();
	RootSign();
	Pso();

	if (!CHECK_POINTERSNULL("All shader ptr are loaded", "Shader pointers are not correctly loaded", m_RootSignature, m_pPsoAlpha, m_pPsoNoAlpha, m_vsByteCode, m_psByteCode))
		return GCRENDER_ERROR_POINTER_NULL;

	return GCRENDER_SUCCESS_OK;
}

void GCShader::SetRenderTarget(ID3D12Resource* rtt) {
	m_pRtt = rtt;
}

void GCShader::SetRenderTargetFormats(DXGI_FORMAT format, int i)
{
	m_rtvFormats[i] = format;
}