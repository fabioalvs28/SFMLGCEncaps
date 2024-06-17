#include "framework.h"




GCShader::GCShader() {

}


GCShader::~GCShader()
{

};



void GCShader::Render() {
	m_pRender->GetCommandList()->SetPipelineState(GetPso());
	m_pRender->GetCommandList()->SetGraphicsRootSignature(GetRootSign());
}


//#TODO Need To make documentation
// Initialize Precompile the shader | And load must be call when we need it 
void GCShader::Initialize(GCRender* pRender, const std::string& filePath, const std::string& csoDestinationPath, int type) {

	std::wstring wideFilePath(filePath.begin(), filePath.end());

	if (_waccess(wideFilePath.c_str(), 0) == 0)
		OutputDebugString((L"Shader not found: " + wideFilePath + L"\n").c_str());


	m_pRender = pRender;
	m_type = type;
	PreCompile(filePath, csoDestinationPath);

}
 
void GCShader::CompileShader() {

}

void GCShader::RootSign() {
    // Déclaration des paramètres racine
    CD3DX12_ROOT_PARAMETER slotRootParameter[3];

	slotRootParameter[0].InitAsConstantBufferView(0);
	slotRootParameter[1].InitAsConstantBufferView(1);
    
	// If texture
	if (m_type == texture) {
		CD3DX12_DESCRIPTOR_RANGE srvTable;
		srvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
		slotRootParameter[2].InitAsDescriptorTable(1, &srvTable);
	}

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



    // Configuration de la signature racine


    CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(m_type+2, slotRootParameter, 1, &staticSample, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    // Sérialisation de la signature racine
    ID3DBlob* serializedRootSig = nullptr;
    ID3DBlob* errorBlob = nullptr;
    HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &serializedRootSig, &errorBlob);

    // Gestion des erreurs de sérialisation
    if (errorBlob != nullptr) {
        ::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
    }

    // Création de la signature racine
    m_pRender->Getmd3dDevice()->CreateRootSignature(
        0,
        serializedRootSig->GetBufferPointer(),
        serializedRootSig->GetBufferSize(),
        IID_PPV_ARGS(&m_RootSignature)
    );
}

void GCShader::Pso() {
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
	psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;

	// Customize the blend state for transparency
	CD3DX12_BLEND_DESC blendDesc(D3D12_DEFAULT);
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	psoDesc.BlendState = blendDesc;

	// Use default depth stencil state
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// Set other pipeline state settings
	//psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME; //permet de voir les bordure
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = m_pRender->GetBackBufferFormat();
	psoDesc.SampleDesc.Count = m_pRender->Get4xMsaaState() ? 4 : 1;
	psoDesc.SampleDesc.Quality = m_pRender->Get4xMsaaState() ? (m_pRender->Get4xMsaaQuality() - 1) : 0;
	psoDesc.DSVFormat = m_pRender->GetDepthStencilFormat();

	// Create the graphics pipeline state
	m_pRender->Getmd3dDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_PSO));
}



ID3D12RootSignature* GCShader::GetRootSign() {
	return m_RootSignature;
}

ID3D12PipelineState* GCShader::GetPso() {
	return m_PSO;
}


ID3DBlob* GCShader::GetmvsByteCode()
{
	return m_vsByteCode;
};

ID3DBlob* GCShader::GetmpsByteCode()
{
	return m_psByteCode;
};


ID3DBlob* GCShader::CompileShaderBase(const std::wstring& filename, const D3D_SHADER_MACRO* defines, const std::string& entrypoint,const std::string& target)
{
	UINT compileFlags = 0;
	#if defined(DEBUG) || defined(_DEBUG)  
		compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	#endif

	HRESULT hr = S_OK;

	ID3DBlob* byteCode = nullptr;
	ID3DBlob* errors;
	hr = D3DCompileFromFile(filename.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entrypoint.c_str(), target.c_str(), compileFlags, 0, &byteCode, &errors);

	if (errors != nullptr)
		OutputDebugStringA((char*)errors->GetBufferPointer());

	//hr;

	return byteCode;
}

void GCShader::SaveShaderToFile(ID3DBlob* shaderBlob, const std::wstring& filename) {
	std::ofstream file(filename, std::ios::binary);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file for writing");
	}
	file.write(static_cast<const char*>(shaderBlob->GetBufferPointer()), shaderBlob->GetBufferSize());
	file.close();
}

ID3DBlob* GCShader::LoadShaderFromFile(const std::wstring& filename) {
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file for reading");
	}
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	ID3DBlob* shaderBlob = nullptr;
	HRESULT hr = D3DCreateBlob(size, &shaderBlob);
	if (FAILED(hr)) {
		throw std::runtime_error("Failed to create shader blob");
	}

	if (!file.read(static_cast<char*>(shaderBlob->GetBufferPointer()), size)) {
		shaderBlob->Release();
		throw std::runtime_error("Failed to read file");
	}

	return shaderBlob;
}

void GCShader::PreCompile(const std::string& filePath, const std::string& csoDestinationPath) {
	std::wstring wideFilePath(filePath.begin(), filePath.end());
	std::wstring wideCsoDestinationPath(csoDestinationPath.begin(), csoDestinationPath.end());

	ID3DBlob* vsByteCode = CompileShaderBase(wideFilePath, nullptr, "VS", "vs_5_0");
	ID3DBlob* psByteCode = CompileShaderBase(wideFilePath, nullptr, "PS", "ps_5_0");

	SaveShaderToFile(vsByteCode, wideCsoDestinationPath + L"VS.cso");
	SaveShaderToFile(psByteCode, wideCsoDestinationPath + L"PS.cso");
}

//void GCShader::UpdateCameraBuffer(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projMatrix)
//{
//    //viewMatrix = DirectX::XMMatrixTranspose(viewMatrix);
//    //projMatrix = DirectX::XMMatrixTranspose(projMatrix);
//
//
//    //XMStoreFloat4x4(&cameraCB.view, viewMatrix);
//    //XMStoreFloat4x4(&cameraCB.proj, projMatrix);
//
//
//    m_pCameraCB->CopyData(0, cameraCB);  // Pass the address of cameraCB
//}