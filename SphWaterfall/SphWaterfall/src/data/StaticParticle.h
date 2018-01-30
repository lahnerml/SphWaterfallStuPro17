#pragma once
#include "ISphParticle.h"

class StaticParticle : public IStaticParticle {
public:
	StaticParticle();
	StaticParticle(Vector3);
	~StaticParticle();

	virtual Vector3 position();
	virtual void position(Vector3 position);

	virtual Vector3 velocity();
	virtual void velocity(Vector3 velocity);

	virtual IStaticParticle::ParticleType getParticleType();
private:
	Vector3 pos;
};