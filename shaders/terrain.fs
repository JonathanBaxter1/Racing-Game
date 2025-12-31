#version 430

#define stoneTex textures[3]
#define grassTex textures[4]
#define snowTex textures[5]

uniform sampler2D textures[8];
uniform vec3 lightDir;

in vec3 coord;
in vec3 normal;
in vec3 color;

out vec4 FragColor;

void main()
{
	vec3 norm = normal;
	float nx = abs(norm.x);
	float ny = abs(norm.y);
	float nz = abs(norm.z);
	vec2 texCoordX = coord.yz;
	vec2 texCoordY = coord.xz;
	vec2 texCoordZ = coord.xy;

	vec3 texAxis;
	texAxis.x = clamp((nx - ny*0.9)*5.0, 0.0, 1.0);
	texAxis.x *= clamp((nx - nz*0.9)*5.0, 0.0, 1.0);
	texAxis.y = clamp((ny - nx*0.9)*5.0, 0.0, 1.0);
	texAxis.y *= clamp((ny - nz*0.9)*5.0, 0.0, 1.0);
	texAxis.z = clamp((nz - nx*0.9)*5.0, 0.0, 1.0);
	texAxis.z *= clamp((nz - ny*0.9)*5.0, 0.0, 1.0);

	texAxis = texAxis/(texAxis.x + texAxis.y + texAxis.z);
	vec3 texColorX = vec3(0.0, 0.0, 0.0);
	vec3 texColorY = vec3(0.0, 0.0, 0.0);
	vec3 texColorZ = vec3(0.0, 0.0, 0.0);
	if (color == vec3(1.0, 0.0, 0.0)) {
		if (texAxis.x > 0.0) texColorX = vec3(texture(snowTex, texCoordX));
		if (texAxis.y > 0.0) texColorY = vec3(texture(snowTex, texCoordY));
		if (texAxis.z > 0.0) texColorZ = vec3(texture(snowTex, texCoordZ));
	} else if (color == vec3(0.0, 1.0, 0.0)) {
		if (texAxis.x > 0.0) texColorX = vec3(texture(grassTex, texCoordX));
		if (texAxis.y > 0.0) texColorY = vec3(texture(grassTex, texCoordY));
		if (texAxis.z > 0.0) texColorZ = vec3(texture(grassTex, texCoordZ));
	} else if (color == vec3(0.0, 0.0, 1.0)) {
		if (texAxis.x > 0.0) texColorX = vec3(texture(stoneTex, texCoordX));
		if (texAxis.y > 0.0) texColorY = vec3(texture(stoneTex, texCoordY));
		if (texAxis.z > 0.0) texColorZ = vec3(texture(stoneTex, texCoordZ));
	} else {
		if (texAxis.x > 0.0) texColorX = vec3(texture(snowTex, texCoordX)*color.r + texture(grassTex, texCoordX)*color.g+ texture(stoneTex, texCoordX)*color.b);
		if (texAxis.y > 0.0) texColorY = vec3(texture(snowTex, texCoordY)*color.r + texture(grassTex, texCoordY)*color.g+ texture(stoneTex, texCoordY)*color.b);
		if (texAxis.z > 0.0) texColorZ = vec3(texture(snowTex, texCoordZ)*color.r + texture(grassTex, texCoordZ)*color.g+ texture(stoneTex, texCoordZ)*color.b);
	}
	vec3 texColor = texColorX*texAxis.x + texColorY*texAxis.y + texColorZ*texAxis.z;

	float diffuseScalar = 1.0*max(dot(normal, lightDir), 0.0);
	float ambientScalar = 0.4;

	FragColor = vec4(ambientScalar*texColor + diffuseScalar*texColor, 1.0);
}

