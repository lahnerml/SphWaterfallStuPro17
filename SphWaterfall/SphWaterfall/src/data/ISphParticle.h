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

		virtual Vector3 position() { return Vector3(); }
		virtual void position(Vector3 position) { }
		
		virtual Vector3 velocity() { return Vector3(); }
		virtual void velocity(Vector3 velocity) { }

		virtual ParticleType getParticleType() { return ParticleType::FLUID; }
	private:
};