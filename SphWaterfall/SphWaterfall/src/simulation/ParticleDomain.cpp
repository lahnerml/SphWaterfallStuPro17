#pragma once
#include "ParticleDomain.h"

ParticleDomain::ParticleDomain() :
	origin(Vector3()),
	dimensions(Vector3())
{
	particles = std::vector<ISphParticle>(0);
}

ParticleDomain::ParticleDomain(const Vector3& origin, const Vector3& size) : 
	origin(origin),
	dimensions(size)
{
	particles = std::vector<ISphParticle>(0);
}

ParticleDomain::~ParticleDomain() {

}

std::vector<ISphParticle> ParticleDomain::removeParticlesOutsideDomain() {
	particles_outside_domain = 0;
	std::vector<ISphParticle> outsideParticles;
	for (int i = 0; i < particles.size(); i++) {
		ISphParticle each_particle = particles.at(i);
		if (!(each_particle.position - origin).in_range_of(dimensions)) {
			particles_outside_domain++;
			outsideParticles.push_back(each_particle);

			auto it = particles.begin() + i;
			std::move(it, it + 1, std::inserter(outsideParticles, outsideParticles.end()));
			particles.erase(it, it + 1);
		}
	}

	return outsideParticles;
}

const Vector3& ParticleDomain::getDimensions() const {
	return dimensions;
}

const Vector3& ParticleDomain::getOrigin() const {
	return origin;
}

void ParticleDomain::addParticle(const ISphParticle& particle) {
	particles.push_back(particle);
}

std::vector<ISphParticle> ParticleDomain::getParticles() {
	return particles;
}

std::vector<ISphParticle> ParticleDomain::getTopRimParticles() {
	if (top_rim_particles.isNull()) {
		std::vector<ISphParticle> rim_particles;
		for (ISphParticle particle : particles) {
			if (particle.position.z - origin.z > dimensions.z - rim_distance) {
				rim_particles.push_back(particle);
			}
		}
		top_rim_particles.set(rim_particles);
	}
	return top_rim_particles.getInternal();
}

std::vector<ISphParticle> ParticleDomain::getBottomRimParticles() {
	if (bottom_rim_particles.isNull()) {
		std::vector<ISphParticle> rim_particles;
		for (ISphParticle particle : particles) {
			if (particle.position.z - origin.z < rim_distance) {
				rim_particles.push_back(particle);
			}
		}
		bottom_rim_particles.set(rim_particles);
	}
	return bottom_rim_particles.getInternal();
}

std::vector<ISphParticle> ParticleDomain::getFrontRimParticles() {
	if (front_rim_particles.isNull()) {
		std::vector<ISphParticle> rim_particles;
		for (ISphParticle particle : particles) {
			if (particle.position.x - origin.x > dimensions.x - rim_distance) {
				rim_particles.push_back(particle);
			}
		}
		front_rim_particles.set(rim_particles);
	}
	return front_rim_particles.getInternal();
}

std::vector<ISphParticle> ParticleDomain::getBackRimParticles() {
	if (back_rim_particles.isNull()) {
		std::vector<ISphParticle> rim_particles;
		for (ISphParticle particle : particles) {
			if (particle.position.x - origin.x < rim_distance) {
				rim_particles.push_back(particle);
			}
		}
		back_rim_particles.set(rim_particles);
	}
	return back_rim_particles.getInternal();
}

std::vector<ISphParticle> ParticleDomain::getLeftRimParticles() {
	if (left_rim_particles.isNull()) {
		std::vector<ISphParticle> rim_particles;
		for (ISphParticle particle : particles) {
			if (particle.position.y - origin.y < rim_distance) {
				rim_particles.push_back(particle);
			}
		}
		left_rim_particles.set(rim_particles);
	}
	return left_rim_particles.getInternal();
}

std::vector<ISphParticle> ParticleDomain::getRightRimParticles() {
	if (right_rim_particles.isNull()) {
		std::vector<ISphParticle> rim_particles;
		for (ISphParticle particle : particles) {
			if (particle.position.y - origin.y > dimensions.y - rim_distance) {
				rim_particles.push_back(particle);
			}
		}
		right_rim_particles.set(rim_particles);
	}
	return right_rim_particles.getInternal();
}

std::vector<ISphParticle> ParticleDomain::getFrontBottomRimParticles() {
	if (front_bottom_rim_particles.isNull()) {
		std::vector<ISphParticle> rim_particles;
		for (ISphParticle particle : getFrontRimParticles()) {
			if (particle.position.z - origin.z < rim_distance) {
				rim_particles.push_back(particle);
			}
		}
		front_bottom_rim_particles.set(rim_particles);
	}
	return front_bottom_rim_particles.getInternal();
}

std::vector<ISphParticle> ParticleDomain::getFrontTopRimParticles() {
	if (front_top_rim_particles.isNull()) {
		std::vector<ISphParticle> rim_particles;
		for (ISphParticle particle : getFrontRimParticles()) {
			if (particle.position.z - origin.z > dimensions.z - rim_distance) {
				rim_particles.push_back(particle);
			}
		}
		front_top_rim_particles.set(rim_particles);
	}
	return front_top_rim_particles.getInternal();
}

