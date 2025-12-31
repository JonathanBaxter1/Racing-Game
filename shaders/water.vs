#version 430
layout (location = 0) in vec2 aPos;

uniform mat4 projection;
uniform mat4 view;

out vec4 clipSpace;
out vec3 fragPos;

void main()
{
	float waterLevel = 50.0;
	vec4 worldPos = vec4(aPos.x - 50000.0, waterLevel, aPos.y - 50000.0, 1.0);
	fragPos = worldPos.xyz;
	clipSpace = projection*view*worldPos;
	gl_Position = clipSpace;
}
