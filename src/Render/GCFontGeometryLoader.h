#pragma once
    
class GCFontGeometryLoader {
public:
    GCFontGeometryLoader();

    void Initialize(const std::string& metadataFile);

    // Crée un maillage à partir du texte fourni
    GCGeometry* CreateText(const std::string& text);
    void GenerateFontMetadata(std::string filePath);

private:
    void LoadMetadata(const std::string& metadataFile);
    void GenerateMesh(GCGeometry* geometry, const std::string& text);
    std::vector<std::tuple<int, float, float, float, float, float, float, float, float>> m_metadata; 
};
