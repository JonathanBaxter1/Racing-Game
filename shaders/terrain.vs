#version 430
layout (location = 0) in vec2 aPos;

void main()
{
	vec4 worldPos = vec4(aPos.x, 0.0, aPos.y, 1.0);
	gl_Position = worldPos;
}
