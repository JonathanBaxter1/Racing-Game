#version 430

in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D diffuseMapTex;

void main()
{
	FragColor = texture(diffuseMapTex, texCoord);
}