std::vector<ISphParticle> ParticleDomain::getBackBottomRimParticles() {
	if (back_bottom_rim_particles.isNull()) {
		std::vector<ISphParticle> rim_particles;
		for (ISphParticle particle : getBackRimParticles()) {
			if (particle.position.z - origin.z < rim_distance) {
				rim_particles.push_back(particle);
			}
		}
		back_bottom_rim_particles.set(rim_particles);
	}
	return back_bottom_rim_particles.getInternal();
}

std::vector<ISphParticle> ParticleDomain::getBackTopRimParticles() {
	if (back_top_rim_particles.isNull()) {
		std::vector<ISphParticle> rim_particles;
		for (ISphParticle particle : getBackRimParticles()) {
			if (particle.position.z - origin.z > dimensions.z - rim_distance) {
				rim_particles.push_back(particle);
			}
		}
		back_top_rim_particles.set(rim_particles);
	}
	return back_top_rim_particles.getInternal();
}

std::vector<ISphParticle> ParticleDomain::getFrontLeftRimParticles() {
	if (front_left_rim_particles.isNull()) {
		std::vector<ISphParticle> rim_particles;
		for (ISphParticle particle : getFrontRimParticles()) {
			if (particle.position.y - origin.y < rim_distance) {
				rim_particles.push_back(particle);
			}
		}
		front_left_rim_particles.set(rim_particles);
	}
	return front_left_rim_particles.getInternal();
}

std::vector<ISphParticle> ParticleDomain::getFrontRightRimParticles() {
	if (front_right_rim_particles.isNull()) {
		std::vector<ISphParticle> rim_particles;
		for (ISphParticle particle : getFrontRimParticles()) {
			if (particle.position.y - origin.y > dimensions.y - rim_distance) {
				rim_particles.push_back(particle);
			}
		}
		front_right_rim_particles.set(rim_particles);
	}
	return front_right_rim_particles.getInternal();
}

std::vector<ISphParticle> ParticleDomain::getBackLeftRimParticles() {
	if (back_left_rim_particles.isNull()) {
		std::vector<ISphParticle> rim_particles;
		for (ISphParticle particle : getBackRimParticles()) {
			if (particle.position.y - origin.y < rim_distance) {
				rim_particles.push_back(particle);
			}
		}
		back_left_rim_particles.set(rim_particles);
	}
	return back_left_rim_particles.getInternal();
}

std::vector<ISphParticle> ParticleDomain::getBackRightRimParticles() {
	if (back_right_rim_particles.isNull()) {
		std::vector<ISphParticle> rim_particles;
		for (ISphParticle particle : getBackRimParticles()) {
			if (particle.position.y - origin.y > dimensions.y - rim_distance) {
				rim_particles.push_back(particle);
			}
		}
		back_right_rim_particles.set(rim_particles);
	}
	return back_right_rim_particles.getInternal();
}

std::vector<ISphParticle> ParticleDomain::getLeftBottomRimParticles() {
	if (left_bottom_rim_particles.isNull()) {
		std::vector<ISphParticle> rim_particles;
		for (ISphParticle particle : getLeftRimParticles()) {
			if (particle.position.z - origin.z < rim_distance) {
				rim_particles.push_back(particle);
			}
		}
		left_bottom_rim_particles.set(rim_particles);
	}
	return left_bottom_rim_particles.getInternal();
}

std::vector<ISphParticle> ParticleDomain::getLeftTopRimParticles() {
	if (left_top_rim_particles.isNull()) {
		std::vector<ISphParticle> rim_particles;
		for (ISphParticle particle : getLeftRimParticles()) {
			if (particle.position.z - origin.z > dimensions.z - rim_distance) {
				rim_particles.push_back(particle);
			}
		}
		left_top_rim_particles.set(rim_particles);
	}
	return left_top_rim_particles.getInternal();
}
std::vector<ISphParticle> ParticleDomain::getRightBottomRimParticles() {
	if (right_bottom_rim_particles.isNull()) {
		std::vector<ISphParticle> rim_particles;
		for (ISphParticle particle : getRightRimParticles()) {
			if (particle.position.z - origin.z < rim_distance) {
				rim_particles.push_back(particle);
			}
		}
		right_bottom_rim_particles.set(rim_particles);
	}
	return right_bottom_rim_particles.getInternal();
}

std::vector<ISphParticle> ParticleDomain::getRightTopRimParticles() {
	if (right_top_rim_particles.isNull()) {
		std::vector<ISphParticle> rim_particles;
		for (ISphParticle particle : getRightRimParticles()) {
			if (particle.position.z - origin.z > dimensions.z - rim_distance) {
				rim_particles.push_back(particle);
			}
		}
		right_top_rim_particles.set(rim_particles);
	}
	return right_top_rim_particles.getInternal();
}

