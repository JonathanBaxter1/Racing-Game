#version 430

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float aTexIndex;

out vec2 texCoord;
out float texIndex;

uniform float waterHeight;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_ClipDistance[0] = aPos.y - waterHeight + 100.0;
	gl_Position = projection*view*vec4(aPos, 1.0);
	texCoord = aTexCoord;
	texIndex = aTexIndex;
}
