#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inUv;
out vec3 color;
out vec2 uv;

void main()
{
	gl_Position = vec4(inPos.x, inPos.y, inPos.z, 1.0);
	color = inColor;
	uv = inUv;
}