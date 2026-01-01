#version 430

uniform vec3 diffuseColor;

out vec4 FragColor;

void main()
{
	FragColor = vec4(diffuseColor.rgb, 1.0);
}
