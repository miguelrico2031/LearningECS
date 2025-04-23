#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inUv;
out vec3 color;
out vec2 uv;

uniform mat4 modelViewProj;

void main()
{
	color = inColor;
	uv = inUv;
	gl_Position = modelViewProj * vec4(inPos, 1.0);
}