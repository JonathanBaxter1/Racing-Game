#pragma once

class Checkpoints;

class AiAirplane : public Airplane
{
private:
	float corneringSpeed;
	float splineX1, splineY1, splineZ1, splineAngle1, splineMag1, splineX2, splineY2, splineZ2, splineAngle2, splineMag2;
	float hermiteT;
	unsigned int curCheckpoint;
	float pseudoRand;
	float pseudoRand2;
	float lastPseudoRand;
	float lastPseudoRand2;
public:
	static float staticRand;
	AiAirplane(Object* object, Checkpoints checkpoints, float speed, float corneringSpeed);
	void update(float deltaT, Checkpoints checkpoints);
	void updateSplinePoints(Checkpoints checkpoints);
};
