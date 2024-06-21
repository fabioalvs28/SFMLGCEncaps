struct GCVERTEX
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT4 Color;

	GCVERTEX() : Pos(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)), Color(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)) {}

	GCVERTEX(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT4& color) : Pos(position), Color(color) {}
};

struct GCVERTEXTEXTURE
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT2 TexC; // Uv

	GCVERTEXTEXTURE() : Pos(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)), TexC(DirectX::XMFLOAT2(0.0f, 0.0f)) {}

	GCVERTEXTEXTURE(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT2& texCoord) : Pos(position), TexC(texCoord) {}
};
