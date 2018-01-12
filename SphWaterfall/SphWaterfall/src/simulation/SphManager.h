#pragma once
#include "ParticleDomain.h"
#include "mpi.h"
#include <unordered_map>

class SphManager {
public:
	SphManager(const Vector3&);
	~SphManager();

	void update(double);
private:
	int worldSize;
	Vector3 domain_dimensions;

	void clearAccellerations();
	void updateVelocity(ISphParticle);
	void computeAccelleration(ISphParticle);
	void computeDenity(ISphParticle);
	void computeViscosity(ISphParticle);
	void findNeighbourDomains(ParticleDomain);

	void add_particles(const std::vector<ISphParticle>&);
	void exchangeParticles();
	int computeTargetProcess(const ISphParticle&);
	int computeTargetDomain(const ISphParticle&);
	ParticleDomain& getParticleDomain(const int&);

	std::unordered_map<int, ParticleDomain> domains;
};