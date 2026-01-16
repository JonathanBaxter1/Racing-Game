#version 430

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 model;

out vec3 fragPos;
out vec3 normal;
out vec2 texCoord;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 worldPos = model*vec4(aPos, 1.0);
	float waterHeight = 50.0;
	gl_ClipDistance[0] = worldPos.y - waterHeight;

	gl_Position = projection*view*worldPos;
	fragPos = worldPos.xyz;

	// need to put this on the CPU?
	normal = mat3(transpose(inverse(model)))*aNormal;
	texCoord = aTexCoord;
}
