#include "include.h"

Airplane::Airplane(Object* object)
{
	this->timeSinceBoost = 100.0;
	this->object = object;
}

void Airplane::aiUpdate(float deltaT, Checkpoints checkpoints)
{
	static float t = 0.0;
	static unsigned int curCheckpoint = 0;

	unsigned int numCheckpoints = checkpoints.objects.size();
	unsigned int nextCheckpoint = (curCheckpoint + 1)%numCheckpoints;
	unsigned int nextCheckpoint2 = (curCheckpoint + 2)%numCheckpoints;
	unsigned int nextCheckpoint3 = (curCheckpoint + 3)%numCheckpoints;
	unsigned int lastCheckpoint = (curCheckpoint + numCheckpoints - 1)%numCheckpoints;
	float x1 = checkpoints.objects[curCheckpoint].x;
	float y1 = checkpoints.objects[curCheckpoint].y;
	float z1 = checkpoints.objects[curCheckpoint].z;
	float angle1 = -checkpoints.objects[curCheckpoint].yaw;
	float x2 = checkpoints.objects[nextCheckpoint].x;
	float y2 = checkpoints.objects[nextCheckpoint].y;
	float z2 = checkpoints.objects[nextCheckpoint].z;
	float angle2 = -checkpoints.objects[nextCheckpoint].yaw;
	float mag1 = checkpoints.tangentMags[curCheckpoint];
	float mag2 = checkpoints.tangentMags[nextCheckpoint];

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

	float cross = dx * ddz - dz * ddx;
	float denom = pow(dx*dx + dz*dz, 1.5);
	float yawRate = speed * cross / denom;

	this->object->x = x;
	this->object->y = y;
	this->object->z = z;
	this->object->roll = yawRate;
	this->object->pitch = -atan(dy/sqrt(dx*dx + dz*dz));
	this->object->yaw = atan2(dz, dx) - M_PI*0.5;
	this->object->update();
	t += deltaT*100.0/dl;
	if (t >= 1.0) {
		t = 0.0;
		curCheckpoint++;
		curCheckpoint %= numCheckpoints;
	}
}

void Airplane::playerUpdate(float deltaT, vec3 controls)
{
	float desiredTurnAngle = controls.x;
	float desiredPitch = controls.y;
	float desiredSpeed = controls.z;
	float speedBoost = 0.0;
	if (timeSinceBoost < 0.25) {
		speedBoost = 4.0*timeSinceBoost*180.0;
	} else if (timeSinceBoost < 2.0) {
		speedBoost = 180.0;
	} else if (timeSinceBoost < 2.25) {
		speedBoost = 4.0*(2.25 - timeSinceBoost)*180.0;
	}
	this->timeSinceBoost += deltaT;
	this->object->roll = desiredTurnAngle;
	this->object->pitch = desiredPitch;
	this->object->yaw += desiredTurnAngle*deltaT;
	this->speed = (desiredSpeed + speedBoost + sin(this->object->pitch)*40.0);
	this->object->z += deltaT*this->speed*cos(this->object->yaw)*cos(this->object->pitch);
	this->object->x -= deltaT*this->speed*sin(this->object->yaw)*cos(this->object->pitch);
	this->object->y -= deltaT*this->speed*sin(this->object->pitch);
	this->object->update();
}

void Airplane::performBoost()
{
	timeSinceBoost = 0.0;
}

void Airplane::checkCollision(float waterHeight, unsigned short* heightMap, unsigned int heightMapWidth, unsigned int heightMapHeight, float heightMapMax)
{
	unsigned int roundedPlayerX = clamp((unsigned int)this->object->x, 0, heightMapWidth - 1);
	unsigned int roundedPlayerZ = clamp((unsigned int)this->object->z, 0, heightMapHeight - 1);
	float terrainHeight = heightMap[roundedPlayerX + roundedPlayerZ*heightMapWidth]/65536.0*heightMapMax;
	if (this->object->y < waterHeight || this->object->y < terrainHeight) {
		std::cout << "You crashed your plane!" << std::endl;
		exit(0);
	}
}

void Airplane::render(Shader textureShader, Shader colorShader, unsigned int frame)
{
	this->object->render(textureShader, colorShader, frame);
}
