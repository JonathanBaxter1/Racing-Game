#version 430
out vec4 FragColor;

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

uniform sampler2D diffuseMapTex;
uniform sampler2D specularMapTex;
uniform vec3 viewPos;
uniform vec3 lightDir;
uniform float shininess;

void main()
{
	FragColor = texture(diffuseMapTex, texCoord);
}
