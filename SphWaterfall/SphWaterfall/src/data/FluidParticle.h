#pragma once
#include "SphParticle.h"

class FluidParticle : public SphParticle {
	public:
		FluidParticle(Vector3);
		FluidParticle(Vector3, Vector3);
		FluidParticle(Vector3, Vector3, double);
		~FluidParticle();
		
		Vector3 getVelocity();
		void setVelocity(Vector3);
	private:
		Vector3 velocity;
};