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

		virtual Vector3 position() = 0;
		virtual void position(Vector3 position) = 0;
		
		virtual Vector3 velocity() = 0;
		virtual void velocity(Vector3 velocity) = 0;

		virtual ParticleType getParticleType() = 0;
	private:
};