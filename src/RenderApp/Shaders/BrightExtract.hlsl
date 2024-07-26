Texture2D<float4> InputImage : register(t0);
RWTexture2D<float4> BrightImage : register(u0);

[numthreads(16, 16, 1)]
void CSMain(uint3 DTid : SV_DispatchThreadID)
{
    float3 color = InputImage.Load(int3(DTid.xy, 0)).rgb;
    float brightness = dot(color, float3(0.2126, 0.7152, 0.0722)); // Luminance
    if (brightness > 0.7)
    { // Threshold
        BrightImage[DTid.xy] = float4(color, 1.0);
    }
    else
    {
        BrightImage[DTid.xy] = float4(0, 0, 0, 1.0);
    }
}