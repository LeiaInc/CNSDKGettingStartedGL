layout (location = 0) in vec2 aTexCoord;
out vec2 TexCoord;

uniform mat4 rectTransform;

void main()
{
    // Scale and offset rect coordinates.
    TexCoord  = (rectTransform * vec4(aTexCoord, 0, 1)).xy;

    // Convert from texture UVs to clipspace positions ([0,1] -> [-1,1])
    gl_Position = vec4((TexCoord * 2.0) - 1.0, 0, 1);
}