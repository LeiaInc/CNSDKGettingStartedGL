#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    vec4 outColor = texture(ourTexture, TexCoord);

    if (outColor.a <= 0.5)
        discard;

    FragColor = outColor;
}