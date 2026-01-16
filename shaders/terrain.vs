#version 430

layout (location = 0) in vec2 aPos;
layout (location = 1) in float aResolutionDivisor2;

flat out float resolutionDivisor2;

void main()
{
	resolutionDivisor2 = aResolutionDivisor2;
	vec4 worldPos = vec4(aPos.x, 0.0, aPos.y, 1.0);
	gl_Position = worldPos;
}
