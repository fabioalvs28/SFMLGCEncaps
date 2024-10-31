#pragma once
#include <algorithm>
#include "pch.h"

#include <cstdint>
#include <vector>
#if defined(_MSVC_LANG)
#define DDS_CPP_VERSION _MSVC_LANG
#else
#define DDS_CPP_VERSION __cplusplus
#endif

#define MAKE_FOUR_CHARACTER_CODE(char1, char2, char3, char4)                                                                               \
    static_cast<uint32_t>(char1) | (static_cast<uint32_t>(char2) << 8) | (static_cast<uint32_t>(char3) << 16) |                            \
        (static_cast<uint32_t>(char4) << 24)

#define UNDO_FOUR_CHARACTER_CODE(x, name)                                                                                            \
    unsigned char name[4];                                                                                                           \
    name[0] = x >> 0;                                                                                                                \
    name[1] = x >> 8;                                                                                                                \
    name[2] = x >> 16;                                                                                                               \
    name[3] = x >> 24;

#if __cplusplus >= 201703L || (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L)
#define DDS_CPP_17 1
#else
#define DDS_CPP_17 0
#endif

#if __cplusplus >= 202002L || (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L)
#define DDS_CPP_20 1
#else
#define DDS_CPP_20 0
#endif

#if DDS_CPP_17
#define DDS_NO_DISCARD [[nodiscard]]
#else
#define DDS_NO_DISCARD
#endif

#include "dds_formats.hpp"

#if DDS_CPP_17 && defined(DDS_USE_STD_FILESYSTEM)
#include <filesystem>
namespace fs = std::filesystem;
#endif

#if DDS_CPP_20
#include <concepts>
#endif

#include <fstream>
#include <iostream>

