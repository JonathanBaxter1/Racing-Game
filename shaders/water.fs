#version 430

uniform sampler2D reflectionTex;
uniform sampler2D dudvTex;
uniform sampler2D heightMapTex;
uniform vec3 viewPos;
uniform vec3 lightDir;
uniform float time;

in vec4 clipSpace;
in vec3 fragPos;

out vec4 FragColor;

void main()
{
	vec3 cameraVec = viewPos - fragPos;
	float distance = length(cameraVec);
	vec2 mapCoord = fragPos.xz/4096.0;
	float waterHeight = 50.0;
	float depth = waterHeight - texture(heightMapTex, mapCoord).x*274.0;

	vec2 dudvCoord1 = vec2(fragPos.x + time*2.0, fragPos.z)/50.0;
	vec2 dudvCoord2 = vec2(fragPos.x, fragPos.z + time*2.0)/50.0;
	vec2 dudvOffset1 = (texture(dudvTex, dudvCoord1).rg - 0.5)*0.02;
	vec2 dudvOffset2 = (texture(dudvTex, dudvCoord2).rg - 0.5)*0.02;
	vec2 dudvOffset = dudvOffset1 + dudvOffset2;
	dudvOffset *= clamp((depth - 1.0)/(distance/10.0), 0.0, 1.0);
	vec2 texCoord;
	texCoord.x = (clipSpace.x/clipSpace.w)/2.0 + 0.5;
	texCoord.y = 1.0 - ((clipSpace.y/clipSpace.w)/2.0 + 0.5);
	texCoord += dudvOffset;
	texCoord = clamp(texCoord, 0.0001, 0.9999);

	vec3 normal = normalize(vec3(dudvOffset.x, 0.1, dudvOffset.y));
	vec3 viewDir = normalize(cameraVec);

	vec3 specularColor = vec3(1.0, 1.0, 1.0);
	float specularScalar = 0.0;
	if (dot(normal, lightDir) > 0.0) {
		vec3 reflectDir = reflect(-lightDir, normal);
		specularScalar = 1.0*pow(max(dot(viewDir, reflectDir), 0.0), 64.0);
	}
	float diffuseScalar = 1.0*max(dot(normal, lightDir), 0.0);
	float ambientScalar = 0.3;
	vec3 waterColor = vec3(0.06, 0.5, 0.6);
	vec3 reflectionColor = vec3(texture(reflectionTex, texCoord));
	vec3 finalColor = mix(reflectionColor, waterColor, 0.2);
	FragColor = vec4(ambientScalar*finalColor + diffuseScalar*finalColor + specularScalar*specularColor, 1.0);
}
