#define TEXCOORD_ORIGIN_TOP_LEFT
//#define TEXCOORD_ORIGIN_BOTTOM_LEFT

struct VSInput
{
    float3 Pos : POSITION;
    float3 Col : COLOR;
    float2 Tex : TEXCOORD;
};

struct PSInput
{
    float4               Pos : SV_POSITION;
    noperspective float2 Tex  : TEXCOORD;
};

Texture2D    viewTexture : register(t0);
SamplerState viewSampler : register(s0);

cbuffer LeiaSharpenShaderConstantBufferData : register(b0)
{
    float gamma;
    float sharpeningCenter;
    int sharpeningValueCount;
    float padding;

    float4x4 rectTransform;

    float4 textureInvSize;

    float4 sharpeningValues[18];
};

float3 GammaToLinear(float3 col)
{
    return float3(pow(col.x, gamma), pow(col.y, gamma), pow(col.z, gamma));
}

float3 LinearToGamma(float3 col)
{
    float invGamma = 1.0 / gamma; // todo: pass-in invGamma instead of computing per-pixel.
    return float3(pow(col.x, invGamma), pow(col.y, invGamma), pow(col.z, invGamma));
}

PSInput VSMain(VSInput input)
{
    PSInput output = (PSInput)0;

    // Scale and offset rect coordinates.
    output.Tex = mul(rectTransform, float4(input.Tex, 0, 1)).xy;

    // Scale and offset position.
    output.Pos = float4((output.Tex * 2.0) - 1.0, 0, 1);

#ifdef TEXCOORD_ORIGIN_TOP_LEFT
    output.Tex.y = 1.0f - output.Tex.y;
#endif

    return output;
}

float4 PSMain(PSInput input) : SV_Target0
{
    float4 final_color = float4(0, 0, 0, 1);
    final_color.rgb = sharpeningCenter * GammaToLinear(viewTexture.Sample(viewSampler, input.Tex).rgb);

#if ACT_OPTIMIZED
    float coef_sum = 0.0;
    for (int index = 0; index < sharpeningValueCount/2; index++)
    {
        coef_sum += sharpeningValues[2*index].z; // calculating sum of all coefs
    }
    float2 uv1 = input.Tex + sharpeningValues[4].xy * textureInvSize.xy; // tapping at order +3
    final_color.rgb -= coef_sum * GammaToLinear(viewTexture.Sample(viewSampler, uv1).rgb);
    float2 uv2 = input.Tex + sharpeningValues[5].xy * textureInvSize.xy; // tapping at order -3
    final_color.rgb -= coef_sum * GammaToLinear(viewTexture.Sample(viewSampler, uv2).rgb);
    // NOTE: need to guard against case where we have less than 3 ACT coefs, which happens if numViews < 6 
    // NOTE: for numViews == 6 we only need to tap once with twice the coef
#else
    for (int index = 0; index < sharpeningValueCount; index++)
    {
        float2 uv = input.Tex + sharpeningValues[index].xy * textureInvSize.xy;
        final_color.rgb -= sharpeningValues[index].z * GammaToLinear(viewTexture.Sample(viewSampler, uv).rgb);
    }
#endif

    final_color.rgb = clamp(final_color.rgb, 0.0, 1.0);
    final_color.rgb = LinearToGamma(final_color.rgb);
    
    //Uncomment to do a passthrough.
    //final_color = viewTexture.Sample(viewSampler, input.Tex);

    return final_color;
}