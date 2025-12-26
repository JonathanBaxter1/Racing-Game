#version 430
out vec4 FragColor;

in vec3 fragPos;
in vec3 normal;

uniform vec3 viewPos;
uniform vec3 lightDir;

uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform vec3 ambientColor;
uniform float shininess;

void main()
{;
	FragColor = vec4(diffuseColor, 1.0);
}
