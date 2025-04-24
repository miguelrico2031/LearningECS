#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUv;
out vec3 normal;
out vec2 uv;

uniform mat4 modelViewProj;

void main()
{
	gl_Position = modelViewProj * vec4(inPos, 1.0);
	normal = inNormal;
	uv = inUv;
}