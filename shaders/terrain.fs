#version 430

#define stoneLayer 0
#define grassLayer 1
#define snowLayer 2

uniform sampler2DArray maps;
uniform sampler2DArray textures;
uniform vec3 lightDir;
uniform vec3 viewPos;
uniform float graphicsSetting;

in vec3 coord;
in vec3 normal;
in vec3 color;

out vec4 FragColor;

void main()
{
	vec3 norm = normalize(normal);
	float distance = distance(viewPos, coord*30.0);

	vec2 texCoordY = coord.xz;
	vec3 texColor;
	vec3 texColorStone;
	vec3 texColorGrass;
	vec3 texColorSnow;
	if (graphicsSetting == 0.0) {
		texColorGrass = vec3(0.0, 0.0, 0.0);
		texColorSnow = vec3(0.0, 0.0, 0.0);
	} else {
		texColorGrass = vec3(texture(textures, vec3(texCoordY, grassLayer)));
		texColorSnow = vec3(texture(textures, vec3(texCoordY, snowLayer)));
	}
	if ((norm.y > 0.85 || distance > 1500.0 || graphicsSetting == 0.0) && graphicsSetting != 3.0) {
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

	if (graphicsSetting == 0.0) {
		texColor = texColorStone;
	} else {
		texColor = texColorStone*color.b + texColorGrass*color.g + texColorSnow*color.r;
	}
	float diffuseScalar = 1.0*max(dot(normal, lightDir), 0.0);
	float ambientScalar = 0.4;

	FragColor = vec4(ambientScalar*texColor + diffuseScalar*texColor, 1.0);
}

