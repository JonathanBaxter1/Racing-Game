#version 430

#define reflectionTex textures[0]

uniform sampler2D textures[8];

in vec4 clipSpace;
in vec3 normal;

out vec4 FragColor;

void main()
{
	vec2 texCoord;
	texCoord.x = (clipSpace.x/clipSpace.w)/2.0 + 0.5;
	texCoord.y = 1.0 - ((clipSpace.y/clipSpace.w)/2.0 + 0.5);
	vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
	float diffuseScalar = 1.0*max(dot(normal, lightDir), 0.0);
	float ambientScalar = 0.3;
	vec3 waterColor = vec3(0.06, 0.5, 0.6);
	vec3 reflectionColor = vec3(texture(reflectionTex, texCoord));
	vec3 finalColor = mix(reflectionColor, waterColor, 0.2);
	FragColor = vec4(finalColor, 1.0);
//	FragColor = vec4(ambientScalar*waterColor + diffuseScalar*waterColor, 1.0);
}