// clang-format off
#ifndef __dxgiformat_h__ // We'll try and act as if we're the actual dxgiformat.h header.
#define __dxgiformat_h__
// See https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format
// For converting D3DFormat to DXGI_Format see the following table:
// https://docs.microsoft.com/en-us/windows/win32/direct3d10/d3d10-graphics-programming-guide-resources-legacy-formats.
enum DXGI_FORMAT {
    DXGI_FORMAT_UNKNOWN	                    = 0,
    DXGI_FORMAT_R32G32B32A32_TYPELESS       = 1,
    DXGI_FORMAT_R32G32B32A32_FLOAT          = 2,
    DXGI_FORMAT_R32G32B32A32_UINT           = 3,
    DXGI_FORMAT_R32G32B32A32_SINT           = 4,
    DXGI_FORMAT_R32G32B32_TYPELESS          = 5,
    DXGI_FORMAT_R32G32B32_FLOAT             = 6,
    DXGI_FORMAT_R32G32B32_UINT              = 7,
    DXGI_FORMAT_R32G32B32_SINT              = 8,
    DXGI_FORMAT_R16G16B16A16_TYPELESS       = 9,
    DXGI_FORMAT_R16G16B16A16_FLOAT          = 10,
    DXGI_FORMAT_R16G16B16A16_UNORM          = 11,
    DXGI_FORMAT_R16G16B16A16_UINT           = 12,
    DXGI_FORMAT_R16G16B16A16_SNORM          = 13,
    DXGI_FORMAT_R16G16B16A16_SINT           = 14,
    DXGI_FORMAT_R32G32_TYPELESS             = 15,
    DXGI_FORMAT_R32G32_FLOAT                = 16,
    DXGI_FORMAT_R32G32_UINT                 = 17,
    DXGI_FORMAT_R32G32_SINT                 = 18,
    DXGI_FORMAT_R32G8X24_TYPELESS           = 19,
    DXGI_FORMAT_D32_FLOAT_S8X24_UINT        = 20,
    DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS    = 21,
    DXGI_FORMAT_X32_TYPELESS_G8X24_UINT     = 22,
    DXGI_FORMAT_R10G10B10A2_TYPELESS        = 23,
    DXGI_FORMAT_R10G10B10A2_UNORM           = 24,
    DXGI_FORMAT_R10G10B10A2_UINT            = 25,
    DXGI_FORMAT_R11G11B10_FLOAT             = 26,
    DXGI_FORMAT_R8G8B8A8_TYPELESS           = 27,
    DXGI_FORMAT_R8G8B8A8_UNORM              = 28,
    DXGI_FORMAT_R8G8B8A8_UNORM_SRGB         = 29,
    DXGI_FORMAT_R8G8B8A8_UINT               = 30,
    DXGI_FORMAT_R8G8B8A8_SNORM              = 31,
    DXGI_FORMAT_R8G8B8A8_SINT               = 32,
    DXGI_FORMAT_R16G16_TYPELESS             = 33,
    DXGI_FORMAT_R16G16_FLOAT                = 34,
    DXGI_FORMAT_R16G16_UNORM                = 35,
    DXGI_FORMAT_R16G16_UINT                 = 36,
    DXGI_FORMAT_R16G16_SNORM                = 37,
    DXGI_FORMAT_R16G16_SINT                 = 38,
    DXGI_FORMAT_R32_TYPELESS                = 39,
    DXGI_FORMAT_D32_FLOAT                   = 40,
    DXGI_FORMAT_R32_FLOAT                   = 41,
    DXGI_FORMAT_R32_UINT                    = 42,
    DXGI_FORMAT_R32_SINT                    = 43,
    DXGI_FORMAT_R24G8_TYPELESS              = 44,
    DXGI_FORMAT_D24_UNORM_S8_UINT           = 45,
    DXGI_FORMAT_R24_UNORM_X8_TYPELESS       = 46,
    DXGI_FORMAT_X24_TYPELESS_G8_UINT        = 47,
    DXGI_FORMAT_R8G8_TYPELESS               = 48,
    DXGI_FORMAT_R8G8_UNORM                  = 49,
    DXGI_FORMAT_R8G8_UINT                   = 50,
    DXGI_FORMAT_R8G8_SNORM                  = 51,
    DXGI_FORMAT_R8G8_SINT                   = 52,
    DXGI_FORMAT_R16_TYPELESS                = 53,
    DXGI_FORMAT_R16_FLOAT                   = 54,
    DXGI_FORMAT_D16_UNORM                   = 55,
    DXGI_FORMAT_R16_UNORM                   = 56,
    DXGI_FORMAT_R16_UINT                    = 57,
    DXGI_FORMAT_R16_SNORM                   = 58,
    DXGI_FORMAT_R16_SINT                    = 59,
    DXGI_FORMAT_R8_TYPELESS                 = 60,
    DXGI_FORMAT_R8_UNORM                    = 61,
    DXGI_FORMAT_R8_UINT                     = 62,
    DXGI_FORMAT_R8_SNORM                    = 63,
    DXGI_FORMAT_R8_SINT                     = 64,
    DXGI_FORMAT_A8_UNORM                    = 65,
    DXGI_FORMAT_R1_UNORM                    = 66,
    DXGI_FORMAT_R9G9B9E5_SHAREDEXP          = 67,
    DXGI_FORMAT_R8G8_B8G8_UNORM             = 68,
    DXGI_FORMAT_G8R8_G8B8_UNORM             = 69,
    DXGI_FORMAT_BC1_TYPELESS                = 70,
    DXGI_FORMAT_BC1_UNORM                   = 71,
    DXGI_FORMAT_BC1_UNORM_SRGB              = 72,
    DXGI_FORMAT_BC2_TYPELESS                = 73,
    DXGI_FORMAT_BC2_UNORM                   = 74,
    DXGI_FORMAT_BC2_UNORM_SRGB              = 75,
    DXGI_FORMAT_BC3_TYPELESS                = 76,
    DXGI_FORMAT_BC3_UNORM                   = 77,
    DXGI_FORMAT_BC3_UNORM_SRGB              = 78,
    DXGI_FORMAT_BC4_TYPELESS                = 79,
    DXGI_FORMAT_BC4_UNORM                   = 80,
    DXGI_FORMAT_BC4_SNORM                   = 81,
    DXGI_FORMAT_BC5_TYPELESS                = 82,
    DXGI_FORMAT_BC5_UNORM                   = 83,
    DXGI_FORMAT_BC5_SNORM                   = 84,
    DXGI_FORMAT_B5G6R5_UNORM                = 85,
    DXGI_FORMAT_B5G5R5A1_UNORM              = 86,
    DXGI_FORMAT_B8G8R8A8_UNORM              = 87,
    DXGI_FORMAT_B8G8R8X8_UNORM              = 88,
    DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM  = 89,
    DXGI_FORMAT_B8G8R8A8_TYPELESS           = 90,
    DXGI_FORMAT_B8G8R8A8_UNORM_SRGB         = 91,
    DXGI_FORMAT_B8G8R8X8_TYPELESS           = 92,
    DXGI_FORMAT_B8G8R8X8_UNORM_SRGB         = 93,
    DXGI_FORMAT_BC6H_TYPELESS               = 94,
    DXGI_FORMAT_BC6H_UF16                   = 95,
    DXGI_FORMAT_BC6H_SF16                   = 96,
    DXGI_FORMAT_BC7_TYPELESS                = 97,
    DXGI_FORMAT_BC7_UNORM                   = 98,
    DXGI_FORMAT_BC7_UNORM_SRGB              = 99,
    DXGI_FORMAT_AYUV                        = 100,
    DXGI_FORMAT_Y410                        = 101,
    DXGI_FORMAT_Y416                        = 102,
    DXGI_FORMAT_NV12                        = 103,
    DXGI_FORMAT_P010                        = 104,
    DXGI_FORMAT_P016                        = 105,
    DXGI_FORMAT_420_OPAQUE                  = 106,
    DXGI_FORMAT_YUY2                        = 107,
    DXGI_FORMAT_Y210                        = 108,
    DXGI_FORMAT_Y216                        = 109,
    DXGI_FORMAT_NV11                        = 110,
    DXGI_FORMAT_AI44                        = 111,
    DXGI_FORMAT_IA44                        = 112,
    DXGI_FORMAT_P8                          = 113,
    DXGI_FORMAT_A8P8                        = 114,
    DXGI_FORMAT_B4G4R4A4_UNORM              = 115,
    DXGI_FORMAT_P208                        = 130,
    DXGI_FORMAT_V208                        = 131,
    DXGI_FORMAT_V408                        = 132,
	DXGI_FORMAT_SAMPLER_FEEDBACK_MIN_MIP_OPAQUE,
	DXGI_FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE,
    DXGI_FORMAT_FORCE_UINT                  = 0xffffffff
};
#endif // #ifndef __dxgiformat_h__
// clang-format on

#if DDS_CPP_20
#include <span>
#endif

namespace dds {
#if DDS_CPP_20
    template <typename T>
    using span = std::span<T>;
#else
    // This is a quick reimplementation of std::span to use it with versions before C++20
    template <typename T>
    class span {
        T* _data;
        std::size_t _size = 0;

