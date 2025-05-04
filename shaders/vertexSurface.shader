#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 model;

out vec3 fragPos;
out vec3 normal;
out vec2 texCoord;

uniform mat4 view;
uniform mat4 projection;
uniform uint size;

void main()
{
	float x = 10.0 + aPos.x + floor(gl_InstanceID/float(size));
	float z = 10.0 + aPos.z + mod(gl_InstanceID, float(size));
	float y = 7.0*(sin(x/20.0)*sin(z/10.0));
//	float angle = 6.283185*fract(sin(dot(vec2(x, z)/float(size), vec2(12.9898,78.233)))*43758.5453123);
//	float y = aPos.y + angle;
	vec4 worldPos = vec4(x, y, z, 1.0);
//	gl_Position = projection*view*model*vec4(aPos, 1.0);
	gl_Position = projection*view*worldPos;
	fragPos = vec3(worldPos);

	// need to put this on the CPU
//	normal = mat3(transpose(inverse(model)))*aNormal;
	normal = aNormal;
	texCoord = aTexCoord;
}
