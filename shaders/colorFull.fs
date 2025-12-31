#version 430
out vec4 FragColor;

uniform vec3 diffuseColor;

void main()
{
	FragColor = vec4(diffuseColor, 1.0);
}
