#version 430
out vec4 FragColor;

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

uniform sampler2D diffuseMapTex;
uniform sampler2D specularMapTex;
uniform vec3 viewPos;
uniform float shininess;

struct PointLight {
	vec3 pos;

	float linear;
	float quad;
};
#define NUM_POINT_LIGHTS 4

uniform PointLight pointLights[NUM_POINT_LIGHTS];

vec3 calcPointLight(PointLight pointLight, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 norm = normalize(normal);
	vec3 diffuseMap = vec3(texture(diffuseMapTex, texCoord));

	float ambientScalar = 0.2;
	vec3 colorOut = ambientScalar*diffuseMap;

	for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
		colorOut += calcPointLight(pointLights[i], norm, fragPos, viewDir);
	}

	FragColor = vec4(colorOut, 1.0);
}

vec3 calcPointLight(PointLight pointLight, vec3 norm, vec3 fragPos, vec3 viewDir)
{
	vec3 diffuseMap = vec3(texture(diffuseMapTex, texCoord));
	vec3 specularMap = vec3(texture(specularMapTex, texCoord));
	vec3 lightDir = normalize(pointLight.pos - fragPos);

	float distance = length(pointLight.pos - fragPos);
	float diffuseScalar = 0.5*max(dot(norm, lightDir), 0.0);
	float specularScalar = 0.0;
	if (dot(normal, lightDir) > 0.0) {
		vec3 reflectDir = reflect(-lightDir, norm);
		specularScalar = 1.0*pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	}
	float attenuation = 3.0/(1.0 + pointLight.linear*distance + pointLight.quad*(distance*distance));
	vec3 diffuse = diffuseScalar*diffuseMap;
	vec3 specular = specularScalar*specularMap;

	return attenuation*(diffuse + specular);
}
