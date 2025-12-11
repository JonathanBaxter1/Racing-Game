#version 430
layout (location = 0) in vec2 aPos;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	vec4 worldPos = vec4(aPos.x*100.0, 500.0, aPos.y*100.0, 1.0);
	gl_Position = projection*view*worldPos;
//	gl_Position = vec4(1.0-aPos.x, aPos.y, 0.0, 1.0);
}
