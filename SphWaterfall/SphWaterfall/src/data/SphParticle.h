#pragma once
#include "Vector3.h"
#include "../simulation/SimulationUtilities.h"

class SphParticle {
	public:
		enum ParticleType
		{
			FLUID,
			STATIC,
			MIRRORED
		};

		SphParticle();
		SphParticle(Vector3 position);
		SphParticle(Vector3 position, Vector3 velocity);
		SphParticle(Vector3 position, Vector3 velocity, double mass);
		SphParticle(Vector3 position, ParticleType particle_type);
		~SphParticle();

		friend bool operator==(const SphParticle&, const SphParticle&);

		friend std::ostream& operator<<(std::ostream&, const SphParticle&);

		Vector3 position;
		Vector3 velocity;
		double mass;
		double local_density;

		ParticleType getParticleType() const;
	private:
		ParticleType particle_type;
};