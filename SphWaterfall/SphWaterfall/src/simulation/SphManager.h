#pragma once
#include "ParticleDomain.h"
#include "mpi.h"

class SphManager {
public:
	SphManager();
	~SphManager();

	void update(double);
private:
	void clearAccellerations();
	void updateVelocity(ISphParticle);
	void computeAccelleration(ISphParticle);
	void computeDenity(ISphParticle);
	void computeViscosity(ISphParticle);
	void findNeighbourDomains(ParticleDomain);

	ParticleDomain domains[];
};