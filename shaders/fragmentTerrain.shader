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
float perlin(float x, float y, float cellSize);

void main()
{
	float x = fragPos.x;
//	float y = fragPos.y;
	float z = fragPos.z;

	float y = 0.0;
	float cellSize = 8192.0;
	float multiplier = 1.0;
	float perlinDx = 0.0;
	float perlinDy = 0.0;
	for (int i = 0; i < 4; i++) {
		float perlinResult = perlin(x + 1000000.0, z + 1000000.0, cellSize);
		float perlinResultX = perlin(x + 1.0 + 1000000.0, z + 1000000.0, cellSize);
		float perlinResultY = perlin(x + 1000000.0, z + 1.0 + 1000000.0, cellSize);
		y += perlinResult*multiplier;
		perlinDx += (perlinResultX - perlinResult)*cellSize;
		perlinDy += (perlinResultY - perlinResult)*cellSize;
		cellSize *= 0.5;
		multiplier *= 0.5;
	}
	float grassY = y;
	float grassPerlinDx = perlinDx;
	float grassPerlinDy = perlinDy;
	for (int i = 0; i < 8; i++) {
		float perlinResult = perlin(x + 1000000.0, z + 1000000.0, cellSize);
		float perlinResultX = perlin(x + 1.0 + 1000000.0, z + 1000000.0, cellSize);
		float perlinResultY = perlin(x + 1000000.0, z + 1.0 + 1000000.0, cellSize);
		y += perlinResult*multiplier;
		perlinDx += (perlinResultX - perlinResult)*cellSize;
		perlinDy += (perlinResultY - perlinResult)*cellSize;
		cellSize *= 0.5;
		multiplier *= 0.5;
	}
	grassY = (grassY + 0.8)/1.6;
	float newY = (y + 0.8)/1.6;
	float isMountain = step(0.8, newY);
	float isMountainFade = clamp((newY - 0.75)*10.0, 0.0, 1.0);
	y = mix(grassY, newY, isMountainFade);
	perlinDx = mix(grassPerlinDx, perlinDx, isMountainFade);
	perlinDy = mix(grassPerlinDy, perlinDy, isMountainFade);
	perlinDx /= 1.6;
	perlinDy /= 1.6;
	float scaleFlat = 0.1;
	float scaleMountain = 2*y-3.1+1.28/y;
	y *= mix(scaleFlat, scaleMountain, isMountain)*8192.0;
	perlinDx *= mix(scaleFlat, scaleMountain, isMountain);
	perlinDy *= mix(scaleFlat, scaleMountain, isMountain);
	vec3 norm = normalize(vec3(-perlinDx, 1.0, -perlinDy));
	float flatness = pow(norm.y,2);

	float stoneSnowMix = clamp((y - 1300.0 + flatness*300.0)/5.0, 0.0, 1.0);
	float grassStoneMix = 1.0 - clamp((y - 650.0)/50.0, 0.0, 1.0);
	vec2 texCoord = vec2(fragPos.x, fragPos.z);
	vec4 snowColor = texture(snowDiffuseTex, texCoord/8.0);
	vec4 stoneColor = texture(stoneDiffuseTex, texCoord/256.0);
	vec4 grassColor = texture(grassDiffuseTex, texCoord/8.0);
	vec3 color = vec3(mix(stoneColor, snowColor, stoneSnowMix));
	color = mix(color, vec3(grassColor), grassStoneMix);

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 lightDir = normalize(vec3(1.0, .5, 1.0));

	float diffuseScalar = 1.0*max(dot(norm, lightDir), 0.0);

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

float perlin(float x, float y, float cellSize)
{
	float x2 = x/cellSize;
	float y2 = y/cellSize;
	int x0 = int(x2);
	int y0 = int(y2);
	int x1 = x0 + 1;
	int y1 = y0 + 1;
	float u = fade(x2 - float(x0));
	float v = fade(y2 - float(y0));

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

	return n;
}

