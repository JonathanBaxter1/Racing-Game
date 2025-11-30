#version 330 core
out vec4 FragColor;

in vec3 fragPos;
in vec3 normal;
in vec2 surfaceCoord;
in vec2 g00;
in vec2 g01;
in vec2 g10;
in vec2 g11;

uniform sampler2D diffuseMapTex;
uniform sampler2D specularMapTex;
uniform vec3 viewPos;

void main()
{
	float x = surfaceCoord.x;
	float z = surfaceCoord.y;
	float cellSize = 64.0;
	float x2 = x/cellSize;
	float z2 = z/cellSize;
	int x0 = int(x2);
	int z0 = int(z2);
	int x1 = x0 + 1;
	int z1 = z0 + 1;

	float n00 = dot(g00, vec2(x2 - float(x0), z2 - float(z0)));
	float n01 = dot(g01, vec2(x2 - float(x0), z2 - float(z1)));
	float n10 = dot(g10, vec2(x2 - float(x1), z2 - float(z0)));
	float n11 = dot(g11, vec2(x2 - float(x1), z2 - float(z1)));

	float interp1 = mix(n00, n10, (x2 - float(x0)));
	float interp2 = mix(n01, n11, (x2 - float(x0)));
	float perlinResult = mix(interp1, interp2, z2 - float(z0));

	float y = perlinResult*30.0+5.0;
	float isSnow = step(8.0, y);
	float isGrass = 1.0-step(3.0, y);
	vec3 color = vec3(0.4+0.6*isSnow, 0.4+0.6*isSnow+0.6*isGrass, 0.4+0.6*isSnow);

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
	vec3 norm = normalize(normal);

	float diffuseScalar = 0.9*max(dot(norm, lightDir), 0.0);
//	vec3 diffuseMap = vec3(texture(diffuseMapTex, texCoord));

	float ambientScalar = 0.1;
	vec3 colorOut = ambientScalar*color + diffuseScalar*color;
//	vec3 colorOut = ambientScalar*diffuseMap + diffuseScalar*diffuseMap;

	FragColor = vec4(colorOut, 1.0);
}
