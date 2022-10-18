layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 view;
uniform mat4 world;
uniform mat4 worldViewProj;
    
out vec2 TexCoord;
out vec4 Color;
flat out int TexIdx;

void main()
{
    gl_Position =  worldViewProj * vec4((aPos), 1.0f);
    TexIdx = int(aCol.r);
    TexCoord = aTexCoord;
    Color = vec4(aCol, 1.0f);
}