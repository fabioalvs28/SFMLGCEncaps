#ifndef PCH_H
#define PCH_H
#pragma once

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")


//#define WIN32_LEAN_AND_MEAN             // Exclure les en-t�tes Windows rarement utilis�s
// Fichiers d'en-t�te Windows
#include <windows.h>

// Fichiers d'en-t�te C RunTime
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "Timer.h"
#include <string>
#include <map>
#include <codecvt>
#include <locale>
#include <vector>
#include <variant>
#include <chrono>
#include <memory>
#include <algorithm>
#include <array>
#include <unordered_map>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <cassert>
#include <iostream>


#include <wrl.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <d3d11_1.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <wrl/client.h>
#include <WindowsX.h>


struct GCGeometry;
struct GCMESHBUFFERDATA;


struct GCSHADERCB;
struct GCWORLDCB;
struct GCVIEWPROJCB;

enum GC_PRIMITIVE_ID;

// Add William 
class GCGraphicsLogger;
//


class GCGraphics;
class GCMaterial;
class GCMesh;
class GCModelParser;
class GCModelParserObj;
class GCPrimitiveFactory;
class GCRender;
class GCShader;
class GCTexture;
class GCTextureFactory;
class GCShaderUploadBufferBase; // 
class GCUtils;
class MathHelper;
class Timer;
class Window;

#include "d3dx12.h"
#include "DDSTextureLoader.h"
#include "MathHelper.h"


// Add William 
#include "./GCGraphicsLogger.h"
#include "./GCShaderConstantBufferStruct.h"
#include "./GCMeshBufferData.h"
#include "GCPrimitiveIdEnum.h"


//
#include "d3dx12.h"
#include "Window.h"
#include "Macros.h"
#include "GCUploadBuffer.h"
#include "GCRender.h"
#include "Vectors.h"
#include "GCGeometry.h"
#include "GCMesh.h"
#include "GCShader.h"
#include "GCMaterial.h"
#include "GCModelParser.h"
#include "GCModelParserObj.h"

#include "GCPrimitiveFactory.h"
#include "GCTexture.h"
#include "GCGraphics.h"
#include "GCTextureFactory.h"
#include "Timer.h"
#include "GCUtils.h"



#endif // PCH_H