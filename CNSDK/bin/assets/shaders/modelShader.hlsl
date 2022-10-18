struct VSInput
{
    float3 Pos : POSITION;
    float3 Col : COLOR;
    float2 Tex : TEXCOORD;
};

struct PSInput
{
    float4               Pos : SV_POSITION;
    float4               Col : COLOR;
    noperspective float2 Tex : TEXCOORD;
};

cbuffer LeiaSharpenShaderConstantBufferData : register(b0)
{
    float4x4 worldViewProj;
};

PSInput VSMain(VSInput input)
{
    PSInput output = (PSInput)0;

    output.Pos = mul(worldViewProj, float4(input.Pos, 1.0f));
    output.Col = float4(input.Col, 1.0f);
    output.Tex = input.Tex;

    return output;
}

float4 PSMain(PSInput input) : SV_Target0
{
    return input.Col;
}
