#pragma once
#include "../data/ISphParticle.h"
#include "../data/Vector3.h"

#include <vector>

class ParticleDomain {
public:
	ParticleDomain();
	ParticleDomain(const ISphParticle* particles, const int& particle_count, const Vector3& center, const Vector3& size);
	~ParticleDomain();

	void updateParticlesOutsideDomain();

private:
	std::vector<ISphParticle> particles;
	std::vector<ParticleDomain> neighbourDomains;
	int particlesOutsideDomain;
	Vector3 origin;
	Vector3 dimensions;
};