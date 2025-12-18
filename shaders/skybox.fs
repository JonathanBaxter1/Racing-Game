#version 430

in vec2 texCoord;
in float texIndex;

out vec4 FragColor;

uniform sampler2D textures[6];

void main()
{
	vec3 color = vec3(texture(textures[int(texIndex)], texCoord));
	FragColor = vec4(color, 1.0);
}
