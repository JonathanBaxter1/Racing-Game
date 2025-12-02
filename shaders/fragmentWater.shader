#version 330 core
out vec4 FragColor;

in vec3 fragPos;
in vec3 normal;

uniform sampler2D diffuseMapTex;
uniform sampler2D specularMapTex;
uniform vec3 viewPos;

void main()
{
	float x = fragPos.x;
	float y = fragPos.y;
	float z = fragPos.z;

	vec3 color = vec3(0.06, 0.5, 0.6);

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
	vec3 norm = normalize(normal);

	float diffuseScalar = 0.7*max(dot(norm, lightDir), 0.0);

	float ambientScalar = 0.3;
	vec3 colorOut = ambientScalar*color + diffuseScalar*color;

	FragColor = vec4(colorOut, 1.0);
}
