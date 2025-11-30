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

vec2 gradient(int x, int y, int seed);

void main()
{
	float x = aPos.x + floor(gl_InstanceID/float(size));
	float z = aPos.z + mod(gl_InstanceID, float(size));
	float cellSize = 64.0;
	float x2 = x/cellSize;
	float z2 = z/cellSize;
	int x0 = int(x2);
	int z0 = int(z2);
	int x1 = x0 + 1;
	int z1 = z0 + 1;
	vec2 g00 = gradient(x0, z0, 42);
	vec2 g01 = gradient(x0, z1, 42);
	vec2 g10 = gradient(x1, z0, 42);
	vec2 g11 = gradient(x1, z1, 42);

	float n00 = dot(g00, vec2(x2 - float(x0), z2 - float(z0)));
	float n01 = dot(g01, vec2(x2 - float(x0), z2 - float(z1)));
	float n10 = dot(g10, vec2(x2 - float(x1), z2 - float(z0)));
	float n11 = dot(g11, vec2(x2 - float(x1), z2 - float(z1)));

	float interp1 = mix(n00, n10, (x2 - float(x0)));
	float interp2 = mix(n01, n11, (x2 - float(x0)));
	float perlinResult = mix(interp1, interp2, z2 - float(z0));

	float y = perlinResult*30.0+5.0;
	vec4 worldPos = vec4(x, y, z, 1.0);
//	gl_Position = projection*view*model*vec4(aPos, 1.0);
	gl_Position = projection*view*worldPos;
	fragPos = vec3(worldPos);

	// need to put this on the CPU
//	normal = mat3(transpose(inverse(model)))*aNormal;
	float fx = -(2.0 * cos(x / 20.0) * (1.0 / 20.0) * sin(z / 10.0));
	float fz = -(2.0 * sin(x / 20.0) * cos(z / 10.0) * (1.0 / 10.0));

	normal = normalize(vec3(fx, 1.0, fz));
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
