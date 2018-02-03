#pragma once
#include "Vector3.h"

class SphParticle {
	public:
		SphParticle();
		SphParticle(Vector3);
		SphParticle(Vector3, double);
		~SphParticle();
		
		Vector3 position;

		double mass;
		double local_density;

		virtual Vector3 getVelocity() = 0;
	private:
};