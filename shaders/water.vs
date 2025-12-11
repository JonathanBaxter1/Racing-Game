#version 430
layout (location = 0) in vec2 aPos;

void main()
{
	float waterLevel = 50.0;
	vec4 worldPos = vec4(aPos.x, waterLevel, aPos.y, 1.0);
	gl_Position = worldPos;
}
