#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 aTexCoord;

float SCR_WIDTH = 800 / 5;
float SCR_HEIGHT = 600 / 5;
float widthZ = 1.0/SCR_WIDTH;
float heightZ = 1.0/SCR_HEIGHT;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;
uniform mat4 worldViewProj;

uniform vec4 worldPos;

uniform vec3 vertexPosition_worldspace;

uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform vec3 particleCenter_wordspace;

uniform mat4 CameraPosition;
uniform mat4 ViewProjectionMatrix;
    
out vec2 TexCoord;
out vec4 Color;

float getNorm(float currentValue, float minValue, float maxValue)
{
    return (currentValue - minValue)/(maxValue - minValue);
}



void main()
{
    vec3 taPos = aPos;
    //taPos.z = aPos.y;
    //taPos.y = aPos.z;

    //gl_Position =  worldViewProj * vec4((taPos * 5.0), 1.0f);

    vec3 vertexPosition_worldspace = 
		particleCenter_wordspace
		+ CameraRight_worldspace * aPos.x * 5.0
		+ CameraUp_worldspace * aPos.z * 5.0;


	// Output position of the vertex
	gl_Position = ViewProjectionMatrix * vec4(vertexPosition_worldspace, 1.0f);

    
    TexCoord = aTexCoord;
}