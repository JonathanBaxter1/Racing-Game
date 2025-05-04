#version 330 core
out vec4 FragColor;

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

uniform sampler2D diffuseMapTex;
uniform sampler2D specularMapTex;
uniform vec3 viewPos;

void main()
{
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 norm = normalize(normal);
	vec3 diffuseMap = vec3(texture(diffuseMapTex, texCoord));

	float ambientScalar = 1.0;
	vec3 colorOut = ambientScalar*diffuseMap;

	FragColor = vec4(colorOut, 1.0);
}
