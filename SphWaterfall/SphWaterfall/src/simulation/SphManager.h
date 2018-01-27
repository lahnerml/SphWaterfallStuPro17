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
	int world_size;
	Vector3 domain_dimensions;
	std::unordered_map<int, ParticleDomain> domains;

	void clearAccellerations();
	void updateVelocity(ISphParticle& particle);
	void computeAccelleration(ISphParticle& particle);
	void computeDensity(ISphParticle&);
	void computeViscosity(ISphParticle& particle);
	void findNeighbourDomains(ParticleDomain);

	void add_particles(const std::vector<ISphParticle>&);
	void exchangeParticles();
	int computeTargetProcess(const ISphParticle&) const;
	int computeTargetDomain(const ISphParticle&) const;
	int hash(const Vector3&) const;
	Vector3& unhash(const int&) const;
	ParticleDomain& getParticleDomain(const int&);
};