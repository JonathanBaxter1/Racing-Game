#version 430

#define stoneTex textures[2]

uniform sampler2D textures[8];

in vec3 coord;
in vec3 normal;

out vec4 FragColor;

void main()
{
	vec2 texCoord;
	float nx = abs(normal.x);
	float ny = abs(normal.y);
	float nz = abs(normal.z);
	if (ny > nx && ny > nz) {
		texCoord = coord.xz/30.0;
	} else if (nx > nz) {
		texCoord = coord.yz/30.0;
	} else {
		texCoord = coord.xy/30.0;
	}
	vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
	float diffuseScalar = 1.0*max(dot(normal, lightDir), 0.0);
	float ambientScalar = 0.4;
	vec3 texColor = vec3(texture(stoneTex, texCoord));
	FragColor = vec4(ambientScalar*texColor + diffuseScalar*texColor, 1.0);
}
