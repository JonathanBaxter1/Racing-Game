#version 430
layout (location = 0) in vec2 aPos;

void main()
{
	float waterLevel = 50.0;
	vec4 worldPos = vec4(aPos.x - 50000.0 + 2048.0, waterLevel, aPos.y - 50000.0 + 2048.0, 1.0);
	gl_Position = worldPos;
}