std::vector<ISphParticle> ParticleDomain::getFrontLeftBottomRimParticles() {
	if (front_left_bottom_rim_particles.isNull()) {
		std::vector<ISphParticle> rim_particles;
		for (ISphParticle particle : getFrontLeftRimParticles()) {
			if (particle.position.z - origin.z < rim_distance) {
				rim_particles.push_back(particle);
			}
		}
		front_left_bottom_rim_particles.set(rim_particles);
	}
	return front_left_bottom_rim_particles.getInternal();
}

std::vector<ISphParticle> ParticleDomain::getFrontRightBottomRimParticles() {
	if (front_right_bottom_rim_particles.isNull()) {
		std::vector<ISphParticle> rim_particles;
		for (ISphParticle particle : getFrontRightRimParticles()) {
			if (particle.position.z - origin.z < rim_distance) {
				rim_particles.push_back(particle);
			}
		}
		front_right_bottom_rim_particles.set(rim_particles);
	}
	return front_right_bottom_rim_particles.getInternal();
}

std::vector<ISphParticle> ParticleDomain::getFrontLeftTopRimParticles() {
	if (front_left_top_rim_particles.isNull()) {
		std::vector<ISphParticle> rim_particles;
		for (ISphParticle particle : getFrontLeftRimParticles()) {
			if (particle.position.z - origin.z > dimensions.z - rim_distance) {
				rim_particles.push_back(particle);
			}
		}
		front_left_top_rim_particles.set(rim_particles);
	}
	return front_left_top_rim_particles.getInternal();
}

std::vector<ISphParticle> ParticleDomain::getFrontRightTopRimParticles() {
	if (front_right_top_rim_particles.isNull()) {
		std::vector<ISphParticle> rim_particles;
		for (ISphParticle particle : getFrontRightRimParticles()) {
			if (particle.position.z - origin.z > dimensions.z - rim_distance) {
				rim_particles.push_back(particle);
			}
		}
		front_right_top_rim_particles.set(rim_particles);
	}
	return front_right_top_rim_particles.getInternal();
}

std::vector<ISphParticle> ParticleDomain::getBackLeftBottomRimParticles() {
	if (back_left_bottom_rim_particles.isNull()) {
		std::vector<ISphParticle> rim_particles;
		for (ISphParticle particle : getBackLeftRimParticles()) {
			if (particle.position.z - origin.z < rim_distance) {
				rim_particles.push_back(particle);
			}
		}
		back_left_bottom_rim_particles.set(rim_particles);
	}
	return back_left_bottom_rim_particles.getInternal();
}

std::vector<ISphParticle> ParticleDomain::getBackRightBottomRimParticles() {
	if (back_right_bottom_rim_particles.isNull()) {
		std::vector<ISphParticle> rim_particles;
		for (ISphParticle particle : getBackRightRimParticles()) {
			if (particle.position.z - origin.z < rim_distance) {
				rim_particles.push_back(particle);
			}
		}
		back_right_bottom_rim_particles.set(rim_particles);
	}
	return back_right_bottom_rim_particles.getInternal();
}

std::vector<ISphParticle> ParticleDomain::getBackLeftTopRimParticles() {
	if (back_left_top_rim_particles.isNull()) {
		std::vector<ISphParticle> rim_particles;
		for (ISphParticle particle : getBackLeftRimParticles()) {
			if (particle.position.z - origin.z > dimensions.z - rim_distance) {
				rim_particles.push_back(particle);
			}
		}
		back_left_top_rim_particles.set(rim_particles);
	}
	return back_left_top_rim_particles.getInternal();
}

std::vector<ISphParticle> ParticleDomain::getBackRightTopRimParticles() {
	if (back_right_top_rim_particles.isNull()) {
		std::vector<ISphParticle> rim_particles;
		for (ISphParticle particle : particles) {
			if (particle.position.z - origin.z > dimensions.z - rim_distance) {
				rim_particles.push_back(particle);
			}
		}
		back_right_top_rim_particles.set(rim_particles);
	}
	return back_right_top_rim_particles.getInternal();
}

void ParticleDomain::resetRimParticles() {
	front_rim_particles.reset();
	back_rim_particles.reset();
	left_rim_particles.reset();
	right_rim_particles.reset();
	top_rim_particles.reset();
	bottom_rim_particles.reset();
	front_bottom_rim_particles.reset();
	front_top_rim_particles.reset();
	back_bottom_rim_particles.reset();
	back_top_rim_particles.reset();
	front_left_rim_particles.reset();
	front_right_rim_particles.reset();
	back_left_rim_particles.reset();
	back_right_rim_particles.reset();
	left_bottom_rim_particles.reset();
	left_top_rim_particles.reset();
	right_bottom_rim_particles.reset();
	right_top_rim_particles.reset();
	front_left_bottom_rim_particles.reset();
	front_right_bottom_rim_particles.reset();
	front_left_top_rim_particles.reset();
	front_right_top_rim_particles.reset();
	back_left_bottom_rim_particles.reset();
	back_right_bottom_rim_particles.reset();
	back_left_top_rim_particles.reset();
	back_right_top_rim_particles.reset();
}