    public:
        span(T* data, std::size_t size) : _data(data), _size(size) {};
        DDS_NO_DISCARD std::size_t size() const noexcept {
            return _size;
        }
        DDS_NO_DISCARD std::size_t size_bytes() const noexcept {
            return _size * sizeof(T);
        }
        DDS_NO_DISCARD T* data() const noexcept {
            return _data;
        }
    };
#endif

    enum ResourceDimension { Unknown, Buffer, Texture1D, Texture2D, Texture3D };

    enum ReadResult {
        Success = 0,
        Failure = -1,
        UnsupportedFormat = -2,
        NoDx10Header = -3,
        InvalidSize = -4,
    };

    enum DdsMagicNumber {
        DDS = MAKE_FOUR_CHARACTER_CODE('D', 'D', 'S', ' '),

        DXT1 = MAKE_FOUR_CHARACTER_CODE('D', 'X', 'T', '1'), // BC1_UNORM
        DXT2 = MAKE_FOUR_CHARACTER_CODE('D', 'X', 'T', '2'), // BC2_UNORM
        DXT3 = MAKE_FOUR_CHARACTER_CODE('D', 'X', 'T', '3'), // BC2_UNORM
        DXT4 = MAKE_FOUR_CHARACTER_CODE('D', 'X', 'T', '4'), // BC3_UNORM
        DXT5 = MAKE_FOUR_CHARACTER_CODE('D', 'X', 'T', '5'), // BC3_UNORM
        ATI1 = MAKE_FOUR_CHARACTER_CODE('A', 'T', 'I', '1'), // BC4_UNORM
        BC4U = MAKE_FOUR_CHARACTER_CODE('B', 'C', '4', 'U'), // BC4_UNORM
        BC4S = MAKE_FOUR_CHARACTER_CODE('B', 'C', '4', 'S'), // BC4_SNORM
        ATI2 = MAKE_FOUR_CHARACTER_CODE('A', 'T', 'I', '2'), // BC5_UNORM
        BC5U = MAKE_FOUR_CHARACTER_CODE('B', 'C', '5', 'U'), // BC5_UNORM
        BC5S = MAKE_FOUR_CHARACTER_CODE('B', 'C', '5', 'S'), // BC5_SNORM
        RGBG = MAKE_FOUR_CHARACTER_CODE('R', 'G', 'B', 'G'), // R8G8_B8G8_UNORM
        GRBG = MAKE_FOUR_CHARACTER_CODE('G', 'R', 'B', 'G'), // G8R8_G8B8_UNORM
        YUY2 = MAKE_FOUR_CHARACTER_CODE('Y', 'U', 'Y', '2'), // YUY2
        UYVY = MAKE_FOUR_CHARACTER_CODE('U', 'Y', 'V', 'Y'),

        DX10 = MAKE_FOUR_CHARACTER_CODE('D', 'X', '1', '0'), // Any DXGI format
    };

    enum class PixelFormatFlags : uint32_t {
        AlphaPixels = 0x1,
        Alpha = 0x2,
        FourCC = 0x4,
        PAL8 = 0x20,
        RGB = 0x40,
        RGBA = RGB | AlphaPixels,
        YUV = 0x200,
        Luminance = 0x20000,
        LuminanceA = Luminance | AlphaPixels,
    };

    DDS_NO_DISCARD constexpr PixelFormatFlags operator&(PixelFormatFlags a, PixelFormatFlags b) {
        return static_cast<PixelFormatFlags>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
    }

    struct FilePixelFormat {
        uint32_t size;
        PixelFormatFlags flags;
        uint32_t fourCC;
        uint32_t bitCount;
        uint32_t rBitMask;
        uint32_t gBitMask;
        uint32_t bBitMask;
        uint32_t aBitMask;
    };
    static_assert(sizeof(FilePixelFormat) == 32, "DDS file pixel format size mismatch");

    enum HeaderFlags : uint32_t {
        Caps = 0x1,
        Height = 0x2,
        Width = 0x4,
        Pitch = 0x8,
        PixelFormat = 0x1000,
        Texture = Caps | Height | Width | PixelFormat,
        Mipmap = 0x20000,
        Volume = 0x800000,
        LinearSize = 0x00080000,
    };

    enum Caps2Flags : uint32_t {
        Cubemap = 0x200,
    };

    struct FileHeader {
        uint32_t size;
        HeaderFlags flags;
        uint32_t height;
        uint32_t width;
        uint32_t pitch;
        uint32_t depth;
        uint32_t mipmapCount;
        uint32_t reserved[11];
        FilePixelFormat pixelFormat;
        uint32_t caps1;
        uint32_t caps2;
        uint32_t caps3;
        uint32_t caps4;
        uint32_t reserved2;

        DDS_NO_DISCARD bool hasAlphaFlag() const;
    };
    static_assert(sizeof(FileHeader) == 124, "DDS Header size mismatch");

    inline bool FileHeader::hasAlphaFlag() const {
        return (pixelFormat.flags & PixelFormatFlags::AlphaPixels) == PixelFormatFlags::AlphaPixels;
    }

    /** An additional header for DX10 */
    struct Dx10Header {
        DXGI_FORMAT dxgiFormat;
        ResourceDimension resourceDimension;
        uint32_t miscFlags;
        uint32_t arraySize;
        uint32_t miscFlags2;
    };
    static_assert(sizeof(Dx10Header) == 20, "DDS DX10 Header size mismatch");

