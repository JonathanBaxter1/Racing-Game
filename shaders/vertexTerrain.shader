#version 330 core
layout (location = 0) in vec2 aPos;

out vec3 fragPos;

uniform mat4 view;
uniform mat4 projection;
uniform uint innerWidth;
uniform uint outerWidth;
uniform uint squareSize;
uniform float centerX;
uniform float centerY;

vec2 gradient(int x, int y);
float fade(float t);
float perlin(float x, float y, float cellSize);

void main()
{
	float centerXrounded = floor(centerX/256.0)*256.0;
	float centerYrounded = floor(centerY/256.0)*256.0;

	uint outerWidth2 = outerWidth/squareSize;
	uint innerWidth2 = innerWidth/squareSize;
	uint thickness = (outerWidth2 - innerWidth2)/2u;
	uint section1Offset = outerWidth2*thickness;
	uint section2Offset = section1Offset + innerWidth2*thickness;
	uint section3Offset = section2Offset + innerWidth2*thickness;

	float x = centerXrounded + aPos.x*float(squareSize);
	float z = centerYrounded + aPos.y*float(squareSize);
	int xOffset = -int(outerWidth2)/2;
	int zOffset = -int(outerWidth2)/2;

	if (step(section3Offset, uint(gl_InstanceID)) == 1.0) {
		uint newID = uint(gl_InstanceID) - uint(section3Offset);
		xOffset += int(newID%outerWidth2);
		zOffset += int(thickness + innerWidth2 + newID/outerWidth2);
	} else if (step(section2Offset, uint(gl_InstanceID)) == 1.0) {
		uint newID = uint(gl_InstanceID) - uint(section2Offset);
		xOffset += int(thickness + innerWidth2 + newID%thickness);
		zOffset += int(thickness + newID/thickness);
	} else if (step(section1Offset, uint(gl_InstanceID)) == 1.0) {
		uint newID = uint(gl_InstanceID) - uint(section1Offset);
		xOffset += int(newID%thickness);
		zOffset += int(thickness + newID/thickness);
	} else {
		xOffset += gl_InstanceID%int(outerWidth2);
		zOffset += gl_InstanceID/int(outerWidth2);
	}
	x += float(xOffset*int(squareSize));
	z += float(zOffset*int(squareSize));

	float y = 0.0;
	float cellSize = 8192.0;
	float multiplier = 1.0;
	for (int i = 0; i < 4; i++) {
		float perlinResult = perlin(x + 1000000.0, z + 1000000.0, cellSize);
		y += perlinResult*multiplier;
		cellSize *= 0.5;
		multiplier *= 0.5;
	}
	float grassY = y;
	for (int i = 0; i < 8; i++) {
		float perlinResult = perlin(x + 1000000.0, z + 1000000.0, cellSize);
		y += perlinResult*multiplier;
		cellSize *= 0.5;
		multiplier *= 0.5;
	}
	grassY = (grassY + 0.8)/1.6;
	float newY = (y + 0.8)/1.6;
	float isMountain = step(0.8, newY);
	float isMountainFade = clamp((newY - 0.75)*10.0, 0.0, 1.0);
	y = mix(grassY, newY, isMountainFade);
	float yFlat = 0.1*y;
	float yMountain = (y - 0.775)*(y - 0.775)*2 + 0.07875;
	y = mix(yFlat, yMountain, isMountain)*8192.0;
	vec4 worldPos = vec4(x, y, z, 1.0);
	gl_Position = projection*view*worldPos;
	fragPos = vec3(worldPos);
}

vec2 gradient(int x, int y, int seed)
{
	uint w = 32u;
	uint s = 16u;
	uint a = uint(x);
	uint b = uint(y);
	a *= 3284157443u;
	b ^= a << s | a >> w - s;
	b *= 1911520717u;
	a ^= b << s | b >> w - s;
	a *= 2048419325u;
	float random = a * (3.14159265 / ~(~0u >> 1));

	return vec2(sin(random), cos(random));
}

float fade(float t)
{
	return t*t*t*(t*(t*6 - 15) + 10);
}

float perlin(float x, float y, float cellSize)
{
	float x2 = x/cellSize;
	float y2 = y/cellSize;
	int x0 = int(x2);
	int y0 = int(y2);
	int x1 = x0 + 1;
	int y1 = y0 + 1;
	float u = fade(x2 - float(x0));
	float v = fade(y2 - float(y0));

	vec2 g00 = gradient(x0, y0, 42);
	vec2 g01 = gradient(x0, y1, 42);
	vec2 g10 = gradient(x1, y0, 42);
	vec2 g11 = gradient(x1, y1, 42);

//	float n00 = dot(g00, vec2(u, v));
//	float n01 = dot(g01, vec2(u, v-1));
//	float n10 = dot(g10, vec2(u-1, v));
//	float n11 = dot(g11, vec2(u-1, v-1));
	float n00 = dot(g00, vec2(x2 - float(x0), y2 - float(y0)));
	float n01 = dot(g01, vec2(x2 - float(x0), y2 - float(y1)));
	float n10 = dot(g10, vec2(x2 - float(x1), y2 - float(y0)));
	float n11 = dot(g11, vec2(x2 - float(x1), y2 - float(y1)));

	float nx0 = mix(n00, n10, u);
	float nx1 = mix(n01, n11, u);
	float n = mix(nx0, nx1, v);

	return n;
}
