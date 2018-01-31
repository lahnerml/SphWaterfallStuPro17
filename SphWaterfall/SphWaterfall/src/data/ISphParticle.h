#pragma once
#include "Vector3.h"

class ISphParticle {
public:
	enum ParticleType
	{
		FLUID,
		STATIC,
		MIRRORED
	};

	Vector3 position;
	Vector3 velocity;

	ParticleType getParticleType();
private:
};