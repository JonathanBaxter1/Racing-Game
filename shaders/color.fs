#version 430
out vec4 FragColor;

in vec3 fragPos;
in vec3 normal;
//in vec2 texCoord;

uniform vec3 viewPos;
uniform vec3 lightDir;

uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform vec3 ambientColor;
uniform float shininess;

void main()
{
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 norm = normalize(normal);
	float diffuseScalar = 1.0*max(dot(norm, lightDir), 0.0);
//	float ambientScalar = 0.05;

	float specularScalar = 0.0;
	if (dot(norm, lightDir) > 0.0) {
		vec3 reflectDir = reflect(-lightDir, norm);
		specularScalar = 1.0*pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	}

	vec3 colorOut = ambientColor + diffuseScalar*diffuseColor + specularScalar*specularColor;
	FragColor = vec4(colorOut, 1.0);
}
