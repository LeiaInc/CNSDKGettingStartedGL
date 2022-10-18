#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 aTexCoord;

float SCR_WIDTH = 800 / 5;
float SCR_HEIGHT = 600 / 5;
float widthZ = 1.0/SCR_WIDTH;
float heightZ = 1.0/SCR_HEIGHT;

uniform mat4 projection;

uniform vec4 uiPos;
uniform vec4 uiSrc;
uniform vec2 uiBaseSrc;


out vec2 TexCoord;

float getNorm(float currentValue, float minValue, float maxValue)
{
    return (currentValue - minValue)/(maxValue - minValue);
}

void main()
{
    int idx = int(aCol.x);
    vec4 locPos =   vec4(aPos.xz, 0.0, 1.0);

    locPos.x *= widthZ * uiPos.z;
    locPos.y *= heightZ * uiPos.w;
        

    float srcWZ = 1.0/uiBaseSrc.x;
    float srcHZ = 1.0/uiBaseSrc.y;

    if(idx == 3)
    {
        locPos.x = uiPos.x * widthZ; 
        locPos.y = uiPos.y * heightZ;

              TexCoord = vec2(0,0);

        TexCoord.x = uiSrc.x * srcWZ;
        TexCoord.y = uiSrc.y * srcHZ;

    }
    else if(idx == 2)
    {
        locPos.x = uiPos.x * widthZ; 
        locPos.y = (uiPos.y + uiPos.w) * heightZ;
  

        TexCoord = vec2(0,1);

        TexCoord.x = uiSrc.x * srcWZ;
        TexCoord.y = (uiSrc.y + uiSrc.w )* srcHZ;
    }
    else if(idx == 1)
    {
        locPos.x = (uiPos.x + uiPos.z) * widthZ; 
        locPos.y = (uiPos.y + uiPos.w) * heightZ;

         TexCoord = vec2(1,1);

        TexCoord.x = (uiSrc.x + uiSrc.z) * srcWZ;
        TexCoord.y = (uiSrc.y + uiSrc.w ) * srcHZ;

    }
    else if(idx == 0)
    {
        locPos.x = (uiPos.x + uiPos.z) * widthZ;  
        locPos.y = uiPos.y * heightZ;


                TexCoord = vec2(1,0);

        TexCoord.x = (uiSrc.x + uiSrc.z) * srcWZ;
        TexCoord.y = uiSrc.y * srcHZ;
    }

    locPos.x =  -1 + locPos.x;
    locPos.y =  1 - locPos.y;

    TexCoord.y = 1 - TexCoord.y;
    

    gl_Position = locPos;

}