#version 330 core

in vec3 color;
in vec2 uv;
uniform sampler2D mainTexture;
uniform float blue;
out vec4 fragColor;

void main()
{
	fragColor = texture(mainTexture, uv) * vec4(1, blue, 1, 1);
}