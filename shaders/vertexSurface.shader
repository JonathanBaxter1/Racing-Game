#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 model;

out vec3 fragPos;
out vec3 normal;
out vec2 surfaceCoord;
out vec2 g00;
out vec2 g01;
out vec2 g10;
out vec2 g11;

uniform mat4 view;
uniform mat4 projection;
uniform uint size;

const vec2 gradients[8] = vec2[8](
	vec2(1, 0),
	vec2(-1, 0),
	vec2(0, 1),
	vec2(0, -1),
	vec2(0.707, 0.707),
	vec2(0.707, -0.707),
	vec2(-0.707, 0.707),
	vec2(-0.707, -0.707)
);


int hash(int x);
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
	g00 = gradient(x0, z0, 42);
	g01 = gradient(x0, z1, 42);
	g10 = gradient(x1, z0, 42);
	g11 = gradient(x1, z1, 42);

	float n00 = dot(g00, vec2(x2 - float(x0), z2 - float(z0)));
	float n01 = dot(g01, vec2(x2 - float(x0), z2 - float(z1)));
	float n10 = dot(g10, vec2(x2 - float(x1), z2 - float(z0)));
	float n11 = dot(g11, vec2(x2 - float(x1), z2 - float(z1)));

	float interp1 = mix(n00, n10, (x2 - float(x0)));
	float interp2 = mix(n01, n11, (x2 - float(x0)));
	float perlinResult = mix(interp1, interp2, z2 - float(z0));

	surfaceCoord = vec2(x, z);
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

int hash(int x)
{
	x ^= x >> 16;
	x *= 0x7feb352d;
	x ^= x >> 15;
	x *= 0x846ca68b;
	x ^= x >> 16;
	return x;
}

vec2 gradient(int x, int y, int seed)
{
	seed ^= hash(x);
	seed ^= hash(y*0x9e3779b9);
	return gradients[seed&7];
}
