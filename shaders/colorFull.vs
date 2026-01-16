#version 430

layout (location = 0) in vec3 aPos;

uniform float waterHeight;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 worldPos = vec4(aPos, 1.0);
	gl_ClipDistance[0] = worldPos.y - waterHeight;
	gl_Position = projection*view*worldPos;
}
