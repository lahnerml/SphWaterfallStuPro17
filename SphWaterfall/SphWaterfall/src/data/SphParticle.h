#pragma once
#include "Vector3.h"

class SphParticle {
	public:
		SphParticle();
		SphParticle(Vector3);
		SphParticle(Vector3, Vector3);
		SphParticle(Vector3, Vector3, double);
		~SphParticle();
		
		Vector3 position;
		Vector3 velocity;
		double mass;
		double local_density;
	private:
};