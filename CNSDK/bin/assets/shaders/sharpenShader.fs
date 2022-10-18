precision mediump float;

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D viewTexture;

uniform float gamma;
uniform float sharpeningCenter;
uniform vec4 textureInvSize;
uniform vec4 sharpeningValues[18];
uniform int sharpeningValueCount;

vec3 GammaToLinear(vec3 col)
{
    return vec3(pow(col.x, gamma), pow(col.y, gamma), pow(col.z, gamma));
}

vec3 LinearToGamma(vec3 col)
{
    float invGamma = 1.0 / gamma; // todo: pass-in invGamma instead of computing per-pixel.
    return vec3(pow(col.x, invGamma), pow(col.y, invGamma), pow(col.z, invGamma));
}


void main()
{

    vec4 final_color = vec4(0, 0, 0, 1);
    final_color.rgb = sharpeningCenter * GammaToLinear(texture(viewTexture, TexCoord).rgb);

#if ACT_OPTIMIZED
    float coef_sum = 0.0;
    for (int index = 0; index < sharpeningValueCount/2; index++)
    {
        coef_sum += sharpeningValues[2*index].z; // calculating sum of all coefs
    }
    vec2 uv1 = TexCoord + sharpeningValues[4].xy * textureInvSize.xy; // tapping at order +3
    final_color.rgb -= coef_sum * GammaToLinear(texture(viewTexture, uv1).rgb);
    vec2 uv2 = TexCoord + sharpeningValues[5].xy * textureInvSize.xy; // tapping at order -3
    final_color.rgb -= coef_sum * GammaToLinear(texture(viewTexture, uv2).rgb);
    // NOTE: need to guard against case where we have less than 3 ACT coefs, which happens if numViews < 6 
    // NOTE: for numViews == 6 we only need to tap once with twice the coef
#else
    for (int index = 0; index < sharpeningValueCount; index++)
    {
        vec2 uv = TexCoord + sharpeningValues[index].xy * textureInvSize.xy;
        final_color.rgb -= sharpeningValues[index].z * GammaToLinear(texture(viewTexture, uv).rgb);
    }
#endif


    final_color.rgb = clamp(final_color.rgb, 0.0, 1.0);
    final_color.rgb = LinearToGamma(final_color.rgb);
    FragColor = final_color;
    //Uncomment to do a passthrough.
    //FragColor = texture(viewTexture, TexCoord);
}