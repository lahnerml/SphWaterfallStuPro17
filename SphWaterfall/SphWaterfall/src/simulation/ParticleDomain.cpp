#pragma once
#include "ParticleDomain.h"

#define RIM_DISTANCE Q_MAX

ParticleDomain::ParticleDomain() :
	origin(Vector3()),
	dimensions(Vector3())
{
	particles = std::vector<SphParticle>();
}

ParticleDomain::ParticleDomain(const Vector3& origin, const Vector3& dimension) : 
	origin(origin),
	dimensions(dimension)
{
	particles = std::vector<SphParticle>();
}

ParticleDomain::~ParticleDomain() {

}

int ParticleDomain::size() const {
	return particles.size();
}


void ParticleDomain::clearRimParticles() {
	neighbour_rim_particles = std::unordered_map<int, std::vector<SphParticle>>();
}

std::vector<SphParticle> ParticleDomain::removeParticlesOutsideDomain() {
	std::vector<SphParticle> outside_particles;
	int domain_id = SimulationUtilities::computeDomainID(origin, dimensions);

	int number_of_particles_inside_domain = 0;
	while (number_of_particles_inside_domain < particles.size()) {
		SphParticle each_particle = particles.at(number_of_particles_inside_domain);
		Vector3 vector_difference = each_particle.position - origin;
		if (each_particle.getParticleType() == SphParticle::FLUID && SimulationUtilities::computeDomainID(each_particle.position, dimensions) != domain_id) {
			//std::cout << "outside particle: " << each_particle << " origin: " << origin << "  dimension: " << dimensions << std::endl << "debug: " << vector_difference << std::endl; //debug
			outside_particles.push_back(each_particle);
			particles.erase(particles.begin() + number_of_particles_inside_domain);
		}
		else {
			number_of_particles_inside_domain++;
		}
	}

	return outside_particles;
}

void ParticleDomain::setNeighbourRimParticles(const std::unordered_map<int, std::vector<SphParticle>> neighbour_rim_map) {
	neighbour_rim_particles = neighbour_rim_map;
}

std::unordered_map<int, std::vector<SphParticle>>& ParticleDomain::getNeighbourRimParticles() {
	return neighbour_rim_particles;
}

const Vector3& ParticleDomain::getDimensions() const {
	return dimensions;
}

const Vector3& ParticleDomain::getOrigin() const {
	return origin;
}

void ParticleDomain::addParticle(const SphParticle& particle) {
	particles.push_back(particle);
}

std::vector<SphParticle>& ParticleDomain::getParticles() {
	return particles;
}

