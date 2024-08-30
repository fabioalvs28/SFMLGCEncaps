#ifndef GCUtilites_h
#define GCUtilites_h

typedef const char*			   cstr;
typedef const wchar_t*		   custr;
typedef __int8                 I8;
typedef __int16                I16;
typedef __int32                I32;
typedef __int64                I64;
typedef unsigned __int8        UI8;
typedef unsigned __int16       UI16;
typedef unsigned __int32       UI32;
typedef unsigned __int64       UI64;



#define RGB(r, g, b) (UI32)(r << 24 | g << 16 | b << 8 | 0)
#define RGBA(r, g, b, a) (UI32)(r << 24 | g << 16 | b << 8 | a)
#define GetRValue(rgb) (uint8_t)(rgb & 0xFF << 24)
#define GetGValue(rgb) (uint8_t)(rgb & 0xFF << 16)
#define GetBValue(rgb) (uint8_t)(rgb & 0xFF << 8)
#define GetAValue(rgb) (uint8_t)(rgb & 0xFF << 0)

#define BYTE                   UI8

#define DELPTRS(ptr) do { \
    delete[] ptr;        \
    ptr = nullptr;       \
} while (0)

#endif // !GCUtilites.h
