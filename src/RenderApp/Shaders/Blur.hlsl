Texture2D<float4> BrightImage : register(t0);
RWTexture2D<float4> BlurImage : register(u0);

[numthreads(16, 16, 1)]
void CSMain(uint3 DTid : SV_DispatchThreadID)
{
    float4 color = float4(0, 0, 0, 0);
    int radius = 4;

    for (int x = -radius; x <= radius; x++)
    {
        for (int y = -radius; y <= radius; y++)
        {
            color += BrightImage.Load(int3(DTid.xy + int2(x, y), 0));
        }
    }

    color /= (float) ((radius * 2 + 1) * (radius * 2 + 1));
    BlurImage[DTid.xy] = color;
}
