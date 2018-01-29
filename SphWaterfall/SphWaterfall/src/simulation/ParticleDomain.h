#pragma once
#include "../data/ISphParticle.h"
#include "../data/Vector3.h"

#include <vector>
#include <iterator>

class ParticleDomain {
public:
	ParticleDomain();
	ParticleDomain(const Vector3&, const Vector3&);
	~ParticleDomain();

	std::vector<ISphParticle> removeParticlesOutsideDomain();

	const Vector3& getDimensions() const;
	void addParticle(const ISphParticle&);
	std::vector<ISphParticle> getParticles();

private:
	std::vector<ISphParticle> particles;
	std::vector<ISphParticle> static_particles;
	std::vector<int> neighbour_domains;
	int particles_outside_domain;
	Vector3 origin;
	Vector3 dimensions;
};