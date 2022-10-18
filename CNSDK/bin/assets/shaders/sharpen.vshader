// Compatibility definitions to make HLSL/GLSL work with minimal changes
#if USE_HLSL == 1

#define uniform /**/

#define InTex input.Tex
#define OutTexCoord output.Tex
#define OutPos output.Pos

#else

#define float2 vec2
#define float3 vec3
#define float4 vec4
#define float4x4 mat4
#define fmod mod
#define lerp mix
#define mul(x,y) ((x)*(y))

#define InTex aTexCoord
#define OutTexCoord TexCoord
#define OutPos gl_Position

#endif

// Shader input/output signatures
#if USE_HLSL == 1

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

#else

layout (location = 0) in vec2 aTexCoord;
out vec2 TexCoord;

#endif

// Global variables (uniforms for GLSL and constant buffer for HLSL)
#if USE_HLSL == 1
cbuffer LeiaSharpenShaderConstantBufferData : register(b0)
{
#endif

    uniform float gamma;
    uniform float sharpeningCenter;
    uniform int sharpeningValueCount;
    uniform float padding;

    uniform float4x4 rectTransform;

    uniform float4 textureInvSize;

    uniform float4 sharpeningValues[18];

#if USE_HLSL == 1
};
#endif

#if USE_HLSL == 1
PSInput main(VSInput input)
#else
void main()
#endif
{
#if USE_HLSL == 1
    PSInput output = (PSInput)0;
#endif

    // Scale and offset rect coordinates.
    OutTexCoord = mul(rectTransform, float4(InTex, 0.0, 1.0)).xy;

    // Scale and offset position.
    OutPos = float4((OutTexCoord * 2.0) - 1.0, 0.0, 1.0);

#if UV_TOP_LEFT == 1
    OutTexCoord.y = 1.0 - OutTexCoord.y;
#endif

#if USE_HLSL == 1
    return output;
#endif
}
