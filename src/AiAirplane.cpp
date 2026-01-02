#include "include.h"

AiAirplane::AiAirplane(Object* object) : Airplane(object)
{
	this->hermiteT = 0.05;
	this->curCheckpoint = 16;
}

void AiAirplane::update(float deltaT, Checkpoints checkpoints)
{
	unsigned int numCheckpoints = checkpoints.objects.size();
	float t = this->hermiteT;

	unsigned int nextCheckpoint = (curCheckpoint + 1)%numCheckpoints;
	float x1 = checkpoints.objects[curCheckpoint].x;
	float y1 = checkpoints.objects[curCheckpoint].y;
	float z1 = checkpoints.objects[curCheckpoint].z;
	float angle1 = -checkpoints.objects[curCheckpoint].yaw;
	float x2 = checkpoints.objects[nextCheckpoint].x;
	float y2 = checkpoints.objects[nextCheckpoint].y;
	float z2 = checkpoints.objects[nextCheckpoint].z;
	float angle2 = -checkpoints.objects[nextCheckpoint].yaw;
	float mag1 = checkpoints.tangentMagsStart[curCheckpoint];
	float mag2 = checkpoints.tangentMagsEnd[curCheckpoint];

	// Hermite Spline Calcs
	float dx1 = mag1*sin(angle1);
	float dy1 = 0.0;
	float dz1 = mag1*cos(angle1);
	float dx2 = mag2*sin(angle2);
	float dy2 = 0.0;
	float dz2 = mag2*cos(angle2);
	float h00 = 2.0*t*t*t - 3.0*t*t + 1;
	float h10 = t*t*t - 2.0*t*t + t;
	float h01 = -2.0*t*t*t + 3.0*t*t;
	float h11 = t*t*t - t*t;
	float x = h00*x1 + h10*dx1 + h01*x2 + h11*dx2;
	float y = h00*y1 + h10*dy1 + h01*y2 + h11*dy2;
	float z = h00*z1 + h10*dz1 + h01*z2 + h11*dz2;

	float dh00 = 6.0*t*t - 6.0*t;
	float dh10 = 3.0*t*t - 4.0*t + 1.0;
	float dh01 = -6.0*t*t + 6.0*t;
	float dh11 = 3.0*t*t - 2.0*t;
	float dx = dh00*x1 + dh10*dx1 + dh01*x2 + dh11*dx2;
	float dy = dh00*y1 + dh10*dy1 + dh01*y2 + dh11*dy2;
	float dz = dh00*z1 + dh10*dz1 + dh01*z2 + dh11*dz2;
	float dl = sqrt(dx*dx + dy*dy + dz*dz);

	float ddh00 = 12.0*t - 6.0;
	float ddh10 = 6.0*t - 4.0;
	float ddh01 = -12.0*t + 6.0;
	float ddh11 = 6.0*t - 2.0;
	float ddx = ddh00*x1 + ddh10*dx1 + ddh01*x2 + ddh11*dx2;
	float ddz = ddh00*z1 + ddh10*dz1 + ddh01*z2 + ddh11*dz2;

	float cross = dx*ddz - dz*ddx;
	float denom = pow(dx*dx + dz*dz, 1.5);
	float yawRate = this->speed*cross/denom;

	this->object->x = x;
	this->object->y = y;
	this->object->z = z;
	this->object->roll = (this->object->roll*(1.0 - deltaT*6.0) + yawRate*deltaT*6.0);
	this->object->pitch = -atan(dy/sqrt(dx*dx + dz*dz));
	this->object->yaw = atan2(dz, dx) - M_PI*0.5;
	this->object->update();
	this->hermiteT += deltaT*this->speed/dl;
	if (this->hermiteT >= 1.0) {
		this->hermiteT = 0.0;
		curCheckpoint++;
		this->curCheckpoint %= numCheckpoints;
	}
}

void AiAirplane::setPersonality(float speed)
{
	this->speed = speed;
}
