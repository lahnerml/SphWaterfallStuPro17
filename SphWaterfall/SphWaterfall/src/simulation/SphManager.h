#pragma once
#include <unordered_map>
#include <vector>
#include <iterator>

#include "mpi.h"

#include "ParticleDomain.h"

#include "SphKernelFactory.h"
#include "SphNeighbourSearchFactory.h"

class SphManager {
public:
	SphManager(const Vector3&);
	~SphManager();

	void update(double);
private:
	int world_size;
	Vector3 domain_dimensions;
	std::unordered_map<int, ParticleDomain> domains;
	std::vector<ISphParticle> neighbour_particles;

	ISphKernel* kernel;
	ISphNeighbourSearch* neighbour_search;

	SphKernelFactory kernel_factory;
	SphNeighbourSearchFactory neighbour_search_factory;

	void clearAccellerations();
	void updateVelocity(ISphParticle& particle);
	Vector3 computeAccelleration(ISphParticle& particle);
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