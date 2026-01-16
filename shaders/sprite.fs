#version 430

in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D tex;
uniform vec3 color;

void main()
{
	vec4 colorMult = vec4(color, 1.0);
	FragColor = colorMult*texture(tex, texCoord);
}
