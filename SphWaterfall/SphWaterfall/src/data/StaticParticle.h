#pragma once
#include "ISphParticle.h"

class StaticParticle : public ISphParticle {
public:
	StaticParticle();
	StaticParticle(Vector3);
	~StaticParticle();

	virtual ISphParticle::ParticleType getParticleType();
private:
	Vector3 position;
};
