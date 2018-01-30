#pragma once
#include "mpi.h"
#include "SphKernelFactory.h"
#include "SphNeighbourSearchFactory.h"

#include <vector>
#include <unordered_map>
#include <iterator>



class SphManager {
public:
	SphManager(const Vector3&, double simulation_time, double timestep_duration);
	SphManager();
	~SphManager();

	void simulate();
	void update(double timestep);

	void add_particles(const std::vector<ISphParticle>&);
private:
	int world_size;
	Vector3 domain_dimensions;
	double simulation_time;
	double timestep_duration;
	std::unordered_map<int, ParticleDomain> domains;
	std::vector<ISphParticle> neighbour_particles;
	ISphKernel* kernel;
	ISphNeighbourSearch* neighbour_search;
	SphKernelFactory kernel_factory;
	SphNeighbourSearchFactory neighbour_search_factory;

	void updateVelocity(ISphParticle& particle, double timestep);
	Vector3 computeAcceleration(ISphParticle& particle);
	Vector3 computeDensityAcceleration(ISphParticle& particle);
	Vector3 computeViscosityAcceleration(ISphParticle& particle);
	void setLocalDensities();
	void computeLocalDensity(ISphParticle&);
	double computeLocalPressure(ISphParticle&);
	void findNeighbourDomains(ParticleDomain);

	MPI_Request requestRimParticles(const Vector3&, const Vector3&);
	void exchangeParticles();
	int computeTargetProcess(const ISphParticle&) const;
	int computeTargetDomain(const ISphParticle&) const;
	int hash(const Vector3&) const;
	Vector3 unhash(const int&) const;
	ParticleDomain& getParticleDomain(const int&);
	void sendRimParticles(const int&, const int&);
};