    struct Image {
        uint32_t numMips;
        uint32_t arraySize = 1;
        uint32_t width = 1;
        uint32_t height = 1;
        uint32_t depth = 1;
        ResourceDimension dimension;
        bool supportsAlpha = false;
        DXGI_FORMAT format;
        std::vector<uint8_t> data = {};
        std::vector<dds::span<uint8_t>> mipmaps;
    };

    template <typename T, typename U>
#if DDS_CPP_20
        requires ((std::is_enum_v<T>&& std::integral<std::underlying_type_t<T>>) || std::integral<T>) && requires (T t, U u) {
            { t& u } -> std::same_as<U>;
    }
#endif
    DDS_NO_DISCARD constexpr bool hasBit(T value, U bit) noexcept {
#if !DDS_CPP_20
        static_assert((std::is_enum_v<T> && std::is_integral_v<std::underlying_type_t<T>>) || std::is_integral_v<T>);
#endif
        return (value & bit) == bit;
    }

    DDS_NO_DISCARD constexpr uint32_t getBlockSize(const DXGI_FORMAT format) {
        switch (format) {
        case DXGI_FORMAT_BC1_UNORM:
        case DXGI_FORMAT_BC1_UNORM_SRGB:
        case DXGI_FORMAT_BC4_UNORM:
        case DXGI_FORMAT_BC4_SNORM:
            return 8;
        case DXGI_FORMAT_BC2_UNORM:
        case DXGI_FORMAT_BC2_UNORM_SRGB:
        case DXGI_FORMAT_BC3_UNORM:
        case DXGI_FORMAT_BC3_UNORM_SRGB:
        case DXGI_FORMAT_BC5_UNORM:
        case DXGI_FORMAT_BC5_SNORM:
        case DXGI_FORMAT_BC7_UNORM:
        case DXGI_FORMAT_BC7_UNORM_SRGB:
            return 16;
        default:
            return 0;
        }
    }

