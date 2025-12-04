#version 330 core
out vec4 FragColor;

in vec3 fragPos;

uniform sampler2D snowDiffuseTex;
uniform sampler2D stoneDiffuseTex;
uniform sampler2D grassDiffuseTex;
uniform sampler2D sandDiffuseTex;
uniform vec3 viewPos;

vec2 gradient(int x, int y);
float fade(float t);
float fadePrime(float t);
vec3 perlin(float x, float y, float cellSize);

void main()
{
	float x = fragPos.x;
//	float y = fragPos.y;
	float z = fragPos.z;
//	vec3 norm = normalize(normal);

	float y = 0.0;
	float cellSize = 1024.0;
	float perlinDx = 0.0;
	float perlinDy = 0.0;
	float flatness = 0.0;
	for (int i = 0; i < 8; i++) {
		vec3 perlinResult = perlin(x + 1000000.0, z + 1000000.0, cellSize);
		y += perlinResult.x*cellSize*0.5;
		perlinDx += perlinResult.y;
		perlinDy += perlinResult.z;
		flatness += normalize(vec3(-perlinResult.y, 1.0, -perlinResult.z)).y;
		cellSize *= 0.5;
	}

	vec3 norm = normalize(vec3(-perlinDx, 1.0, -perlinDy));

	float snowStoneMix = clamp((y - 200.0 + flatness*30.0)/30.0, 0.0, 1.0);
	vec2 texCoord = vec2(fragPos.x, fragPos.z)/64.0;
	vec4 snowColor = texture(snowDiffuseTex, texCoord);
	vec4 stoneColor = texture(stoneDiffuseTex, texCoord);
	vec3 color = vec3(mix(stoneColor, snowColor, snowStoneMix));

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));

	float diffuseScalar = 0.7*max(dot(norm, lightDir), 0.0);

	float ambientScalar = 0.3;

	float distance = distance(viewPos, fragPos);
	float fogLevel = clamp((distance-12288.0)/4096.0, 0.0, 1.0);
	vec3 surfaceColor = ambientScalar*color + diffuseScalar*color;
	vec3 fogColor = vec3(0.5, 0.75, 0.95);
	vec3 colorOut = mix(surfaceColor, fogColor, fogLevel);

	FragColor = vec4(colorOut, 1.0);
}

vec2 gradient(int x, int y, int seed)
{
	uint w = 32u;
	uint s = 16u;
	uint a = uint(x);
	uint b = uint(y);
	a *= 3284157443u;
	b ^= a << s | a >> w - s;
	b *= 1911520717u;
	a ^= b << s | b >> w - s;
	a *= 2048419325u;
	float random = a * (3.14159265 / ~(~0u >> 1));

	return vec2(sin(random), cos(random));
}

float fade(float t)
{
	return t*t*t*(t*(t*6 - 15) + 10);
}

float fadePrime(float t)
{
	return t*t*(t*(t*30 - 60) + 30);
}

vec3 perlin(float x, float y, float cellSize)
{
	float x2 = x/cellSize;
	float y2 = y/cellSize;
	int x0 = int(x2);
	int y0 = int(y2);
	int x1 = x0 + 1;
	int y1 = y0 + 1;
	float u = fade(x2 - float(x0));
	float v = fade(y2 - float(y0));
	float du = fadePrime(x2 - float(x0));
	float dv = fadePrime(y2 - float(y0));

	vec2 g00 = gradient(x0, y0, 42);
	vec2 g01 = gradient(x0, y1, 42);
	vec2 g10 = gradient(x1, y0, 42);
	vec2 g11 = gradient(x1, y1, 42);

	float n00 = dot(g00, vec2(x2 - float(x0), y2 - float(y0)));
	float n01 = dot(g01, vec2(x2 - float(x0), y2 - float(y1)));
	float n10 = dot(g10, vec2(x2 - float(x1), y2 - float(y0)));
	float n11 = dot(g11, vec2(x2 - float(x1), y2 - float(y1)));

	float nx0 = mix(n00, n10, u);
	float nx1 = mix(n01, n11, u);
	float n = mix(nx0, nx1, v);

	float dn00_dx = g00.x;
	float dn01_dx = g01.x;
	float dn10_dx = g10.x;
	float dn11_dx = g11.x;

	float dnx0_dx = mix(dn00_dx, dn10_dx, u) + du * (n10 - n00);
	float dnx1_dx = mix(dn01_dx, dn11_dx, u) + du * (n11 - n01);

	float dn_dx = mix(dnx0_dx, dnx1_dx, v) + dv * (nx1 - nx0);

	float dn00_dy = g00.y;
	float dn01_dy = g01.y;
	float dn10_dy = g10.y;
	float dn11_dy = g11.y;

	float dnx0_dy = mix(dn00_dy, dn10_dy, u) + du * (n10 - n00);
	float dnx1_dy = mix(dn01_dy, dn11_dy, u) + du * (n11 - n01);

	float dn_dy = mix(dnx0_dy, dnx1_dy, v) + dv * (nx1 - nx0);

	return vec3(n, dn_dx, dn_dy);
}
