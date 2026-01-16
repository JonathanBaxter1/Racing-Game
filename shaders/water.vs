#version 430

layout (location = 0) in vec2 aPos;

out vec4 clipSpace;
out vec3 fragPos;

uniform float waterHeight;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	vec4 worldPos = vec4(aPos.x - 50000.0, waterHeight, aPos.y - 50000.0, 1.0);
	fragPos = worldPos.xyz;
	clipSpace = projection*view*worldPos;
	gl_Position = clipSpace;
}
