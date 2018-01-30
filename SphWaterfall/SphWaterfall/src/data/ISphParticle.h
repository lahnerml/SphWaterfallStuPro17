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

		virtual ParticleType getParticleType() { return ParticleType::FLUID; }

		Vector3 position;
		Vector3 velocity;
	private:
};
