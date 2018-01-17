#pragma once
#include "ParticleDomain.h"
#include <iterator>

ParticleDomain::ParticleDomain() :
	origin(Vector3()),
	dimensions(Vector3())
{
	ParticleDomain::particles = std::vector<ISphParticle>(0);
}

ParticleDomain::ParticleDomain(const Vector3& origin, const Vector3& size) : 
	origin(origin),
	dimensions(size)
{
	ParticleDomain::particles = std::vector<ISphParticle>();
}

ParticleDomain::~ParticleDomain() {

}

std::vector<ISphParticle> ParticleDomain::removeParticlesOutsideDomain() {
	particles_outside_domain = 0;
	std::vector<ISphParticle> outsideParticles;
	for (int i = 0; i < particles.size(); i++) {
		ISphParticle each_particle = particles.at(i);
		if (!(each_particle.position - origin).in_range_of(dimensions)) {
			particles_outside_domain++;
			outsideParticles.push_back(each_particle);

			auto it = particles.begin() + i;
			std::move(it, it + 1, std::inserter(outsideParticles, outsideParticles.end()));
			particles.erase(it, it + 1);
		}
	}

	return outsideParticles;
}

const Vector3& ParticleDomain::getDimensions() const {
	return dimensions;
}

void ParticleDomain::addParticle(const ISphParticle& particle) {
	particles.push_back(particle);
}
