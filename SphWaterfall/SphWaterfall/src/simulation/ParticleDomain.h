#pragma once
#include "../data/ISphParticle.h"
#include "../data/Vector3.h"
#include "../data/NullableWrapper.h"
#include "../data/NullableWrapper.cpp"

#include <vector>
#include <iterator>

class ParticleDomain {
public:
	ParticleDomain();
	ParticleDomain(const Vector3&, const Vector3&);
	~ParticleDomain();

	std::vector<ISphParticle> removeParticlesOutsideDomain();

	const Vector3& getDimensions() const;
	const Vector3& getOrigin() const;
	void addParticle(const ISphParticle&);
	std::vector<ISphParticle> getParticles();

	std::vector<ISphParticle> getTopRimParticles();
	std::vector<ISphParticle> getBottomRimParticles();
	std::vector<ISphParticle> getLeftRimParticles();
	std::vector<ISphParticle> getRightRimParticles();
	std::vector<ISphParticle> getFrontRimParticles();
	std::vector<ISphParticle> getBackRimParticles();
	void resetRimParticles();

private:
	std::vector<ISphParticle> particles;
	std::vector<ISphParticle> static_particles;
	std::vector<int> neighbour_domains;

	NullableWrapper<std::vector<ISphParticle>> front_rim_particles;
	NullableWrapper<std::vector<ISphParticle>> back_rim_particles;
	NullableWrapper<std::vector<ISphParticle>> left_rim_particles;
	NullableWrapper<std::vector<ISphParticle>> right_rim_particles;
	NullableWrapper<std::vector<ISphParticle>> top_rim_particles;
	NullableWrapper<std::vector<ISphParticle>> bottom_rim_particles;

	int particles_outside_domain;
	double rim_distance = 2;
	Vector3 origin;
	Vector3 dimensions;
};