#include "ParticleDomain.h"

ParticleDomain::ParticleDomain() :
	origin(Vector3()),
	dimensions(Vector3())
{
}

ParticleDomain::ParticleDomain(const Vector3& origin, const Vector3& dimension) :
	origin(origin),
	dimensions(dimension)
{
}

ParticleDomain::~ParticleDomain() {

}

const Vector3& ParticleDomain::getDimensions() const {
	return dimensions;
}

const Vector3& ParticleDomain::getOrigin() const {
	return origin;
}

void ParticleDomain::addParticle(const SphParticle& particle) {
	particles[particle.getParticleType()].push_back(particle);
}

std::vector<SphParticle>& ParticleDomain::getFluidParticles() {
	return particles[SphParticle::FLUID];
}

std::vector<SphParticle>& ParticleDomain::getStaticParticles() {
	return particles[SphParticle::STATIC];
}

std::vector<SphParticle*> ParticleDomain::getParticles() {
	std::vector<SphParticle*> all_particles;
	for (auto& each_particles : particles) {
		for (auto& each_particle : each_particles.second) {
			all_particles.push_back(&each_particle);
		}
	}
	return all_particles;
}

void ParticleDomain::clearParticles() {
	for (auto& each_particles : particles) {
		each_particles.second.clear();
	}
}

void ParticleDomain::clearParticles(SphParticle::ParticleType particle_type) {
	particles[particle_type].clear();
}

const bool ParticleDomain::hasParticles(SphParticle::ParticleType particle_type) {
	return !particles[particle_type].empty();
}

std::vector<SphParticle> ParticleDomain::removeParticlesOutsideDomain() {
	std::vector<SphParticle> outside_particles;
	int domain_id = SimulationUtilities::computeDomainID(origin, dimensions);

	int particle_index = 0;
	while (particle_index < getFluidParticles().size()) {
		SphParticle each_particle = getFluidParticles().at(particle_index);
		if (SimulationUtilities::computeDomainID(each_particle.position, dimensions) != domain_id) {
			//Find particles outside domain
			outside_particles.push_back(each_particle);
			getFluidParticles().erase(getFluidParticles().begin() + particle_index);
		}
		else {
			particle_index++;
		}
	}

	return outside_particles;
}

void ParticleDomain::clearNeighbourRimParticles() {
	neighbour_particles.clear();
}

void ParticleDomain::clearNeighbourRimParticles(SphParticle::ParticleType particle_type) {
	neighbour_particles[particle_type].clear();
}

void ParticleDomain::addNeighbourRimParticles(const std::unordered_map<int, std::vector<SphParticle*>>& neighbour_rim_map, SphParticle::ParticleType particle_type) {
	for (auto& each_neighbour_particles : neighbour_rim_map) {
		neighbour_particles[particle_type][each_neighbour_particles.first].insert(neighbour_particles[particle_type][each_neighbour_particles.first].end(),
			each_neighbour_particles.second.begin(), each_neighbour_particles.second.end());
	}
}

std::unordered_map<int, std::vector<SphParticle*>> ParticleDomain::getNeighbourRimParticles() {
	std::unordered_map<int, std::vector<SphParticle*>> result;
	for (auto& each_neighbour_list : neighbour_particles) {
		for (auto& each_neighbour : each_neighbour_list.second) {
			for (auto& each_particle : each_neighbour.second) {
				result[each_neighbour.first].push_back(each_particle);
			}
		}
	}
	return result;
}

std::unordered_map<int, std::vector<SphParticle*>> ParticleDomain::getRimParticleTargetMap(SphParticle::ParticleType particle_type) {
	std::unordered_map<int, std::vector<SphParticle*>> target_map;
	int domain_id = SimulationUtilities::computeDomainID(origin, dimensions);
	for (SphParticle& particle : particles[particle_type]) {
		for (int x = -1; x <= 1; x++) {
			for (int y = -1; y <= 1; y++) {
				for (int z = -1; z <= 1; z++) {
					int id = SimulationUtilities::computeDomainID(particle.position + ((Vector3(x, y, z).normalize() * Q_MAX)), dimensions);
					if (id != domain_id) {
						target_map[id].push_back(&particle);
					}
				}
			}
		}
	}
	return target_map;
}
