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
	std::vector<ISphParticle> getFrontBottomRimParticles();
	std::vector<ISphParticle> getFrontTopRimParticles();
	std::vector<ISphParticle> getBackBottomRimParticles();
	std::vector<ISphParticle> getBackTopRimParticles();
	std::vector<ISphParticle> getFrontLeftRimParticles();
	std::vector<ISphParticle> getFrontRightRimParticles();
	std::vector<ISphParticle> getBackLeftRimParticles();
	std::vector<ISphParticle> getBackRightRimParticles();
	std::vector<ISphParticle> getLeftBottomRimParticles();
	std::vector<ISphParticle> getLeftTopRimParticles();
	std::vector<ISphParticle> getRightBottomRimParticles();
	std::vector<ISphParticle> getRightTopRimParticles();
	std::vector<ISphParticle> getFrontLeftBottomRimParticles();
	std::vector<ISphParticle> getFrontRightBottomRimParticles();
	std::vector<ISphParticle> getFrontLeftTopRimParticles();
	std::vector<ISphParticle> getFrontRightTopRimParticles();
	std::vector<ISphParticle> getBackLeftBottomRimParticles();
	std::vector<ISphParticle> getBackRightBottomRimParticles();
	std::vector<ISphParticle> getBackLeftTopRimParticles();
	std::vector<ISphParticle> getBackRightTopRimParticles();
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
	NullableWrapper<std::vector<ISphParticle>> front_bottom_rim_particles;
	NullableWrapper<std::vector<ISphParticle>> front_top_rim_particles;
	NullableWrapper<std::vector<ISphParticle>> back_bottom_rim_particles;
	NullableWrapper<std::vector<ISphParticle>> back_top_rim_particles;
	NullableWrapper<std::vector<ISphParticle>> front_left_rim_particles;
	NullableWrapper<std::vector<ISphParticle>> front_right_rim_particles;
	NullableWrapper<std::vector<ISphParticle>> back_left_rim_particles;
	NullableWrapper<std::vector<ISphParticle>> back_right_rim_particles;
	NullableWrapper<std::vector<ISphParticle>> left_bottom_rim_particles;
	NullableWrapper<std::vector<ISphParticle>> left_top_rim_particles;
	NullableWrapper<std::vector<ISphParticle>> right_bottom_rim_particles;
	NullableWrapper<std::vector<ISphParticle>> right_top_rim_particles;
	NullableWrapper<std::vector<ISphParticle>> front_left_bottom_rim_particles;
	NullableWrapper<std::vector<ISphParticle>> front_right_bottom_rim_particles;
	NullableWrapper<std::vector<ISphParticle>> front_left_top_rim_particles;
	NullableWrapper<std::vector<ISphParticle>> front_right_top_rim_particles;
	NullableWrapper<std::vector<ISphParticle>> back_left_bottom_rim_particles;
	NullableWrapper<std::vector<ISphParticle>> back_right_bottom_rim_particles;
	NullableWrapper<std::vector<ISphParticle>> back_left_top_rim_particles;
	NullableWrapper<std::vector<ISphParticle>> back_right_top_rim_particles;
	
	int particles_outside_domain;
	double rim_distance = 2;
	Vector3 origin;
	Vector3 dimensions;
};