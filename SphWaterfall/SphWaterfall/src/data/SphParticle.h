#pragma once
#include "ISphParticle.h"

class SphParticle : public ISphParticle {
	public:
		SphParticle();
		SphParticle(Vector3);
		SphParticle(Vector3, Vector3);
		~SphParticle();

		Vector3 velocity;
	private:
};