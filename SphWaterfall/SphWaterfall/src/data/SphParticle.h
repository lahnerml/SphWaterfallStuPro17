#pragma once
#include "Vector3.h"

class SphParticle {
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
		SphParticle(Vector3, Vector3, double);
		~SphParticle();

		friend bool operator== (const SphParticle a, const SphParticle b);

		Vector3 position;
		Vector3 velocity;
		double mass;
		double local_density;
	private:
};