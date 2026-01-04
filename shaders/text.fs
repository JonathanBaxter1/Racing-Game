#version 430

uniform sampler2D tex;

in vec2 texCoord;

out vec4 FragColor;

void main()
{
	float alpha = texture(tex, texCoord).r;
	FragColor = vec4(0.0, 0.0, 0.0, alpha);
}
