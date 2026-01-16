#version 430

in vec3 fragPos;
in vec3 normal;

out vec4 FragColor;

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

	float specularScalar = 0.0;
	if (dot(norm, lightDir) > 0.0) {
		vec3 reflectDir = reflect(-lightDir, norm);
		specularScalar = 1.0*pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	}

	vec3 colorOut = ambientColor + diffuseScalar*diffuseColor + specularScalar*specularColor;
	FragColor = vec4(colorOut, 1.0);
}
