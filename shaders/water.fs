#version 430

#define stoneTex textures[2]

uniform sampler2D textures[8];

in vec3 normal;

out vec4 FragColor;

void main()
{
	vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
	float diffuseScalar = 1.0*max(dot(normal, lightDir), 0.0);
	float ambientScalar = 0.3;
	vec3 waterColor = vec3(0.06, 0.5, 0.6);
	FragColor = vec4(ambientScalar*waterColor + diffuseScalar*waterColor, 1.0);
}
