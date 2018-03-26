#pragma once
#include "ParticleDomain.h"

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
			SphParticle& each_particle = each_rim.second.at(i);
			if (each_particle.getParticleType() == particle_type) {
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

void ParticleDomain::addNeighbourRimParticles(const std::unordered_map<int, std::vector<SphParticle>> neighbour_rim_map) {
	for (auto& each_neighbour_particles : neighbour_rim_map) {
		neighbour_rim_particles[each_neighbour_particles.first].insert(neighbour_rim_particles[each_neighbour_particles.first].end(), each_neighbour_particles.second.begin(), each_neighbour_particles.second.end());
	}
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

std::unordered_map<int, std::vector<SphParticle>> ParticleDomain::getRimParticleTargetMap(SphParticle::ParticleType particle_type) {
	std::unordered_map<int, std::vector<SphParticle>> target_map;
	int domain_id = SimulationUtilities::computeDomainID(origin, dimensions);
	for (SphParticle& particle : particles) {
		if (particle.getParticleType() == particle_type) {
			for (int x = -1; x <= 1; x++) {
				for (int y = -1; y <= 1; y++) {
					for (int z = -1; z <= 1; z++) {
						int id = SimulationUtilities::computeDomainID(particle.position + ((Vector3(x, y, z).normalize() * R_MAX)), dimensions);
						if (id != domain_id) {
							target_map[id].push_back(particle);
						}
					}
				}
			}
		}
	}

	return target_map;
}
