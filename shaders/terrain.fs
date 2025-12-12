#version 430

#define stoneTex textures[2]

uniform sampler2D textures[8];

in vec2 texCoord;
in vec3 normal;

out vec4 FragColor;

void main()
{
	vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
	float diffuseScalar = 1.0*max(dot(normal, lightDir), 0.0);
	float ambientScalar = 0.1;
	vec3 texColor = vec3(texture(stoneTex, texCoord));
	FragColor = vec4(ambientScalar*texColor + diffuseScalar*texColor, 1.0);
}
