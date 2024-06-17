#include "framework.h"

void GCShaderTexture::CompileShader() 
{
	m_vsByteCode = LoadShaderFromFile(L"../../../src/Render/CsoCompiled/textureVS.cso");
	m_psByteCode = LoadShaderFromFile(L"../../../src/Render/CsoCompiled/texturePS.cso");
	m_InputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
}