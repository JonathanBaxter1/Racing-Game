#version 430
out vec4 FragColor;

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

uniform sampler2D diffuseMapTex;
uniform sampler2D specularMapTex;
uniform vec3 viewPos;
uniform float shininess;

void main()
{
	vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 norm = normalize(normal);
	vec3 diffuseMap = vec3(texture(diffuseMapTex, texCoord));
	vec3 specularColor = vec3(1.0, 1.0, 1.0);
	float diffuseScalar = 1.0*max(dot(norm, lightDir), 0.0);
	float ambientScalar = 0.3;

	float specularScalar = 0.0;
	if (dot(norm, lightDir) > 0.0) {
		vec3 reflectDir = reflect(-lightDir, norm);
		specularScalar = 1.0*pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	}
	vec3 colorOut = ambientScalar*diffuseMap + diffuseScalar*diffuseMap + specularScalar*specularColor;

	FragColor = vec4(colorOut, 1.0);
}
