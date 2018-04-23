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

	const Vector3& getDimensions() const;
	const Vector3& getOrigin() const;

	void addParticle(const SphParticle&);
	std::vector<SphParticle>& getFluidParticles();
	std::vector<SphParticle>& getStaticParticles();
	std::vector<SphParticle*> getParticles();

	void clearParticles();
	void clearParticles(SphParticle::ParticleType);
	const bool hasParticles(SphParticle::ParticleType);

	std::vector<SphParticle> removeParticlesOutsideDomain();

	void clearNeighbourRimParticles();
	void clearNeighbourRimParticles(SphParticle::ParticleType);
	void addNeighbourRimParticles(const std::unordered_map<int, std::vector<SphParticle*>>&, SphParticle::ParticleType);
	std::unordered_map<int, std::vector<SphParticle*>> getNeighbourRimParticles();
	std::unordered_map<int, std::vector<SphParticle*>> getRimParticleTargetMap(SphParticle::ParticleType);

private:
	std::unordered_map <SphParticle::ParticleType, std::vector<SphParticle>> particles;
	std::unordered_map<SphParticle::ParticleType, std::unordered_map<int, std::vector<SphParticle*>>> neighbour_particles;

	Vector3 origin;
	Vector3 dimensions;
};