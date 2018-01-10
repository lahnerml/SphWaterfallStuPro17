#pragma once
#include "ParticleDomain.h"

ParticleDomain::ParticleDomain() :
	origin(Vector3()),
	dimensions(Vector3())
{
	ParticleDomain::particles = std::vector<ISphParticle>(0);
}

ParticleDomain::ParticleDomain(const ISphParticle* particles, const int& particle_count, const Vector3& origin, const Vector3& size) : 
	origin(origin),
	dimensions(size)
{
	ParticleDomain::particles = std::vector<ISphParticle>(particle_count);
	for (int i = 0; i < particle_count; i++) {
		ParticleDomain::particles.push_back(particles[i]);
	}
}

ParticleDomain::~ParticleDomain() {

}

void ParticleDomain::updateParticlesOutsideDomain() {
	particlesOutsideDomain = 0;
	for (auto each_particle : particles) {
		if ((each_particle.position - origin).all_dimensions_smaller_or_equal_then(dimensions)) {
			particlesOutsideDomain++;
		}
	}
}