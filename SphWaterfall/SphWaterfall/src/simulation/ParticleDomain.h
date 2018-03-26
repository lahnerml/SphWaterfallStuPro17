#pragma once
#include "../data/SphParticle.h"
#include "../data/Vector3.h"
#include "../data/NullableWrapper.h"
#include "../data/NullableWrapper.cpp"
#include "../simulation/SimulationUtilities.h"

#include <vector>
#include <iterator>
#include <unordered_map>
#include <iostream>

class ParticleDomain {
public:
	ParticleDomain();
	ParticleDomain(const Vector3& origin, const Vector3& dimension);
	~ParticleDomain();

	std::vector<SphParticle> removeParticlesOutsideDomain();

	int size() const;
	const Vector3& getDimensions() const;
	const Vector3& getOrigin() const;

	void addParticle(const SphParticle&);
	std::vector<SphParticle>& getParticles();
	bool hasFluidParticles();
	std::unordered_map<int, std::vector<SphParticle>> getRimParticleTargetMap(SphParticle::ParticleType particle_type);
	void clearNeighbourRimParticles(SphParticle::ParticleType);
	void addNeighbourRimParticles(const std::unordered_map<int, std::vector<SphParticle>>);
	std::unordered_map<int, std::vector<SphParticle>>& getNeighbourRimParticles();

private:
	std::vector<SphParticle> particles;
	std::unordered_map<int, std::vector<SphParticle>> neighbour_rim_particles;
	std::vector<int> neighbour_domains;
	int number_of_fluid_particles;
	int particles_outside_domain;
	Vector3 origin;
	Vector3 dimensions;
};