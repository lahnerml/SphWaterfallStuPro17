#include "ParticleDomain.h"

ParticleDomain::ParticleDomain() :
	origin(Vector3()),
	dimensions(Vector3())
{
	fluid_particles = std::vector<SphParticle>();
	static_particles = std::vector<SphParticle>();
}

ParticleDomain::ParticleDomain(const Vector3& origin, const Vector3& dimension) :
	origin(origin),
	dimensions(dimension)
{
	fluid_particles = std::vector<SphParticle>();
	static_particles = std::vector<SphParticle>();
}

ParticleDomain::~ParticleDomain() {

}

int ParticleDomain::size() const {
	return fluid_particles.size() + static_particles.size();
}

const Vector3& ParticleDomain::getDimensions() const {
	return dimensions;
}

const Vector3& ParticleDomain::getOrigin() const {
	return origin;
}

void ParticleDomain::addParticle(const SphParticle& particle) {
	if (particle.getParticleType() == SphParticle::FLUID) {
		fluid_particles.push_back(particle);
	}
	else if (particle.getParticleType() == SphParticle::STATIC) {
		static_particles.push_back(particle);
	}
}

std::vector<SphParticle>& ParticleDomain::getFluidParticles() {
	return fluid_particles;
}

std::vector<SphParticle>& ParticleDomain::getStaticParticles() {
	return static_particles;
}

std::vector<SphParticle> ParticleDomain::getParticles() {
	std::vector<SphParticle> all_particles = fluid_particles;
	all_particles.insert(all_particles.end(), static_particles.begin(), static_particles.end());
	return all_particles;
}

void ParticleDomain::clearParticles() {
	fluid_particles.clear();
	static_particles.clear();
}

void ParticleDomain::clearParticles(SphParticle::ParticleType particle_type) {
	if (particle_type == SphParticle::STATIC) {
		fluid_particles.clear();
	}
	else if (particle_type == SphParticle::FLUID) {
		static_particles.clear();
	}
}

const bool ParticleDomain::hasFluidParticles() const {
	return !fluid_particles.empty();
}

const bool ParticleDomain::hasStaticParticles() const {
	return !static_particles.empty();
}

std::vector<SphParticle> ParticleDomain::removeParticlesOutsideDomain() {
	std::vector<SphParticle> outside_particles;
	int domain_id = SimulationUtilities::computeDomainID(origin, dimensions);

	int particle_index = 0;
	while (particle_index < fluid_particles.size()) {
		SphParticle each_particle = fluid_particles.at(particle_index);
		if (SimulationUtilities::computeDomainID(each_particle.position, dimensions) != domain_id) {
			//Find particles outside domain
			outside_particles.push_back(each_particle);
			fluid_particles.erase(fluid_particles.begin() + particle_index);
		}
		else {
			particle_index++;
		}
	}

	return outside_particles;
}

void ParticleDomain::clearNeighbourRimParticles() {
	neighbour_fluid_particles.clear();
	neighbour_static_particles.clear();
}

void ParticleDomain::clearNeighbourRimParticles(SphParticle::ParticleType particle_type) {
	if (particle_type == SphParticle::FLUID) {
		neighbour_fluid_particles.clear();
	}
	else if (particle_type == SphParticle::STATIC) {
		neighbour_static_particles.clear();
	}
}

void ParticleDomain::addNeighbourRimParticles(const std::unordered_map<int, std::vector<SphParticle>>& neighbour_rim_map, SphParticle::ParticleType particle_type) {
	if (particle_type == SphParticle::FLUID) {
		for (auto& each_neighbour_particles : neighbour_fluid_particles) {
			neighbour_fluid_particles[each_neighbour_particles.first].insert(neighbour_fluid_particles[each_neighbour_particles.first].end(),
				each_neighbour_particles.second.begin(), each_neighbour_particles.second.end());
		}
	}
	else if (particle_type == SphParticle::STATIC) {
		for (auto& each_neighbour_particles : neighbour_static_particles) {
			neighbour_static_particles[each_neighbour_particles.first].insert(neighbour_static_particles[each_neighbour_particles.first].end(),
				each_neighbour_particles.second.begin(), each_neighbour_particles.second.end());
		}
	}
}

std::unordered_map<int, std::vector<SphParticle>>& ParticleDomain::getFluidNeighbourRimParticles() {
	return neighbour_fluid_particles;
}

std::unordered_map<int, std::vector<SphParticle>>& ParticleDomain::getStaticNeighbourRimParticles() {
	return neighbour_static_particles;
}

std::unordered_map<int, std::vector<SphParticle>> ParticleDomain::getRimParticleTargetMap(SphParticle::ParticleType particle_type) {
	std::unordered_map<int, std::vector<SphParticle>> target_map;
	int domain_id = SimulationUtilities::computeDomainID(origin, dimensions);
	if (particle_type == SphParticle::FLUID) {
		for (SphParticle& particle : fluid_particles) {
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
	else if (particle_type == SphParticle::STATIC) {
		for (SphParticle& particle : static_particles) {
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
