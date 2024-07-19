// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef ENGINE_PCH_H
#define ENGINE_PCH_H

typedef const char* cstr;
typedef const wchar_t* custr;
typedef __int8                I8;
typedef __int16                I16;
typedef __int32                I32;
typedef __int64                I64;
typedef unsigned __int8        UI8;
typedef unsigned __int16    UI16;
typedef unsigned __int32    UI32;
typedef unsigned __int64    UI64;


// add headers that you want to pre-compile here
#include "framework.h"
#include "GCColor.h"

#endif //ENGINE_PCH_H
