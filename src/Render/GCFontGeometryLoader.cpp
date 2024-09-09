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

GCGeometry* GCFontGeometryLoader::CreateText(const std::string& text, GCColor textColor) {
    GCGeometry* pGeometry = new GCGeometry;
    m_textColor = GCUtils::GCColorToXMFLOAT4(textColor);
    GenerateMesh(pGeometry, text);

    return pGeometry;
}

void GCFontGeometryLoader::GenerateMesh(GCGeometry* pGeometry, const std::string& text) {
    pGeometry->pos.clear();
    pGeometry->uv.clear();
    pGeometry->indices.clear();
    pGeometry->color.clear();

    float xOffset = 0.0f;
    float yOffset = 0.0f;
    float charWidth = 0.2f;
    float charHeight = 0.4f;
    float spacing = 0.1f;
    bool isBold = false;
    bool isItalic = false;
    int letterAmount = 186;

    for (char c : text) {
        if (c == ' ') {
            xOffset += spacing;
            continue;
        }

        if (c == '\b' && isItalic == false)
        {
            isBold = !isBold;
            continue;
        }

        if (c == '\t' && isBold == false)
        {
            isItalic = !isItalic;
            continue;
        }

        if (c == '\n')
        {
            xOffset = 0;
            yOffset -= charHeight;
            continue;
        }

        int startIdx = static_cast<int>(pGeometry->pos.size());
        int asciiIndex;
        float u1, v1, u2, v2, u3, v3, u4, v4;
        bool found = false;

        for (const auto& data : m_metadata) {
            int charId;
            std::tie(charId, u1, v1, u2, v2, u3, v3, u4, v4) = data;

            if (static_cast<int>(c) < 0)
                asciiIndex = static_cast<int>(c) + 95 + 127;
            else
                asciiIndex = static_cast<int>(c);

            asciiIndex += letterAmount * isBold + 2 * letterAmount * isItalic;

            if (charId == asciiIndex) {
                found = true;
                break;
            }
        }



        if (!found) {
            u1 = 0.040000f; v1 = 0.100000f;
            u2 = 0.040000f; v2 = 0.055000f;
            u3 = 0.080000f; v3 = 0.055000f;
            u4 = 0.080000f; v4 = 0.100000f;
        }

        pGeometry->pos.push_back(DirectX::XMFLOAT3(xOffset, yOffset, 0.0f));
        pGeometry->pos.push_back(DirectX::XMFLOAT3(xOffset, yOffset + charHeight, 0.0f));
        pGeometry->pos.push_back(DirectX::XMFLOAT3(xOffset + charWidth, yOffset + charHeight, 0.0f));
        pGeometry->pos.push_back(DirectX::XMFLOAT3(xOffset + charWidth, yOffset, 0.0f));

        pGeometry->color.push_back(DirectX::XMFLOAT4(m_textColor));
        pGeometry->color.push_back(DirectX::XMFLOAT4(m_textColor));
        pGeometry->color.push_back(DirectX::XMFLOAT4(m_textColor));
        pGeometry->color.push_back(DirectX::XMFLOAT4(m_textColor));

        pGeometry->indices.push_back(startIdx + 0);
        pGeometry->indices.push_back(startIdx + 1);
        pGeometry->indices.push_back(startIdx + 2);
        pGeometry->indices.push_back(startIdx + 0);
        pGeometry->indices.push_back(startIdx + 2);
        pGeometry->indices.push_back(startIdx + 3);

        pGeometry->uv.push_back(DirectX::XMFLOAT2(u1, v1));
        pGeometry->uv.push_back(DirectX::XMFLOAT2(u2, v2));
        pGeometry->uv.push_back(DirectX::XMFLOAT2(u3, v3));
        pGeometry->uv.push_back(DirectX::XMFLOAT2(u4, v4));

        xOffset += charWidth + spacing;
    }

    pGeometry->vertexNumber = pGeometry->pos.size();
    pGeometry->indiceNumber = pGeometry->indices.size();
}

void GCFontGeometryLoader::GenerateFontMetadata(std::string filePath) 
{
    std::ofstream outputFile;

    if(std::ifstream(filePath).good() == false)
         outputFile.open(filePath);
    
    int letterWidth = 220; //letter width in px
    int letterHeight = 260; ///letter height in px
    int fileWidth = 5280, fileHeight = 6240;
    int letterAmount = 3*186; // regular bold & italics
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