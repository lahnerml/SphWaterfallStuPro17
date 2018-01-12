#pragma once
#include "../data/ISphParticle.h"
#include "../data/Vector3.h"

#include <vector>

class ParticleDomain {
public:
	ParticleDomain();
	ParticleDomain(const Vector3&, const Vector3&);
	~ParticleDomain();

	std::vector<ISphParticle> removeParticlesOutsideDomain();

	const Vector3& getDimensions() const;
	void addParticle(const ISphParticle&);

private:
	std::vector<ISphParticle> particles;
	std::vector<ISphParticle> static_particles;
	std::vector<int> neighbourDomains;
	int particlesOutsideDomain;
	Vector3 origin;
	Vector3 dimensions;
};