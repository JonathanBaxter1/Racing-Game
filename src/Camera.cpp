#include "include.h"

namespace Camera
{//

float pitch = 0.0;
float yaw = 0.0;
float x = 0.0;
float y = 500.0;
float z = 0.0;
mat4 viewMatrix = {0.0};
mat4 projMatrix = {0.0};

void update(Airplane* airplane)
{
	pitch = 0.0;
	yaw = M_PI + airplane->object->yaw;
	float distanceFromAirplane = (30.0 + airplane->speed*0.05);
	x = airplane->object->x + distanceFromAirplane*sin(airplane->object->yaw);
	y = airplane->object->y;
	z = airplane->object->z - distanceFromAirplane*cos(airplane->object->yaw);

}

}//
