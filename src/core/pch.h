// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef CORE_PCH_H
#define CORE_PCH_H
#include "Define.h"

#include "framework.h"

typedef const char* cstr;
typedef const wchar_t* custr;
typedef __int8 I8;
typedef __int16 I16;
typedef __int32 I32;
typedef __int64 I64;
typedef unsigned __int8 UI8;
typedef unsigned __int16 UI16;
typedef unsigned __int32 UI32;
typedef unsigned __int64 UI64;

// Containers
template <typename T> class GCQueue;
template <typename T> class GCListNode;
template <typename T> class GCList;
template <typename T> class GCVector;
template <typename Key, typename Value> class GCMap;

// Math
struct GCVEC2;
struct GCVEC3;
struct GCQUATERNION;
struct GCMATRIX;
struct GCTransform;

// Logger
class Logger;

// Threading
class GCThread;
class WinThread;

// Other
class GCString;
struct GCColor;



#include <cstdint>
#include <vector>
#include <type_traits>


// Containers
#include "Queue.h"
#include "List.h"
#include "Vector.h"
#include "Map.h"

// Math
#include "Vectors.h"
#include "Quaternion.h"
#include "Matrix.h"
#include "Transform.h"

// Logger
#include "Log.h"
#include "Logger.h"

// Threading
#include "Thread.h"

// Other
#include "GCString.h"
#include "GCColor.h"
#include "GCFile.h"
#include "GCImage.h"
#include "BMPHeader.h"
#include "dds.hpp"
#include "savedds.h"
#include "stb_dxt.h"
#include "GCSpriteSheetHandler.h"

#endif