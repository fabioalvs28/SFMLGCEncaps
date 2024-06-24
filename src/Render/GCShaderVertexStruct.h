struct GCVERTEXCOLOR
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT4 Color;

	GCVERTEXCOLOR() : Pos(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)), Color(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)) {}

	GCVERTEXCOLOR(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT4& color) : Pos(position), Color(color) {}
};

struct GCVERTEXTEXTURE
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT2 TexC; // Uv

	GCVERTEXTEXTURE() : Pos(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)), TexC(DirectX::XMFLOAT2(0.0f, 0.0f)) {}

	GCVERTEXTEXTURE(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT2& texCoord) : Pos(position), TexC(texCoord) {}
};

// Forward declarations

//
//using namespace DirectX;
//
//// Définition de GCVERTEXPOS
//struct GCVERTEXPOS
//{
//    XMFLOAT3 Pos;
//
//    // Définition du type de valeur pour l'initialisation
//    using value_type = XMFLOAT3;
//
//    GCVERTEXPOS(const XMFLOAT3& position = XMFLOAT3(0.0f, 0.0f, 0.0f))
//        : Pos(position)
//    {}
//};
//
//// Définition de GCVERTEXCOL
//struct GCVERTEXCOL
//{
//    XMFLOAT4 Color;
//
//    // Définition du type de valeur pour l'initialisation
//    using value_type = XMFLOAT4;
//
//    GCVERTEXCOL(const XMFLOAT4& color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f))
//        : Color(color)
//    {}
//};
//
//// Définition de GCVERTEXUV
//struct GCVERTEXUV
//{
//    XMFLOAT2 Uv;
//
//    // Définition du type de valeur pour l'initialisation
//    using value_type = XMFLOAT2;
//
//    GCVERTEXUV(const XMFLOAT2& uv = XMFLOAT2(0.0f, 0.0f))
//        : Uv(uv)
//    {}
//};
//
//// Définition de GCVERTEXNORMAL
//struct GCVERTEXNORMAL
//{
//    XMFLOAT3 Normal;
//
//    // Définition du type de valeur pour l'initialisation
//    using value_type = XMFLOAT3;
//
//    GCVERTEXNORMAL(const XMFLOAT3& normal = XMFLOAT3(0.0f, 0.0f, 0.0f))
//        : Normal(normal)
//    {}
//};
//
//// Définition du template GCVERTEX
//template<typename... VertexTypes>
//struct GCVERTEX : public VertexTypes...
//{
//    // Constructeur variadique
//    GCVERTEX(const typename VertexTypes::value_type&... args)
//        : VertexTypes(args)...  // Initialisation des bases de classe avec les arguments fournis
//    {}
//};
//
//// Exemples d'utilisation
//using GCVertexPosColor = GCVERTEX<GCVERTEXPOS, GCVERTEXCOL>;
//using GCVertexPosNormal = GCVERTEX<GCVERTEXPOS, GCVERTEXNORMAL>;
//using GCVertexPosUVNormal = GCVERTEX<GCVERTEXPOS, GCVERTEXUV, GCVERTEXNORMAL>;