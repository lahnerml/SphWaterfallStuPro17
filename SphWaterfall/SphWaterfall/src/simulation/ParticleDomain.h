#pragma once
#include "../data/ISphParticle.h"
#include "../data/Vector3.h"

#include <vector>

class ParticleDomain {
public:
	ParticleDomain(ISphParticle particles[], Vector3, Vector3);
	~ParticleDomain();

	void updateParticlesOutsideDomain();

private:
	ISphParticle particles;
	std::vector<ParticleDomain> neighbourDomains;
	int particlesOutsideDomain;
	Vector3 center;
	Vector3 size;
};