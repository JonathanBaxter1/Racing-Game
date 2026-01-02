#pragma once

class Checkpoints;

class AiAirplane : public Airplane
{
private:
	float hermiteT;
	unsigned int curCheckpoint;
public:
	AiAirplane(Object* object);
	void setPersonality(float speed);
	void update(float deltaT, Checkpoints checkpoints);
};
