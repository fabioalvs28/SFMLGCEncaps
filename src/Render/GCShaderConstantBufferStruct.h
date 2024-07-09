struct GCSHADERCB 
{

};

struct GCWORLDCB : GCSHADERCB
{
    DirectX::XMFLOAT4X4 world; 
    float objectId;
};

struct GCVIEWPROJCB : GCSHADERCB
{
    DirectX::XMFLOAT4X4 view; 
    DirectX::XMFLOAT4X4 proj; 
};

struct GCMATERIALPROPERTIES : GCSHADERCB
{
    DirectX::XMFLOAT4 ambientLightColor;
    DirectX::XMFLOAT4 ambient;
    DirectX::XMFLOAT4 diffuse;
    DirectX::XMFLOAT4 specular;
    float shininess;
    float padding[3];
};

struct GCLIGHT
{
    DirectX::XMFLOAT3 position;
    float padding1;
    DirectX::XMFLOAT3 direction;
    float padding2;
    DirectX::XMFLOAT3 color;
    float spotAngle;
    int lightType;
    float lightIntensity; 
    float padding3[2];    
};

struct GCLIGHTSPROPERTIES : GCSHADERCB
{
    GCLIGHT lights[10]; 
};