    DDS_NO_DISCARD constexpr uint32_t getBitsPerPixel(const DXGI_FORMAT format) {
        switch (format) {
        case DXGI_FORMAT_R32G32B32A32_TYPELESS:
        case DXGI_FORMAT_R32G32B32A32_FLOAT:
        case DXGI_FORMAT_R32G32B32A32_UINT:
        case DXGI_FORMAT_R32G32B32A32_SINT:
            return 128;

        case DXGI_FORMAT_R32G32B32_TYPELESS:
        case DXGI_FORMAT_R32G32B32_FLOAT:
        case DXGI_FORMAT_R32G32B32_UINT:
        case DXGI_FORMAT_R32G32B32_SINT:
            return 96;

        case DXGI_FORMAT_R16G16B16A16_TYPELESS:
        case DXGI_FORMAT_R16G16B16A16_FLOAT:
        case DXGI_FORMAT_R16G16B16A16_UNORM:
        case DXGI_FORMAT_R16G16B16A16_UINT:
        case DXGI_FORMAT_R16G16B16A16_SNORM:
        case DXGI_FORMAT_R16G16B16A16_SINT:
        case DXGI_FORMAT_R32G32_TYPELESS:
        case DXGI_FORMAT_R32G32_FLOAT:
        case DXGI_FORMAT_R32G32_UINT:
        case DXGI_FORMAT_R32G32_SINT:
        case DXGI_FORMAT_R32G8X24_TYPELESS:
        case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
        case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
        case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
            return 64;

        case DXGI_FORMAT_R10G10B10A2_TYPELESS:
        case DXGI_FORMAT_R10G10B10A2_UNORM:
        case DXGI_FORMAT_R10G10B10A2_UINT:
        case DXGI_FORMAT_R11G11B10_FLOAT:
        case DXGI_FORMAT_R8G8B8A8_TYPELESS:
        case DXGI_FORMAT_R8G8B8A8_UNORM:
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
        case DXGI_FORMAT_R8G8B8A8_UINT:
        case DXGI_FORMAT_R8G8B8A8_SNORM:
        case DXGI_FORMAT_R8G8B8A8_SINT:
        case DXGI_FORMAT_R16G16_TYPELESS:
        case DXGI_FORMAT_R16G16_FLOAT:
        case DXGI_FORMAT_R16G16_UNORM:
        case DXGI_FORMAT_R16G16_UINT:
        case DXGI_FORMAT_R16G16_SNORM:
        case DXGI_FORMAT_R16G16_SINT:
        case DXGI_FORMAT_R32_TYPELESS:
        case DXGI_FORMAT_D32_FLOAT:
        case DXGI_FORMAT_R32_FLOAT:
        case DXGI_FORMAT_R32_UINT:
        case DXGI_FORMAT_R32_SINT:
        case DXGI_FORMAT_R24G8_TYPELESS:
        case DXGI_FORMAT_D24_UNORM_S8_UINT:
        case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
        case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
        case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
        case DXGI_FORMAT_R8G8_B8G8_UNORM:
        case DXGI_FORMAT_G8R8_G8B8_UNORM:
        case DXGI_FORMAT_B8G8R8A8_UNORM:
        case DXGI_FORMAT_B8G8R8X8_UNORM:
        case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
        case DXGI_FORMAT_B8G8R8A8_TYPELESS:
        case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
        case DXGI_FORMAT_B8G8R8X8_TYPELESS:
        case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
            return 32;

        case DXGI_FORMAT_R8G8_TYPELESS:
        case DXGI_FORMAT_R8G8_UNORM:
        case DXGI_FORMAT_R8G8_UINT:
        case DXGI_FORMAT_R8G8_SNORM:
        case DXGI_FORMAT_R8G8_SINT:
        case DXGI_FORMAT_R16_TYPELESS:
        case DXGI_FORMAT_R16_FLOAT:
        case DXGI_FORMAT_D16_UNORM:
        case DXGI_FORMAT_R16_UNORM:
        case DXGI_FORMAT_R16_UINT:
        case DXGI_FORMAT_R16_SNORM:
        case DXGI_FORMAT_R16_SINT:
        case DXGI_FORMAT_B5G6R5_UNORM:
        case DXGI_FORMAT_B5G5R5A1_UNORM:
        case DXGI_FORMAT_B4G4R4A4_UNORM:
            return 16;

        case DXGI_FORMAT_R8_TYPELESS:
        case DXGI_FORMAT_R8_UNORM:
        case DXGI_FORMAT_R8_UINT:
        case DXGI_FORMAT_R8_SNORM:
        case DXGI_FORMAT_R8_SINT:
        case DXGI_FORMAT_A8_UNORM:
            return 8;

        case DXGI_FORMAT_R1_UNORM:
            return 1;

        case DXGI_FORMAT_BC1_TYPELESS:
        case DXGI_FORMAT_BC1_UNORM:
        case DXGI_FORMAT_BC1_UNORM_SRGB:
        case DXGI_FORMAT_BC4_TYPELESS:
        case DXGI_FORMAT_BC4_UNORM:
        case DXGI_FORMAT_BC4_SNORM:
            return 4;

        case DXGI_FORMAT_BC2_TYPELESS:
        case DXGI_FORMAT_BC2_UNORM:
        case DXGI_FORMAT_BC2_UNORM_SRGB:
        case DXGI_FORMAT_BC3_TYPELESS:
        case DXGI_FORMAT_BC3_UNORM:
        case DXGI_FORMAT_BC3_UNORM_SRGB:
        case DXGI_FORMAT_BC5_TYPELESS:
        case DXGI_FORMAT_BC5_UNORM:
        case DXGI_FORMAT_BC5_SNORM:
        case DXGI_FORMAT_BC6H_TYPELESS:
        case DXGI_FORMAT_BC6H_UF16:
        case DXGI_FORMAT_BC6H_SF16:
        case DXGI_FORMAT_BC7_TYPELESS:
        case DXGI_FORMAT_BC7_UNORM:
        case DXGI_FORMAT_BC7_UNORM_SRGB:
            return 8;
        default:
            return 0;
        }
    }

#ifdef VK_VERSION_1_0
    DDS_NO_DISCARD inline VkFormat getVulkanFormat(DXGI_FORMAT format, const bool alphaFlag) {
        switch (format) {
        case DXGI_FORMAT_BC1_UNORM: {
            if (alphaFlag)
                return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
            else
                return VK_FORMAT_BC1_RGB_UNORM_BLOCK;
        }
        case DXGI_FORMAT_BC1_UNORM_SRGB: {
            if (alphaFlag)
                return VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
            else
                return VK_FORMAT_BC1_RGB_SRGB_BLOCK;
        }

        case DXGI_FORMAT_BC2_UNORM:
            return VK_FORMAT_BC2_UNORM_BLOCK;
        case DXGI_FORMAT_BC2_UNORM_SRGB:
            return VK_FORMAT_BC2_SRGB_BLOCK;
        case DXGI_FORMAT_BC3_UNORM:
            return VK_FORMAT_BC3_UNORM_BLOCK;
        case DXGI_FORMAT_BC3_UNORM_SRGB:
            return VK_FORMAT_BC3_SRGB_BLOCK;
        case DXGI_FORMAT_BC4_UNORM:
            return VK_FORMAT_BC4_UNORM_BLOCK;
        case DXGI_FORMAT_BC4_SNORM:
            return VK_FORMAT_BC4_SNORM_BLOCK;
        case DXGI_FORMAT_BC5_UNORM:
            return VK_FORMAT_BC5_UNORM_BLOCK;
        case DXGI_FORMAT_BC5_SNORM:
            return VK_FORMAT_BC5_SNORM_BLOCK;
        case DXGI_FORMAT_BC7_UNORM:
            return VK_FORMAT_BC7_UNORM_BLOCK;
        case DXGI_FORMAT_BC7_UNORM_SRGB:
            return VK_FORMAT_BC7_SRGB_BLOCK;

            // 8-bit wide formats
        case DXGI_FORMAT_R8_UNORM:
            return VK_FORMAT_R8_UNORM;
        case DXGI_FORMAT_R8_UINT:
            return VK_FORMAT_R8_UINT;
        case DXGI_FORMAT_R8_SNORM:
            return VK_FORMAT_R8_SNORM;
        case DXGI_FORMAT_R8_SINT:
            return VK_FORMAT_R8_SINT;
#if defined(VK_KHR_maintenance5)
        case DXGI_FORMAT_A8_UNORM:
            return VK_FORMAT_A8_UNORM_KHR;
#endif

            // 16-bit wide formats
        case DXGI_FORMAT_R8G8_UNORM:
            return VK_FORMAT_R8G8_UNORM;
        case DXGI_FORMAT_R8G8_UINT:
            return VK_FORMAT_R8G8_SINT;
        case DXGI_FORMAT_R8G8_SNORM:
            return VK_FORMAT_R8G8_SNORM;
        case DXGI_FORMAT_R8G8_SINT:
            return VK_FORMAT_R8G8_SINT;

        case DXGI_FORMAT_R16_FLOAT:
            return VK_FORMAT_R16_SFLOAT;
        case DXGI_FORMAT_R16_UNORM:
            return VK_FORMAT_R16_UNORM;
        case DXGI_FORMAT_R16_UINT:
            return VK_FORMAT_R16_UINT;
        case DXGI_FORMAT_R16_SNORM:
            return VK_FORMAT_R16_SNORM;
        case DXGI_FORMAT_R16_SINT:
            return VK_FORMAT_R16_SINT;

        case DXGI_FORMAT_B5G5R5A1_UNORM:
            return VK_FORMAT_B5G5R5A1_UNORM_PACK16;
        case DXGI_FORMAT_B5G6R5_UNORM:
            return VK_FORMAT_B5G6R5_UNORM_PACK16;
        case DXGI_FORMAT_B4G4R4A4_UNORM:
            return VK_FORMAT_B4G4R4A4_UNORM_PACK16;

            // 32-bit wide formats
        case DXGI_FORMAT_R8G8B8A8_UNORM:
            return VK_FORMAT_R8G8B8A8_UNORM;
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
            return VK_FORMAT_R8G8B8A8_SRGB;
        case DXGI_FORMAT_R8G8B8A8_UINT:
            return VK_FORMAT_R8G8B8A8_UINT;
        case DXGI_FORMAT_R8G8B8A8_SNORM:
            return VK_FORMAT_R8G8B8A8_SNORM;
        case DXGI_FORMAT_R8G8B8A8_SINT:
            return VK_FORMAT_R8G8B8A8_SINT;
        case DXGI_FORMAT_B8G8R8A8_UNORM:
            return VK_FORMAT_B8G8R8A8_UNORM;
        case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
            return VK_FORMAT_B8G8R8A8_SRGB;

        case DXGI_FORMAT_R16G16_FLOAT:
            return VK_FORMAT_R16G16_SFLOAT;
        case DXGI_FORMAT_R16G16_UNORM:
            return VK_FORMAT_R16G16_UNORM;
        case DXGI_FORMAT_R16G16_UINT:
            return VK_FORMAT_R16G16_UINT;
        case DXGI_FORMAT_R16G16_SNORM:
            return VK_FORMAT_R16G16_SNORM;
        case DXGI_FORMAT_R16G16_SINT:
            return VK_FORMAT_R16G16_SINT;

        case DXGI_FORMAT_R32_FLOAT:
            return VK_FORMAT_R32_SFLOAT;
        case DXGI_FORMAT_R32_UINT:
            return VK_FORMAT_R32_UINT;
        case DXGI_FORMAT_R32_SINT:
            return VK_FORMAT_R32_SINT;

        case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
            return VK_FORMAT_E5B9G9R9_UFLOAT_PACK32;
        case DXGI_FORMAT_R10G10B10A2_UNORM:
            return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
        case DXGI_FORMAT_R10G10B10A2_UINT:
            return VK_FORMAT_A2B10G10R10_UINT_PACK32;
        case DXGI_FORMAT_R11G11B10_FLOAT:
            return VK_FORMAT_B10G11R11_UFLOAT_PACK32;

            // 64-bit wide formats
        case DXGI_FORMAT_R16G16B16A16_FLOAT:
            return VK_FORMAT_R16G16B16A16_SFLOAT;
        case DXGI_FORMAT_R16G16B16A16_SINT:
            return VK_FORMAT_R16G16B16A16_SINT;
        case DXGI_FORMAT_R16G16B16A16_UINT:
            return VK_FORMAT_R16G16B16A16_UINT;
        case DXGI_FORMAT_R16G16B16A16_UNORM:
            return VK_FORMAT_R16G16B16A16_UNORM;
        case DXGI_FORMAT_R16G16B16A16_SNORM:
            return VK_FORMAT_R16G16B16A16_SNORM;

        case DXGI_FORMAT_R32G32_FLOAT:
            return VK_FORMAT_R32G32_SFLOAT;
        case DXGI_FORMAT_R32G32_UINT:
            return VK_FORMAT_R32G32_UINT;
        case DXGI_FORMAT_R32G32_SINT:
            return VK_FORMAT_R32G32_SINT;

            // 96-bit wide formats
        case DXGI_FORMAT_R32G32B32_FLOAT:
            return VK_FORMAT_R32G32B32_SFLOAT;
        case DXGI_FORMAT_R32G32B32_UINT:
            return VK_FORMAT_R32G32B32_UINT;
        case DXGI_FORMAT_R32G32B32_SINT:
            return VK_FORMAT_R32G32B32_SINT;

            // 128-bit wide formats
        case DXGI_FORMAT_R32G32B32A32_FLOAT:
            return VK_FORMAT_R32G32B32A32_SFLOAT;
        case DXGI_FORMAT_R32G32B32A32_UINT:
            return VK_FORMAT_R32G32B32A32_UINT;
        case DXGI_FORMAT_R32G32B32A32_SINT:
            return VK_FORMAT_R32G32B32A32_SINT;

        case DXGI_FORMAT_R8G8_B8G8_UNORM:
        case DXGI_FORMAT_G8R8_G8B8_UNORM:
        case DXGI_FORMAT_YUY2:
        default:
            return VK_FORMAT_UNDEFINED;
        }
    }

