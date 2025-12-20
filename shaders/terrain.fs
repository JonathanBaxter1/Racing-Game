#version 430

#define stoneTex textures[2]

uniform sampler2D textures[8];
uniform vec3 lightDir;

in vec3 coord;
in vec3 normal;

out vec4 FragColor;

void main()
{
//	vec2 texCoord;
	vec3 norm = normalize(normal);
	float nx = abs(norm.x);
	float ny = abs(norm.y);
	float nz = abs(norm.z);
	vec2 texCoordX = coord.yz/30.0;
	vec2 texCoordY = coord.xz/30.0;
	vec2 texCoordZ = coord.xy/30.0;

	vec3 texAxis;
	texAxis.x = clamp((nx - ny*0.9)*5.0, 0.0, 1.0);
	texAxis.x *= clamp((nx - nz*0.9)*5.0, 0.0, 1.0);
	texAxis.y = clamp((ny - nx*0.9)*5.0, 0.0, 1.0);
	texAxis.y *= clamp((ny - nz*0.9)*5.0, 0.0, 1.0);
	texAxis.z = clamp((nz - nx*0.9)*5.0, 0.0, 1.0);
	texAxis.z *= clamp((nz - ny*0.9)*5.0, 0.0, 1.0);

	texAxis = texAxis/(texAxis.x + texAxis.y + texAxis.z);
	vec3 texColorX = vec3(texture(stoneTex, texCoordX));
	vec3 texColorY = vec3(texture(stoneTex, texCoordY));
	vec3 texColorZ = vec3(texture(stoneTex, texCoordZ));
	vec3 texColor = texColorX*texAxis.x + texColorY*texAxis.y + texColorZ*texAxis.z;

	float diffuseScalar = 1.0*max(dot(normal, lightDir), 0.0);
	float ambientScalar = 0.4;

	FragColor = vec4(ambientScalar*texColor + diffuseScalar*texColor, 1.0);
}
