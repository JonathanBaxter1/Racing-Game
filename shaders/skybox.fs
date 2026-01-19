#version 430

in vec2 texCoord;
in float texIndex;

out vec4 FragColor;

uniform sampler2D textures[6];

void main()
{
	vec3 color;
	if (texIndex == 0.0) { color = vec3(texture(textures[0], texCoord));
	} else if (texIndex == 1.0) { color = vec3(texture(textures[1], texCoord));
	} else if (texIndex == 2.0) { color = vec3(texture(textures[2], texCoord));
	} else if (texIndex == 3.0) { color = vec3(texture(textures[3], texCoord));
	} else if (texIndex == 4.0) { color = vec3(texture(textures[4], texCoord));
	} else if (texIndex == 5.0) { color = vec3(texture(textures[5], texCoord));
	}
	FragColor = vec4(color, 1.0);
}