    DDS_NO_DISCARD inline VkImageCreateInfo getVulkanImageCreateInfo(dds::Image* image, VkFormat format) {
        VkImageCreateInfo imageInfo = {};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        switch (image->dimension) {
        case Texture1D:
            imageInfo.imageType = VK_IMAGE_TYPE_1D;
            break;
        case Texture2D:
            imageInfo.imageType = VK_IMAGE_TYPE_2D;
            break;
        case Texture3D:
            imageInfo.imageType = VK_IMAGE_TYPE_3D;
            break;
        default:
            break;
        }
        imageInfo.format = format;
        imageInfo.extent.width = image->width;
        imageInfo.extent.height = image->height;
        imageInfo.extent.depth = image->depth;
        imageInfo.mipLevels = image->numMips;
        imageInfo.arrayLayers = image->arraySize;
        return imageInfo;
    }

    DDS_NO_DISCARD inline VkImageCreateInfo getVulkanImageCreateInfo(dds::Image* image) {
        return getVulkanImageCreateInfo(image, getVulkanFormat(image->format, image->supportsAlpha));
    }

    DDS_NO_DISCARD inline VkImageViewCreateInfo getVulkanImageViewCreateInfo(dds::Image* image, VkFormat format) {
        VkImageViewCreateInfo imageViewInfo = {};
        imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewInfo.format = format;
        imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewInfo.subresourceRange.baseMipLevel = 0;
        imageViewInfo.subresourceRange.levelCount = image->numMips;
        imageViewInfo.subresourceRange.baseArrayLayer = 0;
        imageViewInfo.subresourceRange.layerCount = image->arraySize;
        return imageViewInfo;
    }

