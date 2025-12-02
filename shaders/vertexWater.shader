#version 330 core
layout (location = 0) in vec2 aPos;

out vec3 fragPos;
out vec3 normal;

uniform mat4 view;
uniform mat4 projection;
uniform uint innerWidth;
uniform uint outerWidth;
uniform uint squareSize;
uniform float centerX;
uniform float centerY;

void main()
{
	float centerXrounded = floor(centerX/64.0)*64.0;
	float centerYrounded = floor(centerY/64.0)*64.0;

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
	vec4 worldPos = vec4(x, y, z, 1.0);
	gl_Position = projection*view*worldPos;
	fragPos = vec3(worldPos);

	normal = normalize(vec3(0.0, 1.0, 0.0));
}
