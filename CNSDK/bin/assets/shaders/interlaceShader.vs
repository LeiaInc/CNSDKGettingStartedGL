layout(location = 0) in vec2 aTexCoord;

out vec2 TexCoord;
out vec2 ScreenTexCoord;

uniform mat4 textureTransform;
uniform mat4 rectTransform;

void main()
{
    // Scale and offset texture coordinates.
    TexCoord = (textureTransform * vec4(aTexCoord, 0, 1)).xy;

    // Scale and offset position of rectangle being rendered.
    ScreenTexCoord = (rectTransform * vec4(aTexCoord, 0, 1)).xy;

    // Convert from texture UVs to clipspace positions ([0,1] -> [-1,1])
    gl_Position = vec4((ScreenTexCoord * 2.0) - 1.0, 0, 1);
}