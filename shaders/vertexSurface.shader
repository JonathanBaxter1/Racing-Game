#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 model;

out vec3 fragPos;
out vec3 normal;

uniform mat4 view;
uniform mat4 projection;
uniform uint size;

vec2 gradient(int x, int y);
float fade(float t);
float fadePrime(float t);
vec3 perlin(float x, float y, float cellSize);

void main()
{
	float x = aPos.x + floor(gl_InstanceID/float(size));
	float z = aPos.z + mod(gl_InstanceID, float(size));

	float y = 0.0;
	float cellSize = 1024.0;
	float perlinDx = 0.0;
	float perlinDy = 0.0;
	for (int i = 0; i < 4; i++) {
		vec3 perlinResult = perlin(x, z, cellSize);
		y += perlinResult.x*cellSize;
		perlinDx += perlinResult.y;
		perlinDy += perlinResult.z;
		cellSize *= 0.5;
	}

	vec4 worldPos = vec4(x, y, z, 1.0);
//	gl_Position = projection*view*model*vec4(aPos, 1.0);
	gl_Position = projection*view*worldPos;
	fragPos = vec3(worldPos);

	normal = normalize(vec3(-perlinDx, -perlinDy, 1.0));
//	texCoord = aTexCoord;
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
