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
	void resetRimParticles();
	void clearNeighbourRimParticles(SphParticle::ParticleType);
	void setNeighbourRimParticles(const std::unordered_map<int, std::vector<SphParticle>>);
	std::unordered_map<int, std::vector<SphParticle>>& getNeighbourRimParticles();

	std::vector<SphParticle> getTopRimParticles(SphParticle::ParticleType);
	std::vector<SphParticle> getBottomRimParticles(SphParticle::ParticleType);
	std::vector<SphParticle> getLeftRimParticles(SphParticle::ParticleType);
	std::vector<SphParticle> getRightRimParticles(SphParticle::ParticleType);
	std::vector<SphParticle> getFrontRimParticles(SphParticle::ParticleType);
	std::vector<SphParticle> getBackRimParticles(SphParticle::ParticleType);
	std::vector<SphParticle> getFrontBottomRimParticles(SphParticle::ParticleType);
	std::vector<SphParticle> getFrontTopRimParticles(SphParticle::ParticleType);
	std::vector<SphParticle> getBackBottomRimParticles(SphParticle::ParticleType);
	std::vector<SphParticle> getBackTopRimParticles(SphParticle::ParticleType);
	std::vector<SphParticle> getFrontLeftRimParticles(SphParticle::ParticleType);
	std::vector<SphParticle> getFrontRightRimParticles(SphParticle::ParticleType);
	std::vector<SphParticle> getBackLeftRimParticles(SphParticle::ParticleType);
	std::vector<SphParticle> getBackRightRimParticles(SphParticle::ParticleType);
	std::vector<SphParticle> getLeftBottomRimParticles(SphParticle::ParticleType);
	std::vector<SphParticle> getLeftTopRimParticles(SphParticle::ParticleType);
	std::vector<SphParticle> getRightBottomRimParticles(SphParticle::ParticleType);
	std::vector<SphParticle> getRightTopRimParticles(SphParticle::ParticleType);
	std::vector<SphParticle> getFrontLeftBottomRimParticles(SphParticle::ParticleType);
	std::vector<SphParticle> getFrontRightBottomRimParticles(SphParticle::ParticleType);
	std::vector<SphParticle> getFrontLeftTopRimParticles(SphParticle::ParticleType);
	std::vector<SphParticle> getFrontRightTopRimParticles(SphParticle::ParticleType);
	std::vector<SphParticle> getBackLeftBottomRimParticles(SphParticle::ParticleType);
	std::vector<SphParticle> getBackRightBottomRimParticles(SphParticle::ParticleType);
	std::vector<SphParticle> getBackLeftTopRimParticles(SphParticle::ParticleType);
	std::vector<SphParticle> getBackRightTopRimParticles(SphParticle::ParticleType);

private:
	std::vector<SphParticle> particles;
	std::unordered_map<int, std::vector<SphParticle>> neighbour_rim_particles;

	std::vector<int> neighbour_domains;

	int number_of_fluid_particles;

	NullableWrapper<std::vector<SphParticle>> front_rim_particles;
	NullableWrapper<std::vector<SphParticle>> back_rim_particles;
	NullableWrapper<std::vector<SphParticle>> left_rim_particles;
	NullableWrapper<std::vector<SphParticle>> right_rim_particles;
	NullableWrapper<std::vector<SphParticle>> top_rim_particles;
	NullableWrapper<std::vector<SphParticle>> bottom_rim_particles;
	NullableWrapper<std::vector<SphParticle>> front_bottom_rim_particles;
	NullableWrapper<std::vector<SphParticle>> front_top_rim_particles;
	NullableWrapper<std::vector<SphParticle>> back_bottom_rim_particles;
	NullableWrapper<std::vector<SphParticle>> back_top_rim_particles;
	NullableWrapper<std::vector<SphParticle>> front_left_rim_particles;
	NullableWrapper<std::vector<SphParticle>> front_right_rim_particles;
	NullableWrapper<std::vector<SphParticle>> back_left_rim_particles;
	NullableWrapper<std::vector<SphParticle>> back_right_rim_particles;
	NullableWrapper<std::vector<SphParticle>> left_bottom_rim_particles;
	NullableWrapper<std::vector<SphParticle>> left_top_rim_particles;
	NullableWrapper<std::vector<SphParticle>> right_bottom_rim_particles;
	NullableWrapper<std::vector<SphParticle>> right_top_rim_particles;
	NullableWrapper<std::vector<SphParticle>> front_left_bottom_rim_particles;
	NullableWrapper<std::vector<SphParticle>> front_right_bottom_rim_particles;
	NullableWrapper<std::vector<SphParticle>> front_left_top_rim_particles;
	NullableWrapper<std::vector<SphParticle>> front_right_top_rim_particles;
	NullableWrapper<std::vector<SphParticle>> back_left_bottom_rim_particles;
	NullableWrapper<std::vector<SphParticle>> back_right_bottom_rim_particles;
	NullableWrapper<std::vector<SphParticle>> back_left_top_rim_particles;
	NullableWrapper<std::vector<SphParticle>> back_right_top_rim_particles;
	
	int particles_outside_domain;
	Vector3 origin;
	Vector3 dimensions;
};