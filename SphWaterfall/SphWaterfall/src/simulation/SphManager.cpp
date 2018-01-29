#pragma once
#include "SphManager.h"

SphManager::SphManager(const Vector3& domain_dimensions) :
	domain_dimensions(domain_dimensions)
{
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
}

SphManager::~SphManager() {

}

void SphManager::update(double) {
	for (auto each_domain : domains) {
		for (auto each_particle : each_domain.second.getParticles()) {
			updateVelocity(each_particle);
		}
	}
}

void SphManager::clearAccellerations() {
	for (auto each_domain: domains) {
		for (auto each_particle: each_domain.second.getParticles()) {
			//each_particle.velocity = Vector3(0,0,0);
		}
	}
}

void SphManager::updateVelocity(ISphParticle& particle) {

	computeDensity(particle);
	computeViscosity(particle);
	computeAccelleration(particle);
}

void SphManager::computeAccelleration(ISphParticle& particle) {

	
}

void SphManager::computeDensity(ISphParticle& particle) {
	double density;
	density = 0; // to be implemented
	particle.setDensity(density);
}

void SphManager::computeViscosity(ISphParticle& particle) {
	double viscosity;
	viscosity = 0; // to be implemented
	particle.setViscosity(viscosity);
}

void SphManager::findNeighbourDomains(ParticleDomain) {

}

void SphManager::exchangeParticles() {
	std::unordered_map<int, std::vector<ISphParticle>> target_map;

	for (auto each_domain : domains) {
		std::vector<ISphParticle> outside_particles = each_domain.second.removeParticlesOutsideDomain();
		for (ISphParticle each_particle : outside_particles) {
			int target_id = computeTargetProcess(each_particle);
			int count = target_map.count(target_id);

			if (count == 0) {
				target_map.at(target_id) = std::vector<ISphParticle>();
			}

			std::vector<ISphParticle> list = target_map.at(target_id);
			list.push_back(each_particle);
		}
	}

	std::vector<ISphParticle> all_new_particles;
	std::vector<ISphParticle> incoming_particles;

	// don't send to yourself
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int count = target_map.count(rank);
	if (count != 0) {
		all_new_particles = target_map.at(rank);
		target_map.erase(rank);
	}

	for (auto vector : target_map) {
		MPI_Request request;
		MPI_Isend(vector.second.data(), vector.second.size() * sizeof(ISphParticle), MPI_BYTE, vector.first, 0, MPI_COMM_WORLD, &request);
		MPI_Request_free(&request);
	}

	MPI_Barrier(MPI_COMM_WORLD);

	// receive until there is nothing left
	int flag;
	MPI_Status status;
	MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);

	while (flag) {
		int source = status.MPI_SOURCE;
		int tag = status.MPI_TAG;
		int count;

		MPI_Get_count(&status, MPI_BYTE, &count);

		MPI_Recv(incoming_particles.data(), count, MPI_BYTE, source, tag, MPI_COMM_WORLD, &status);
		std::move(incoming_particles.begin(), incoming_particles.end(), std::inserter(all_new_particles, all_new_particles.end()));

		// next message
		MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
	}

	add_particles(all_new_particles);

	MPI_Barrier(MPI_COMM_WORLD);
}

int SphManager::hash(const Vector3& vector) const {
	int x, y, z;
	x = static_cast<int>(vector.x);
	y = static_cast<int>(vector.y);
	z = static_cast<int>(vector.z);
	y = y << 10;
	z = z << 20;
	return (x + y + z);
}

Vector3& SphManager::unhash(const int& unique_id) const {
	int z = unique_id >> 20;
	int y = (unique_id - (z << 20)) >> 10;
	int x = unique_id - (z << 20) - (y << 10);
	return Vector3(x, y, z);
}

int SphManager::computeTargetDomain(const ISphParticle& particle) const{
	Vector3 targetDomainCoords = particle.position % domain_dimensions;
	return hash(targetDomainCoords);
}

int SphManager::computeTargetProcess(const ISphParticle& particle) const {
	return computeTargetDomain(particle) % world_size;
}

ParticleDomain& SphManager::getParticleDomain(const int& unique_id) {
	int count = domains.count(unique_id);

	if (count == 0) {
		domains.at(unique_id) = ParticleDomain(unhash(unique_id), domain_dimensions);
	}

	return domains.at(unique_id);
}

void SphManager::add_particles(const std::vector<ISphParticle>& new_particles) {
	for (ISphParticle particle : new_particles) {
		int domain_id = computeTargetDomain(particle);
		getParticleDomain(domain_id);
	}
}
