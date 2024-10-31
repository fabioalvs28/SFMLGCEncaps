#include <stdio.h>
#include <memory.h>
#include "savedds.h"

#define STB_DXT_IMPLEMENTATION
#pragma warning(push)
#pragma warning(disable: 4244) // conversion from 'int' to 'unsigned char', possible loss of data
#include "stb_dxt.h"
#pragma warning(pop)
#undef STB_DXT_IMPLEMENTATION

unsigned char* compress_to_dxt(const unsigned char* pData, int width, int height, int bpp, int* outDstSize)
{
    const int blockSize = bpp == 24 ? 8 : 16;
    const int dstSize = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
    unsigned char* dst = new unsigned char[dstSize];
    if (!dst)
    {
        *outDstSize = 0;
        return NULL;
    }
    *outDstSize = dstSize;

    const int c = bpp / 8; // num channels
    const int stride = width * c;
    unsigned char* pd = dst;
    for (int j = 0; j < height; j += 4)
    {
        for (int i = 0; i < width; i += 4)
        {
            const unsigned char* row[4] = {
                &pData[(j + 0) * stride + i * c],
                &pData[(j + 1) * stride + i * c],
                &pData[(j + 2) * stride + i * c],
                &pData[(j + 3) * stride + i * c]
            };

            unsigned char block[16][4];
            for (int b = 0; b < 16; ++b)
            {
                block[b][0] = row[b / 4][(b % 4) * c + 0];
                block[b][1] = row[b / 4][(b % 4) * c + 1];
                block[b][2] = row[b / 4][(b % 4) * c + 2];
                block[b][3] = bpp == 24 ? 0 : row[b / 4][(b % 4) * c + 3];
            }

            stb_compress_dxt_block(pd, &block[0][0], bpp == 24 ? 0 : 1, STB_DXT_HIGHQUAL /* or STB_DXT_NORMAL */);
            pd += blockSize;
        }
    }
    return dst;
}

void savedds(const char* filename, const unsigned char* pData, int width, int height, int bpp)
{
    if (bpp != 24 && bpp != 32)
        return;

    if ((width % 4) || (height % 4))
        return;

    FILE* f;
    fopen_s(&f, filename, "wb");
    if (!f)
        return;

    char filecode[5] = "DDS ";
    fwrite(filecode, 4, 1, f);

    DDSURFACEDESC2 ddsd;
    memset(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = 0;
    ddsd.dwWidth = width;
    ddsd.dwHeight = height;
    ddsd.lPitch = width * height;
    ddsd.dwMipMapCount = 0;
    ddsd.ddpfPixelFormat.dwSize = sizeof(ddsd.ddpfPixelFormat);
    ddsd.ddpfPixelFormat.dwFlags = DDSF_FOURCC;
    ddsd.ddpfPixelFormat.dwFourCC = bpp == 24 ? FOURCC_DXT1 : FOURCC_DXT5;
    fwrite(&ddsd, sizeof(ddsd), 1, f);

    int dstSize = 0;
    unsigned char* dst = compress_to_dxt(pData, width, height, bpp, &dstSize);
    if (dst)
    {
        fwrite(dst, dstSize, 1, f);
        delete[] dst;
    }

    fclose(f);
}
