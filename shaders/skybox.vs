#version 430

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float aTexIndex;

out vec2 texCoord;
out float texIndex;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	mat4 viewStationary = view;
	viewStationary[3][0] = 0.0;
	viewStationary[3][1] = 0.0;
	viewStationary[3][2] = 0.0;
	gl_Position = projection*viewStationary*vec4(aPos, 1.0);

	texCoord = aTexCoord;
	texIndex = aTexIndex;
}
