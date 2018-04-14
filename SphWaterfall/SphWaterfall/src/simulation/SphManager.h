#pragma once
#include "mpi.h"
#include "SphKernelFactory.h"
#include "SphNeighbourSearchFactory.h"
#include "SimulationUtilities.h"

#include <vector>
#include <array>
#include <unordered_map>
#include <iterator>
#include <random>
#include <functional>

class SphManager {
public:
	SphManager(const Vector3&, int number_of_timesteps, double timestep_duration);
	~SphManager();

	void simulate();
	void add_particles(const std::vector<SphParticle>&);
	void exportParticles();
	void setSink(const double&);
	void addSource(const Vector3&);
	const Vector3& getDomainDimensions() const;

private:
	int mpi_rank;
	Vector3 domain_dimensions;
	int number_of_timesteps;
	double sink_height;
	double const timestep_duration;
	double half_timestep_duration;
	Vector3 const gravity_acceleration;

	std::unordered_map<int, ParticleDomain> domains;
	std::unordered_map<int, std::vector<SphParticle>> add_particles_map;
	std::vector<std::pair<SphParticle, std::vector<SphParticle>>> neighbour_particles;
	std::vector<Vector3> sources;

	ISphKernel* kernel;
	ISphNeighbourSearch* neighbour_search;
	SphKernelFactory kernel_factory;
	SphNeighbourSearchFactory neighbour_search_factory;

	ParticleDomain& getParticleDomain(const int&);
	ParticleDomain& getParticleDomain(const Vector3&);
	
	void cleanUpAllParticles();
	void cleanUpFluidParticles();
	void cleanUpStaticParticles();

	void update();
	bool updateVelocity(SphParticle& particle);
	Vector3 computeAcceleration(SphParticle& particle);
	Vector3 computeDensityAcceleration(SphParticle& particle);
	Vector3 computeViscosityAcceleration(SphParticle& particle);
	void computeLocalDensity(SphParticle&);
	double computeLocalPressure(SphParticle&);
	void exchangeParticles();
	void exchangeRimParticles(SphParticle::ParticleType);
	void spawnSourceParticles();
};