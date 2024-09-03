#include "pch.h"
//height : 74
//width : 29
//0,0 : 0 , 116
GCFontGeometryLoader::GCFontGeometryLoader() {
}

void GCFontGeometryLoader::Initialize(const std::string& metadataFile) {
    LoadMetadata(metadataFile);
}

void GCFontGeometryLoader::LoadMetadata(const std::string& metadataFile) {
    GCGraphicsLogger& logger = GCGraphicsLogger::GetInstance();

    logger.LogInfo("Attempting to open metadata file: " + metadataFile);

    std::ifstream file(metadataFile);
    if (!file.is_open()) {
        logger.LogWarning("Failed to open metadata file: " + metadataFile);
        throw std::runtime_error("Failed to open metadata file");
    }

    logger.LogInfo("Metadata file opened successfully: " + metadataFile);

    std::string line;
    int textureWidth = 0, textureHeight = 0;
    if (std::getline(file, line)) {
        textureWidth = std::stoi(line);
    }
    if (std::getline(file, line)) {
        textureHeight = std::stoi(line) - 1;
    }

    logger.LogInfo("Texture size: " + std::to_string(textureWidth) + "x" + std::to_string(textureHeight));

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int id;
        float u1, v1, u2, v2, u3, v3, u4, v4;
        if (iss >> id >> u1 >> v1 >> u2 >> v2 >> u3 >> v3 >> u4 >> v4) {
            m_metadata.emplace_back(id, u1 / textureWidth, v1 / textureHeight, u2 / textureWidth, v2 / textureHeight, u3 / textureWidth, v3 / textureHeight, u4 / textureWidth, v4 / textureHeight);
        }
        else {
            logger.LogWarning("Malformed line in metadata file: " + line);
            throw std::runtime_error("Malformed line in metadata file");
        }
    }

    logger.LogInfo("Metadata loading completed.");
}

GCGeometry* GCFontGeometryLoader::CreateText(const std::string& text) {
    GCGeometry* geometry = new GCGeometry;
    GenerateMesh(geometry, text);

    return geometry;
}

void GCFontGeometryLoader::GenerateMesh(GCGeometry* geometry, const std::string& text) {
    geometry->pos.clear();
    geometry->uv.clear();
    geometry->indices.clear();

    float xOffset = 0.3f;
    float yOffset = 0.5f;
    float charWidth = 0.2f;
    float charHeight = 0.4f;
    float spacing = 0.1f;
    bool isBold = false;
    bool isItalic = false;

    for (char c : text) {
        if (c == ' ') {
            xOffset += spacing;
            continue;
        }

        if (c == '\b' && isItalic == false)
        {
            isBold = !isBold;
        }
        
        if (c == '\i' && isBold == false)
        {
            isItalic = !isItalic;
        }

        int startIdx = static_cast<int>(geometry->pos.size());
        int asciiIndex;
        float u1, v1, u2, v2, u3, v3, u4, v4;
        bool found = false;

        for (const auto& data : m_metadata) {
            int charId;
            std::tie(charId, u1, v1, u2, v2, u3, v3, u4, v4) = data;

            if (static_cast<int>(c) < 0)
                asciiIndex = static_cast<int>(c) + 129 + 127 + 185 * isBold + 2 * 185 * isItalic;
            else
                asciiIndex = static_cast<int>(c);

            if (charId == asciiIndex) {
                found = true;
                break;
            }
        }

        if (c == '\n')
        {
            xOffset = 0;
            yOffset -= charHeight;
        }


        if (!found) {
            u1 = 0.040000f; v1 = 0.100000f;
            u2 = 0.040000f; v2 = 0.055000f;
            u3 = 0.080000f; v3 = 0.055000f;
            u4 = 0.080000f; v4 = 0.100000f;
        }

        geometry->pos.push_back(DirectX::XMFLOAT3(xOffset, yOffset, 0.0f));
        geometry->pos.push_back(DirectX::XMFLOAT3(xOffset, yOffset + charHeight, 0.0f));
        geometry->pos.push_back(DirectX::XMFLOAT3(xOffset + charWidth, yOffset + charHeight, 0.0f));
        geometry->pos.push_back(DirectX::XMFLOAT3(xOffset + charWidth, yOffset, 0.0f));

        geometry->indices.push_back(startIdx + 0);
        geometry->indices.push_back(startIdx + 1);
        geometry->indices.push_back(startIdx + 2);
        geometry->indices.push_back(startIdx + 0);
        geometry->indices.push_back(startIdx + 2);
        geometry->indices.push_back(startIdx + 3);

        geometry->uv.push_back(DirectX::XMFLOAT2(u1, v1));
        geometry->uv.push_back(DirectX::XMFLOAT2(u2, v2));
        geometry->uv.push_back(DirectX::XMFLOAT2(u3, v3));
        geometry->uv.push_back(DirectX::XMFLOAT2(u4, v4));

        xOffset += charWidth + spacing;
    }

    geometry->vertexNumber = geometry->pos.size();
    geometry->indiceNumber = geometry->indices.size();
}

void GCFontGeometryLoader::GenerateFontMetadata(std::string filePath) 
{
    std::ofstream outputFile;

    if(std::ifstream(filePath).good() == false)
         outputFile.open(filePath);
    
    int letterWidth = 210; //letter width in px
    int letterHeight = 260; ///letter height in px
    int fileWidth = 2940, fileHeight = 3640;
    int letterAmount = 364;
    int startIndex = 33; //start index from ascii (!)
    int startU = 0; //start coordX
    int startV = 0; //start coordY
    int offsetX = 0;
    int offsetY = 0;

    if (outputFile.is_open())
    {
        outputFile << fileWidth << std::endl;
        outputFile << fileHeight << std::endl;

        for (int i = 0; i < letterAmount; i++)
        {
            outputFile << startIndex << " " << startU + offsetX << " " << startV + letterHeight + offsetY << " " << startU + offsetX << " " << startV + offsetY << " " << startU + letterWidth + offsetX << " " << startV + offsetY << " " << startU + letterWidth + offsetX << " " << startV + letterHeight + offsetY << std::endl;
            
            if (startU + offsetX + letterWidth == fileWidth)//end of line
            {
                offsetY += letterHeight;
                offsetX = 0;
            }
            else
            {
                offsetX += letterWidth;
            }
            startIndex += 1;
        }
        outputFile.close();
    }
}