    DDS_NO_DISCARD inline VkImageViewCreateInfo getVulkanImageViewCreateInfo(dds::Image* image) {
        return getVulkanImageViewCreateInfo(image, getVulkanFormat(image->format, image->supportsAlpha));
    }
#endif // #ifdef VK_VERSION_1_0

    /**
     * When using this function, image->data will always be empty and the mipmap span's will reference
     * the passed pointer.
     */
    DDS_NO_DISCARD inline ReadResult readImage(std::uint8_t* ptr, std::size_t fileSize, dds::Image* image) {
        // Read the magic number
        auto* ddsMagic = reinterpret_cast<uint32_t*>(ptr);
        ptr += sizeof(uint32_t);

        // Read the header
        if (fileSize < sizeof(dds::FileHeader))
            return dds::ReadResult::Failure;
        const auto* header = reinterpret_cast<const dds::FileHeader*>(ptr);
        ptr += sizeof(dds::FileHeader);

        // Validate header. A DWORD (magic number) containing the four character code value 'DDS '
        // (0x20534444).
        if (*ddsMagic != dds::DdsMagicNumber::DDS)
            return dds::ReadResult::Failure;

        const dds::Dx10Header* additionalHeader = nullptr;
        if (hasBit(header->pixelFormat.flags, PixelFormatFlags::FourCC) &&
            hasBit(header->pixelFormat.fourCC, static_cast<uint32_t>(DdsMagicNumber::DX10))) {
            additionalHeader = reinterpret_cast<const dds::Dx10Header*>(ptr);
            ptr += sizeof(dds::Dx10Header);

            // "If the DDS_PIXELFORMAT dwFlags is set to DDPF_FOURCC and a dwFourCC is
            // set to "DX10", then the total file size needs to be at least 148
            // bytes."
            if (fileSize < 148)
                return dds::ReadResult::InvalidSize;

            image->arraySize = additionalHeader->arraySize;
            image->format = additionalHeader->dxgiFormat;
            image->dimension = additionalHeader->resourceDimension;
        }

        // Determine format information
        auto getFormatInfo = [](const dds::FileHeader* header) -> DXGI_FORMAT {
            auto& pf = header->pixelFormat;
            if (hasBit(pf.flags, PixelFormatFlags::FourCC)) {
                switch (pf.fourCC) {
                    // clang-format off
                case DXT1:            return DXGI_FORMAT_BC1_UNORM;
                case DXT2: case DXT3: return DXGI_FORMAT_BC2_UNORM;
                case DXT4: case DXT5: return DXGI_FORMAT_BC3_UNORM;
                case ATI1: case BC4U: return DXGI_FORMAT_BC4_UNORM;
                case BC4S:            return DXGI_FORMAT_BC4_SNORM;
                case ATI2: case BC5U: return DXGI_FORMAT_BC5_UNORM;
                case BC5S:            return DXGI_FORMAT_BC5_SNORM;
                case RGBG:            return DXGI_FORMAT_R8G8_B8G8_UNORM;
                case GRBG:            return DXGI_FORMAT_G8R8_G8B8_UNORM;
                case YUY2:            return DXGI_FORMAT_YUY2;
                default:              return DXGI_FORMAT_UNKNOWN;
                    // clang-format on
                }
            }

            // TODO: Write more of this bitmask stuff to determine formats.
            if (hasBit(pf.flags, PixelFormatFlags::RGB)) {
                switch (pf.bitCount) {
                case 32: {
                    if (pf.rBitMask == 0xFF && pf.gBitMask == 0xFF00 && pf.bBitMask == 0xFF0000 && pf.aBitMask == 0xFF000000)
                        return DXGI_FORMAT_R8G8B8A8_UNORM;
                    if (pf.rBitMask == 0xFF0000 && pf.gBitMask == 0xFF00 && pf.bBitMask == 0xFF && pf.aBitMask == 0xFF000000)
                        return DXGI_FORMAT_B8G8R8A8_UNORM;
                    if (pf.rBitMask == 0xFFFF && pf.gBitMask == 0xFFFF0000 && pf.bBitMask == 0 && pf.aBitMask == 00)
                        return DXGI_FORMAT_R16G16_UNORM;
                    if (pf.rBitMask == 0x3FF && pf.gBitMask == 0xFFC00 && pf.bBitMask == 0x3FF00000)
                        return DXGI_FORMAT_R10G10B10A2_UNORM;
                    break;
                }
                case 16: {
                    if (pf.rBitMask == 0xFFFF)
                        return DXGI_FORMAT_R16_UNORM;
                    if (pf.rBitMask == 0xFF && pf.gBitMask == 0xFF00)
                        return DXGI_FORMAT_R8G8_UNORM;
                    if (pf.rBitMask == 0xF && pf.gBitMask == 0xF0 && pf.bBitMask == 0xF00 && pf.aBitMask == 0xF000)
                        return DXGI_FORMAT_R8G8B8A8_UNORM;
                    if (pf.rBitMask == 0xF00 && pf.gBitMask == 0xF0 && pf.bBitMask == 0xF && pf.aBitMask == 0xF000)
                        return DXGI_FORMAT_B8G8R8A8_UNORM;
                    if (pf.rBitMask == 0xF800 && pf.gBitMask == 0x07E0 && pf.bBitMask == 0x1F && pf.aBitMask == 0) {
                        if (pf.aBitMask == 0)
                            return DXGI_FORMAT_B5G6R5_UNORM;
                        if (pf.aBitMask == 0x8000)
                            return DXGI_FORMAT_B5G5R5A1_UNORM;
                    }
                    break;
                }
                case 8: {
                    if (pf.rBitMask == 0xFF)
                        return DXGI_FORMAT_R8_UNORM;
                    if (pf.aBitMask == 0xFF)
                        return DXGI_FORMAT_A8_UNORM;
                    break;
                }
                }
            }

            return DXGI_FORMAT_UNKNOWN;
            };

        // We'll always trust the DX10 header.
        if (additionalHeader == nullptr) {
            image->format = getFormatInfo(header);
            if (image->format == DXGI_FORMAT_UNKNOWN)
                return ReadResult::UnsupportedFormat;

            if (header->flags & HeaderFlags::Volume || header->caps2 & Caps2Flags::Cubemap) {
                image->dimension = Texture3D;
            }
            else {
                image->dimension = header->height > 1 ? Texture2D : Texture1D;
            }

            image->arraySize = 1u;
        }

        const auto blockSizeBytes = getBlockSize(image->format);
        const auto bitsPerPixel = getBitsPerPixel(image->format);
        if (!blockSizeBytes && !bitsPerPixel)
            return dds::ReadResult::UnsupportedFormat;

        auto computeMipmapSize = [bitsPerPixel, blockSizeBytes](DXGI_FORMAT dxgiFormat, uint32_t width, uint32_t height) -> uint32_t {
            // Instead of checking each format enum each we'll check for the range in
            // which the BCn compressed formats are.
            if ((dxgiFormat >= DXGI_FORMAT_BC1_TYPELESS && dxgiFormat <= DXGI_FORMAT_BC5_SNORM) ||
                (dxgiFormat >= DXGI_FORMAT_BC6H_TYPELESS && dxgiFormat <= DXGI_FORMAT_BC7_UNORM_SRGB)) {
                auto pitch = max(1u, (width + 3) / 4) * blockSizeBytes;
                return pitch * max(1u, (height + 3) / 4);
            }

            // These formats are special
            if (dxgiFormat == DXGI_FORMAT_R8G8_B8G8_UNORM || dxgiFormat == DXGI_FORMAT_G8R8_G8B8_UNORM) {
                return ((width + 1) >> 1) * 4 * height;
            }

            return max(1u, static_cast<uint32_t>((width * bitsPerPixel + 7) / 8)) * height;
            };

        // TODO: Support files with multiple resources.
        if (image->arraySize > 1)
            return ReadResult::UnsupportedFormat;

        // arraySize is populated with the additional DX10 header.
        uint64_t totalSize = 0;
        for (uint32_t i = 0; i < image->arraySize; ++i) {
            auto mipmaps = max(header->mipmapCount, 1u);
            auto width = header->width;
            auto height = header->height;

            image->mipmaps.reserve(mipmaps);
            for (uint32_t mip = 0; mip < mipmaps && width != 0; ++mip) {
                uint32_t size = computeMipmapSize(image->format, width, height);
                totalSize += static_cast<uint64_t>(size);

                image->mipmaps.emplace_back(ptr, size);
                ptr += size;

                width = max(width / 2, 1u);
                height = max(height / 2, 1u);
            }
        }

        image->numMips = max(header->mipmapCount, 1u);
        image->width = header->width;
        image->height = header->height;
        image->supportsAlpha = header->hasAlphaFlag();

        return dds::ReadResult::Success;
    }

    /**
     * Reads a file from the given path and parses the image. The entire file contents will be in image->data
     * after this function returns successfully. The mipmap spans will point to within image->data.
     */
#if DDS_CPP_17 && defined(DDS_USE_STD_FILESYSTEM)
    DDS_NO_DISCARD inline dds::ReadResult readFile(const fs::path& filepath, dds::Image* image) {
#else
    DDS_NO_DISCARD inline dds::ReadResult readFile(const std::string & filepath, dds::Image * image) {
#endif
        std::ifstream filestream(filepath, std::ios::binary | std::ios::in);
        if (!filestream.is_open())
            return dds::ReadResult::Failure;

        // Read the file into a vector.
        filestream.seekg(0, std::ios::end);
        auto fileSize = filestream.tellg();
        image->data.resize(fileSize);
        filestream.seekg(0);
        filestream.read(reinterpret_cast<char*>(image->data.data()), fileSize);
        return readImage(image->data.data(), fileSize, image);
    }

} // namespace dds
