Texture2D inputTexture : register(t0);
RWTexture2D<float4> outputTexture : register(u0);

[numthreads(16, 16, 1)]
void CSMain(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    int2 texCoord = int2(dispatchThreadID.xy);
    float4 texColor = inputTexture.Load(int3(texCoord, 0));
    float luminance = dot(texColor.rgb, float3(0.299, 0.587, 0.114));
    float4 grayColor = float4(luminance, luminance, luminance, texColor.a);
    outputTexture[texCoord] = grayColor;
}