std::vector<SphParticle> ParticleDomain::getTopRimParticles() {
	if (top_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : particles) {
			if (particle.position.y - origin.y > dimensions.y - RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		top_rim_particles.set(rim_particles);
	}
	return top_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getBottomRimParticles() {
	if (bottom_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : particles) {
			if (particle.position.y - origin.y < RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		bottom_rim_particles.set(rim_particles);
	}
	return bottom_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getFrontRimParticles() {
	if (front_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : particles) {
			if (particle.position.z - origin.z < RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		front_rim_particles.set(rim_particles);
	}
	return front_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getBackRimParticles() {
	if (back_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : particles) {
			if (particle.position.z - origin.z > dimensions.z - RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		back_rim_particles.set(rim_particles);
	}
	return back_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getLeftRimParticles() {
	if (left_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : particles) {
			if (particle.position.x - origin.x < RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		left_rim_particles.set(rim_particles);
	}
	return left_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getRightRimParticles() {
	if (right_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : particles) {
			if (particle.position.x - origin.x > dimensions.x - RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		right_rim_particles.set(rim_particles);
	}
	return right_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getFrontBottomRimParticles() {
	if (front_bottom_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getFrontRimParticles()) {
			if (particle.position.y - origin.y < RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		front_bottom_rim_particles.set(rim_particles);
	}
	return front_bottom_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getFrontTopRimParticles() {
	if (front_top_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getFrontRimParticles()) {
			if (particle.position.y - origin.y > dimensions.y - RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		front_top_rim_particles.set(rim_particles);
	}
	return front_top_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getBackBottomRimParticles() {
	if (back_bottom_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getBackRimParticles()) {
			if (particle.position.y - origin.y < RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		back_bottom_rim_particles.set(rim_particles);
	}
	return back_bottom_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getBackTopRimParticles() {
	if (back_top_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getBackRimParticles()) {
			if (particle.position.y - origin.y > dimensions.y - RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		back_top_rim_particles.set(rim_particles);
	}
	return back_top_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getFrontLeftRimParticles() {
	if (front_left_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getFrontRimParticles()) {
			if (particle.position.x - origin.x < RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		front_left_rim_particles.set(rim_particles);
	}
	return front_left_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getFrontRightRimParticles() {
	if (front_right_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getFrontRimParticles()) {
			if (particle.position.x - origin.x > dimensions.x - RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		front_right_rim_particles.set(rim_particles);
	}
	return front_right_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getBackLeftRimParticles() {
	if (back_left_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getBackRimParticles()) {
			if (particle.position.x - origin.x < RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		back_left_rim_particles.set(rim_particles);
	}
	return back_left_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getBackRightRimParticles() {
	if (back_right_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getBackRimParticles()) {
			if (particle.position.x - origin.x > dimensions.x - RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		back_right_rim_particles.set(rim_particles);
	}
	return back_right_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getLeftBottomRimParticles() {
	if (left_bottom_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getLeftRimParticles()) {
			if (particle.position.y - origin.y < RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		left_bottom_rim_particles.set(rim_particles);
	}
	return left_bottom_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getLeftTopRimParticles() {
	if (left_top_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getLeftRimParticles()) {
			if (particle.position.y - origin.y > dimensions.y - RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		left_top_rim_particles.set(rim_particles);
	}
	return left_top_rim_particles.getInternal();
}
std::vector<SphParticle> ParticleDomain::getRightBottomRimParticles() {
	if (right_bottom_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getRightRimParticles()) {
			if (particle.position.y - origin.y < RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		right_bottom_rim_particles.set(rim_particles);
	}
	return right_bottom_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getRightTopRimParticles() {
	if (right_top_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getRightRimParticles()) {
			if (particle.position.y - origin.y > dimensions.y - RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		right_top_rim_particles.set(rim_particles);
	}
	return right_top_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getFrontLeftBottomRimParticles() {
	if (front_left_bottom_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getFrontLeftRimParticles()) {
			if (particle.position.y - origin.y < RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		front_left_bottom_rim_particles.set(rim_particles);
	}
	return front_left_bottom_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getFrontRightBottomRimParticles() {
	if (front_right_bottom_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getFrontRightRimParticles()) {
			if (particle.position.y - origin.y < RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		front_right_bottom_rim_particles.set(rim_particles);
	}
	return front_right_bottom_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getFrontLeftTopRimParticles() {
	if (front_left_top_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getFrontLeftRimParticles()) {
			if (particle.position.y - origin.y > dimensions.y - RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		front_left_top_rim_particles.set(rim_particles);
	}
	return front_left_top_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getFrontRightTopRimParticles() {
	if (front_right_top_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getFrontRightRimParticles()) {
			if (particle.position.y - origin.y > dimensions.y - RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		front_right_top_rim_particles.set(rim_particles);
	}
	return front_right_top_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getBackLeftBottomRimParticles() {
	if (back_left_bottom_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getBackLeftRimParticles()) {
			if (particle.position.y - origin.y < RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		back_left_bottom_rim_particles.set(rim_particles);
	}
	return back_left_bottom_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getBackRightBottomRimParticles() {
	if (back_right_bottom_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getBackRightRimParticles()) {
			if (particle.position.y - origin.y < RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		back_right_bottom_rim_particles.set(rim_particles);
	}
	return back_right_bottom_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getBackLeftTopRimParticles() {
	if (back_left_top_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getBackLeftRimParticles()) {
			if (particle.position.y - origin.y > dimensions.y - RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		back_left_top_rim_particles.set(rim_particles);
	}
	return back_left_top_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getBackRightTopRimParticles() {
	if (back_right_top_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : particles) {
			if (particle.position.y - origin.y > dimensions.y - RIM_DISTANCE) {
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
