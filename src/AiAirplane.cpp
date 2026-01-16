#include "include.h"

float AiAirplane::staticRand = 1.5;

AiAirplane::AiAirplane(Object* object, Checkpoints checkpoints, float speed, float corneringSpeed) : Airplane(object)
{
	this->speed = speed;
	this->corneringSpeed = corneringSpeed;
	this->hermiteT = 0.05;
	this->curCheckpoint = checkpoints.objects.size() - 1;
	this->pseudoRand = 1.0;
	this->pseudoRand2 = 1.3;
	this->lastPseudoRand = 35.0;
	this->lastPseudoRand2 = 35.0;
	this->updateSplinePoints(checkpoints);
}

void AiAirplane::update(float deltaT, Checkpoints checkpoints)
{
	unsigned int numCheckpoints = checkpoints.objects.size();
	float t = this->hermiteT;

	float x1 = this->splineX1;
	float y1 = this->splineY1;
	float z1 = this->splineZ1;
	float angle1 = this->splineAngle1;
	float mag1 = this->splineMag1;
	float x2 = this->splineX2;
	float y2 = this->splineY2;
	float z2 = this->splineZ2;
	float angle2 = this->splineAngle2;
	float mag2 = this->splineMag2;

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
	this->object->roll = (this->object->roll*(1.0 - deltaT*3.0) + yawRate*deltaT*3.0); // LPF
	this->object->pitch = -atan(dy/sqrt(dx*dx + dz*dz));
	this->object->yaw = atan2(dz, dx) - M_PI*0.5;
	this->propellerAngle += 2.0*M_PI/16.0*60.0*(float)deltaT; // duplicate
	std::vector<float> meshAngles = {0.0, 0.0, 0.0, propellerAngle}; // duplicate
	this->object->update(meshAngles); // duplicate
	float corneringMult = abs(clamp(yawRate, -M_PI*0.5, M_PI*0.5)/(M_PI*0.5));
	float trueSpeed = this->speed*(1.0 - (1.0 - this->corneringSpeed)*corneringMult);
	this->hermiteT += deltaT*trueSpeed/dl;
	if (this->hermiteT >= 1.0) {
		this->hermiteT = 0.0;
		this->curCheckpoint++;
		this->curCheckpoint %= numCheckpoints;
		this->lastPseudoRand = pseudoRand;
		this->lastPseudoRand2 = pseudoRand2;
		this->pseudoRand = fmod(this->pseudoRand*420.0, 69.69);
		this->staticRand = fmod(this->staticRand*314.59, 67.67);
		this->pseudoRand2 = this->staticRand;
		this->updateSplinePoints(checkpoints);
	}
}

void AiAirplane::updateSplinePoints(Checkpoints checkpoints)
{
	unsigned int numCheckpoints = checkpoints.objects.size();
	unsigned int nextCheckpoint = (this->curCheckpoint + 1)%numCheckpoints;

	float lastRand1 = lastPseudoRand/70.0*2.0 - 1.0;
	float lastRand2 = fmod(lastPseudoRand*lastPseudoRand2, 42.0)/42.0*2.0 - 1.0;
	float lastRand3 = lastPseudoRand2/70.0*2.0 - 1.0;
	float lastRand4 = fmod(lastPseudoRand*lastPseudoRand2, 69.0)/69.0*2.0 - 1.0;

	float rand1 = pseudoRand/70.0*2.0 - 1.0;
	float rand2 = fmod(pseudoRand*pseudoRand2, 42.0)/42.0*2.0 - 1.0;
	float rand3 = pseudoRand2/70.0*2.0 - 1.0;
	float rand4 = fmod(pseudoRand*pseudoRand2, 69.0)/69.0*2.0- 1.0;

	this->splineX1 = checkpoints.objects[this->curCheckpoint].x + lastRand1*CHECKPOINT_RADIUS*0.7;
	this->splineY1 = checkpoints.objects[this->curCheckpoint].y + lastRand2*CHECKPOINT_RADIUS*0.7;
	this->splineZ1 = checkpoints.objects[this->curCheckpoint].z + lastRand3*CHECKPOINT_RADIUS*0.7;
	this->splineAngle1 = -checkpoints.objects[this->curCheckpoint].yaw + lastRand4*0.1;
	this->splineMag1 = checkpoints.tangentMagsStart[this->curCheckpoint];

	this->splineX2 = checkpoints.objects[nextCheckpoint].x + rand1*CHECKPOINT_RADIUS*0.7;
	this->splineY2 = checkpoints.objects[nextCheckpoint].y + rand2*CHECKPOINT_RADIUS*0.7;
	this->splineZ2 = checkpoints.objects[nextCheckpoint].z + rand3*CHECKPOINT_RADIUS*0.7;
	this->splineAngle2 = -checkpoints.objects[nextCheckpoint].yaw + rand4*0.1;
	this->splineMag2 = checkpoints.tangentMagsEnd[this->curCheckpoint];
}
