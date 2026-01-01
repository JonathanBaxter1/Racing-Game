#version 430

#define stoneLayer 0
#define grassLayer 1
#define snowLayer 2

uniform sampler2DArray maps;
uniform sampler2DArray textures;
uniform vec3 lightDir;
uniform vec3 viewPos;

in vec3 coord;
in vec3 normal;
in vec3 color;

out vec4 FragColor;

void main()
{
	vec3 norm = normalize(normal);
	float distance = distance(viewPos, coord*30.0);
//	vec3 texColorX = vec3(0.0, 0.0, 0.0);
//	vec3 texColorY = vec3(0.0, 0.0, 0.0);
//	vec3 texColorZ = vec3(0.0, 0.0, 0.0);
//	if (color == vec3(1.0, 0.0, 0.0)) {
//		if (texAxis.x > 0.0) texColorX = vec3(texture(snowTex, texCoordX));
//		if (texAxis.y > 0.0) texColorY = vec3(texture(snowTex, texCoordY));
//		if (texAxis.z > 0.0) texColorZ = vec3(texture(snowTex, texCoordZ));
//	} else if (color == vec3(0.0, 1.0, 0.0)) {
//		if (texAxis.x > 0.0) texColorX = vec3(texture(grassTex, texCoordX));
//		if (texAxis.y > 0.0) texColorY = vec3(texture(grassTex, texCoordY));
//		if (texAxis.z > 0.0) texColorZ = vec3(texture(grassTex, texCoordZ));
//	} else if (color == vec3(0.0, 0.0, 1.0)) {
//		if (texAxis.x > 0.0) texColorX = vec3(texture(stoneTex, texCoordX));
//		if (texAxis.y > 0.0) texColorY = vec3(texture(stoneTex, texCoordY));
//		if (texAxis.z > 0.0) texColorZ = vec3(texture(stoneTex, texCoordZ));
//	} else {
//		if (texAxis.x > 0.0) texColorX = vec3(texture(snowTex, texCoordX)*color.r + texture(grassTex, texCoordX)*color.g+ texture(stoneTex, texCoordX)*color.b);
//		if (texAxis.y > 0.0) texColorY = vec3(texture(snowTex, texCoordY)*color.r + texture(grassTex, texCoordY)*color.g+ texture(stoneTex, texCoordY)*color.b);
//		if (texAxis.z > 0.0) texColorZ = vec3(texture(snowTex, texCoordZ)*color.r + texture(grassTex, texCoordZ)*color.g+ texture(stoneTex, texCoordZ)*color.b);
//	}

	vec2 texCoordY = coord.xz;
	vec3 texColor;
	vec3 texColorStone;
	vec3 texColorGrass = vec3(texture(textures, vec3(texCoordY, grassLayer)));
	vec3 texColorSnow = vec3(texture(textures, vec3(texCoordY, snowLayer)));
	if (norm.y > 0.85 || distance > 1500.0) {
		texColorStone = vec3(texture(textures, vec3(texCoordY, stoneLayer)));
	} else {
		float nx = abs(norm.x);
		float ny = abs(norm.y);
		float nz = abs(norm.z);
		vec2 texCoordX = coord.yz;
		vec2 texCoordZ = coord.xy;

		vec3 texAxis;
		texAxis.x = clamp((nx - ny*0.9)*5.0, 0.0, 1.0);
		texAxis.x *= clamp((nx - nz*0.9)*5.0, 0.0, 1.0);
		texAxis.y = clamp((ny - nx*0.9)*5.0, 0.0, 1.0);
		texAxis.y *= clamp((ny - nz*0.9)*5.0, 0.0, 1.0);
		texAxis.z = clamp((nz - nx*0.9)*5.0, 0.0, 1.0);
		texAxis.z *= clamp((nz - ny*0.9)*5.0, 0.0, 1.0);

		texAxis = texAxis/(texAxis.x + texAxis.y + texAxis.z);
		vec3 texColorX = vec3(texture(textures, vec3(texCoordX, stoneLayer)));
		vec3 texColorY = vec3(texture(textures, vec3(texCoordY, stoneLayer)));
		vec3 texColorZ = vec3(texture(textures, vec3(texCoordZ, stoneLayer)));
		texColorStone = texColorX*texAxis.x + texColorY*texAxis.y + texColorZ*texAxis.z;	
	}

	texColor = texColorStone*color.b + texColorGrass*color.g + texColorSnow*color.r;
	float diffuseScalar = 1.0*max(dot(normal, lightDir), 0.0);
	float ambientScalar = 0.4;

	FragColor = vec4(ambientScalar*texColor + diffuseScalar*texColor, 1.0);
}

