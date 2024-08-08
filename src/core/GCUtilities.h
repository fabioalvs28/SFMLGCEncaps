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

//typedef __int32               COLORREF;


#define RGB(r, g, b) ((COLORREF)(((uint8_t)(r) | ((uint16_t)((uint8_t)(g)) << 8)) | (((uint32_t)(uint8_t)(b)) << 16)))
#define RGBA(r, g, b, a) ((COLORREF)(((uint8_t)(r) | ((uint16_t)((uint8_t)(g)) << 8)) | (((uint32_t)(uint8_t)(b)) << 16)))
#define GetRValue(rgb) ((uint8_t)(rgb))
#define GetGValue(rgb) ((uint8_t)(((uint16_t)(rgb)) >> 8))
#define GetBValue(rgb) ((uint8_t)((rgb) >> 16))

#define BYTE                   UI8

#define DELPTRS(ptr) do { \
    delete[] ptr;        \
    ptr = nullptr;       \
} while (0)

#endif // !GCUtilites.h
