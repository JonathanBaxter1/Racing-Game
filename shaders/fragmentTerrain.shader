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

	float isSnow = step(500.0, y);
	float isGrass = 1.0-step(100.0, y);
	vec3 color = vec3(0.4+0.6*isSnow, 0.4+0.6*isSnow+0.6*isGrass, 0.4+0.6*isSnow);

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
	vec3 norm = normalize(normal);

	float diffuseScalar = 0.7*max(dot(norm, lightDir), 0.0);
//	vec3 diffuseMap = vec3(texture(diffuseMapTex, texCoord));

	float ambientScalar = 0.3;

	float distance = distance(viewPos, fragPos);
	float fogLevel = clamp((distance-12288.0)/4096.0, 0.0, 1.0);
	vec3 surfaceColor = ambientScalar*color + diffuseScalar*color;
	vec3 fogColor = vec3(0.5, 0.75, 0.95);
	vec3 colorOut = mix(surfaceColor, fogColor, fogLevel);

	FragColor = vec4(colorOut, 1.0);
}
