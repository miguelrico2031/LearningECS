#version 330 core

in vec3 normal;
in vec2 uv;
uniform sampler2D mainTexture;
uniform vec4 color;
uniform float tiling;
out vec4 fragColor;

void main()
{
	fragColor = texture(mainTexture, uv * tiling) * color;	
}