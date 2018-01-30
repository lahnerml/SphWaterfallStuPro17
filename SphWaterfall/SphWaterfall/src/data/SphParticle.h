#pragma once
#include "ISphParticle.h"

class SphParticle : public ISphParticle {
	public:
		SphParticle();
		SphParticle(Vector3);
		SphParticle(Vector3, Vector3);
		~SphParticle();

		virtual Vector3 position();
		virtual void position(Vector3 position);

		virtual Vector3 velocity();
		virtual void velocity(Vector3 velocity);

		virtual ISphParticle::ParticleType getParticleType();
	private:
		Vector3 pos;
		Vector3 vel;
};