#pragma once
#include "ISphParticle.h"

class SphParticle : public ISphParticle {
	public:
		enum ParticleType
		{
			FLUID,
			STATIC,
			MIRRORED
		};

		SphParticle();
		SphParticle(Vector3);
		SphParticle(Vector3, Vector3);
		SphParticle(Vector3, SphParticle::ParticleType);
		~SphParticle();

		Vector3 position;
		Vector3 velocity;

		ParticleType getParticleType();
	private:
		const ParticleType pType;
};