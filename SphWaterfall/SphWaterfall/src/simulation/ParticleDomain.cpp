#pragma once
#include "ParticleDomain.h"

#define RIM_DISTANCE Q_MAX

ParticleDomain::ParticleDomain() :
	origin(Vector3()),
	dimensions(Vector3()),
	number_of_fluid_particles(0)
{
	particles = std::vector<SphParticle>();
}

ParticleDomain::ParticleDomain(const Vector3& origin, const Vector3& dimension) : 
	origin(origin),
	dimensions(dimension),
	number_of_fluid_particles(0)
{
	particles = std::vector<SphParticle>();
}

ParticleDomain::~ParticleDomain() {

}

int ParticleDomain::size() const {
	return particles.size();
}


void ParticleDomain::clearNeighbourRimParticles(SphParticle::ParticleType particle_type) {
	for (auto& each_rim : neighbour_rim_particles) {
		for (int i = 0; i < each_rim.second.size(); i++) {
			SphParticle each_particle = each_rim.second.at(i);
			if (each_particle.getParticleType() = particle_type) {
				each_rim.second.erase(each_rim.second.begin() + i);
				--i;
			}
		}
	}
}

std::vector<SphParticle> ParticleDomain::removeParticlesOutsideDomain() {
	std::vector<SphParticle> outside_particles;
	int domain_id = SimulationUtilities::computeDomainID(origin, dimensions);

	int number_of_particles_inside_domain = 0;
	while (number_of_particles_inside_domain < particles.size()) {
		SphParticle each_particle = particles.at(number_of_particles_inside_domain);
		if (each_particle.getParticleType() == SphParticle::FLUID && SimulationUtilities::computeDomainID(each_particle.position, dimensions) != domain_id) {
			//std::cout << "outside particle: " << each_particle << " origin: " << origin << "  dimension: " << dimensions << std::endl << "debug: " << vector_difference << std::endl; //debug
			outside_particles.push_back(each_particle);
			particles.erase(particles.begin() + number_of_particles_inside_domain);
			number_of_fluid_particles--;
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
	if (particle.getParticleType() == SphParticle::ParticleType::FLUID) {
		number_of_fluid_particles++;
	}
	particles.push_back(particle);
}

std::vector<SphParticle>& ParticleDomain::getParticles() {
	return particles;
}

bool ParticleDomain::hasFluidParticles() {
	return number_of_fluid_particles != 0;
}

std::vector<SphParticle> ParticleDomain::getTopRimParticles(SphParticle::ParticleType particle_type) {
	if (top_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : particles) {
			if (particle.getParticleType() == particle_type && particle.position.y - origin.y > dimensions.y - RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		top_rim_particles.set(rim_particles);
	}
	return top_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getBottomRimParticles(SphParticle::ParticleType particle_type) {
	if (bottom_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : particles) {
			if (particle.getParticleType() == particle_type && particle.position.y - origin.y < RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		bottom_rim_particles.set(rim_particles);
	}
	return bottom_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getFrontRimParticles(SphParticle::ParticleType particle_type) {
	if (front_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : particles) {
			if (particle.getParticleType() == particle_type && particle.position.z - origin.z < RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		front_rim_particles.set(rim_particles);
	}
	return front_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getBackRimParticles(SphParticle::ParticleType particle_type) {
	if (back_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : particles) {
			if (particle.getParticleType() == particle_type && particle.position.z - origin.z > dimensions.z - RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		back_rim_particles.set(rim_particles);
	}
	return back_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getLeftRimParticles(SphParticle::ParticleType particle_type) {
	if (left_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : particles) {
			if (particle.getParticleType() == particle_type && particle.position.x - origin.x < RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		left_rim_particles.set(rim_particles);
	}
	return left_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getRightRimParticles(SphParticle::ParticleType particle_type) {
	if (right_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : particles) {
			if (particle.getParticleType() == particle_type && particle.position.x - origin.x > dimensions.x - RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		right_rim_particles.set(rim_particles);
	}
	return right_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getFrontBottomRimParticles(SphParticle::ParticleType particle_type) {
	if (front_bottom_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getFrontRimParticles(particle_type)) {
			if (particle.getParticleType() == particle_type && particle.position.y - origin.y < RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		front_bottom_rim_particles.set(rim_particles);
	}
	return front_bottom_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getFrontTopRimParticles(SphParticle::ParticleType particle_type) {
	if (front_top_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getFrontRimParticles(particle_type)) {
			if (particle.getParticleType() == particle_type && particle.position.y - origin.y > dimensions.y - RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		front_top_rim_particles.set(rim_particles);
	}
	return front_top_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getBackBottomRimParticles(SphParticle::ParticleType particle_type) {
	if (back_bottom_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getBackRimParticles(particle_type)) {
			if (particle.getParticleType() == particle_type && particle.position.y - origin.y < RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		back_bottom_rim_particles.set(rim_particles);
	}
	return back_bottom_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getBackTopRimParticles(SphParticle::ParticleType particle_type) {
	if (back_top_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getBackRimParticles(particle_type)) {
			if (particle.getParticleType() == particle_type && particle.position.y - origin.y > dimensions.y - RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		back_top_rim_particles.set(rim_particles);
	}
	return back_top_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getFrontLeftRimParticles(SphParticle::ParticleType particle_type) {
	if (front_left_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getFrontRimParticles(particle_type)) {
			if (particle.getParticleType() == particle_type && particle.position.x - origin.x < RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		front_left_rim_particles.set(rim_particles);
	}
	return front_left_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getFrontRightRimParticles(SphParticle::ParticleType particle_type) {
	if (front_right_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getFrontRimParticles(particle_type)) {
			if (particle.getParticleType() == particle_type && particle.position.x - origin.x > dimensions.x - RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		front_right_rim_particles.set(rim_particles);
	}
	return front_right_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getBackLeftRimParticles(SphParticle::ParticleType particle_type) {
	if (back_left_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getBackRimParticles(particle_type)) {
			if (particle.getParticleType() == particle_type && particle.position.x - origin.x < RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		back_left_rim_particles.set(rim_particles);
	}
	return back_left_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getBackRightRimParticles(SphParticle::ParticleType particle_type) {
	if (back_right_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getBackRimParticles(particle_type)) {
			if (particle.getParticleType() == particle_type && particle.position.x - origin.x > dimensions.x - RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		back_right_rim_particles.set(rim_particles);
	}
	return back_right_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getLeftBottomRimParticles(SphParticle::ParticleType particle_type) {
	if (left_bottom_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getLeftRimParticles(particle_type)) {
			if (particle.getParticleType() == particle_type && particle.position.y - origin.y < RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		left_bottom_rim_particles.set(rim_particles);
	}
	return left_bottom_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getLeftTopRimParticles(SphParticle::ParticleType particle_type) {
	if (left_top_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getLeftRimParticles(particle_type)) {
			if (particle.getParticleType() == particle_type && particle.position.y - origin.y > dimensions.y - RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		left_top_rim_particles.set(rim_particles);
	}
	return left_top_rim_particles.getInternal();
}
std::vector<SphParticle> ParticleDomain::getRightBottomRimParticles(SphParticle::ParticleType particle_type) {
	if (right_bottom_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getRightRimParticles(particle_type)) {
			if (particle.getParticleType() == particle_type && particle.position.y - origin.y < RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		right_bottom_rim_particles.set(rim_particles);
	}
	return right_bottom_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getRightTopRimParticles(SphParticle::ParticleType particle_type) {
	if (right_top_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getRightRimParticles(particle_type)) {
			if (particle.getParticleType() == particle_type && particle.position.y - origin.y > dimensions.y - RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		right_top_rim_particles.set(rim_particles);
	}
	return right_top_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getFrontLeftBottomRimParticles(SphParticle::ParticleType particle_type) {
	if (front_left_bottom_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getFrontLeftRimParticles(particle_type)) {
			if (particle.getParticleType() == particle_type && particle.position.y - origin.y < RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		front_left_bottom_rim_particles.set(rim_particles);
	}
	return front_left_bottom_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getFrontRightBottomRimParticles(SphParticle::ParticleType particle_type) {
	if (front_right_bottom_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getFrontRightRimParticles(particle_type)) {
			if (particle.getParticleType() == particle_type && particle.position.y - origin.y < RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		front_right_bottom_rim_particles.set(rim_particles);
	}
	return front_right_bottom_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getFrontLeftTopRimParticles(SphParticle::ParticleType particle_type) {
	if (front_left_top_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getFrontLeftRimParticles(particle_type)) {
			if (particle.getParticleType() == particle_type && particle.position.y - origin.y > dimensions.y - RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		front_left_top_rim_particles.set(rim_particles);
	}
	return front_left_top_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getFrontRightTopRimParticles(SphParticle::ParticleType particle_type) {
	if (front_right_top_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getFrontRightRimParticles(particle_type)) {
			if (particle.getParticleType() == particle_type && particle.position.y - origin.y > dimensions.y - RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		front_right_top_rim_particles.set(rim_particles);
	}
	return front_right_top_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getBackLeftBottomRimParticles(SphParticle::ParticleType particle_type) {
	if (back_left_bottom_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getBackLeftRimParticles(particle_type)) {
			if (particle.getParticleType() == particle_type && particle.position.y - origin.y < RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		back_left_bottom_rim_particles.set(rim_particles);
	}
	return back_left_bottom_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getBackRightBottomRimParticles(SphParticle::ParticleType particle_type) {
	if (back_right_bottom_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getBackRightRimParticles(particle_type)) {
			if (particle.getParticleType() == particle_type && particle.position.y - origin.y < RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		back_right_bottom_rim_particles.set(rim_particles);
	}
	return back_right_bottom_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getBackLeftTopRimParticles(SphParticle::ParticleType particle_type) {
	if (back_left_top_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getBackLeftRimParticles(particle_type)) {
			if (particle.getParticleType() == particle_type && particle.position.y - origin.y > dimensions.y - RIM_DISTANCE) {
				rim_particles.push_back(particle);
			}
		}
		back_left_top_rim_particles.set(rim_particles);
	}
	return back_left_top_rim_particles.getInternal();
}

std::vector<SphParticle> ParticleDomain::getBackRightTopRimParticles(SphParticle::ParticleType particle_type) {
	if (back_right_top_rim_particles.isNull()) {
		std::vector<SphParticle> rim_particles;
		for (SphParticle particle : getBackRightRimParticles(particle_type)) {
			if (particle.getParticleType() == particle_type && particle.position.y - origin.y > dimensions.y - RIM_DISTANCE) {
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
