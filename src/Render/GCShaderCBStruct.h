struct GCSHADERCB {
};

struct GCWORLDCB : GCSHADERCB
{
	DirectX::XMFLOAT4X4 world; // Matrice du monde
};

struct GCCAMERACB : GCSHADERCB
{
	DirectX::XMFLOAT4X4 view; // Matrice de vue
	DirectX::XMFLOAT4X4 proj; // Matrice de projection
};

