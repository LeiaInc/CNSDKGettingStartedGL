out vec4 FragColor;
  
in vec2 TexCoord;
in vec4 Color;
flat in int TexIdx;

uniform sampler2D ourTexture[5];

void main()
{
    //vec4 outColor = texture(ourTexture[TexIdx], TexCoord); // Output texture
    vec4 outColor = Color;                                   // Output color

    FragColor = outColor;
}