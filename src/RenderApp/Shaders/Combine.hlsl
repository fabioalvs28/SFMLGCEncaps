Texture2D<float4> OriginalImage : register(t0);
Texture2D<float4> BlurImage : register(t1);
RWTexture2D<float4> OutputImage : register(u0);

[numthreads(16, 16, 1)]
void CSMain(uint3 DTid : SV_DispatchThreadID)
{
    float4 originalColor = OriginalImage.Load(int3(DTid.xy, 0));
    float4 blurColor = BlurImage.Load(int3(DTid.xy, 0));
    OutputImage[DTid.xy] = originalColor + blurColor; // Combine original with bloom
}