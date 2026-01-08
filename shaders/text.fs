#version 430

uniform sampler2D tex;
uniform vec3 color;

in vec2 texCoord;

out vec4 FragColor;

void main()
{
	float alpha = texture(tex, texCoord).r;
	FragColor = vec4(color, alpha);
}
