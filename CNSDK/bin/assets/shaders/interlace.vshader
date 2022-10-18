// Compatibility definitions to make HLSL/GLSL work with minimal changes
#if USE_HLSL == 1

#define uniform /**/

#define InTex input.Tex
#define OutTexCoord output.TexCoord
#define OutScreenTexCoord output.ScreenTexCoord 
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
#define OutScreenTexCoord ScreenTexCoord 
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
    float4               Pos            : SV_POSITION;
    noperspective float2 TexCoord       : TEXCOORD;
    noperspective float2 ScreenTexCoord : TEXCOORD2;
};

#else

layout(location = 0) in vec2 aTexCoord;

out vec2 TexCoord;
out vec2 ScreenTexCoord;

#endif

// Global variables (uniforms for GLSL and constant buffer for HLSL)
#if USE_HLSL == 1
cbuffer LeiaInterlaceShaderConstantBufferData : register(b0)
{
#endif
    uniform float  hardwareViewsX;
    uniform float  hardwareViewsY;
    uniform float  viewResX;
    uniform float  viewResY;

    uniform float  viewportX;
    uniform float  viewportY;
    uniform float  viewportWidth;
    uniform float  viewportHeight;

    uniform float  minView;
    uniform float  maxView;
    uniform float  n;
    uniform float  d_over_n;

    uniform float  p_over_du;
    uniform float  p_over_dv;
    uniform float  colorSlant;
    uniform float  colorInversion;

    uniform float  faceX;
    uniform float  faceY;
    uniform float  faceZ;
    uniform float  pixelPitch;

    uniform float  du;
    uniform float  dv;
    uniform float  s;
    uniform float  cos_theta;

    uniform float  sin_theta;
    uniform float  peelOffset;
    uniform float  No;
    uniform float  displayResX;

    uniform float  displayResY;
    uniform float  blendViews;
    uniform int    interlaceMode;
    uniform int    debugMode;

    uniform int    perPixelCorrection;
    uniform int    viewTextureType;
    uniform float  reconvergenceAmount;
    uniform float  softwareViews;

    uniform float  absReconvergenceAmount;
    uniform float  padding0;
    uniform float  padding1;
    uniform float  padding2;

    uniform float4x4 textureTransform;

    uniform float4x4 rectTransform;
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

    // Scale and offset texture coordinates.
    OutTexCoord = mul(textureTransform, float4(InTex, 0.0, 1.0)).xy;

    // Scale of offset position of rectangle being rendered.
    OutScreenTexCoord = mul(rectTransform, float4(InTex, 0.0, 1.0)).xy;

    // Scale and offset position.
    OutPos = float4((OutScreenTexCoord * 2.0) - 1.0, 0.0, 1.0);

#if UV_TOP_LEFT == 1
    OutTexCoord.y = 1.0 - OutTexCoord.y;
#endif

#if USE_HLSL == 1
    return output;
#endif
}