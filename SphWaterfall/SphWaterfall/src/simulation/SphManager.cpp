#pragma once
#include "SphManager.h"
#include <vector>
#include <unordered_map>
#include <iterator>

SphManager::SphManager(const Vector3& domain_dimensions) :
	domain_dimensions(domain_dimensions)
{
	MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
}

SphManager::~SphManager() {

}

void SphManager::update(double) {

}

void SphManager::clearAccellerations() {

}

void SphManager::updateVelocity(ISphParticle) {

}

void SphManager::computeAccelleration(ISphParticle) {

}

void SphManager::computeDenity(ISphParticle) {

}

void SphManager::computeViscosity(ISphParticle) {

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

	for (auto vector : target_map) {
		MPI_Request request;
		MPI_Isend(vector.second.data(), vector.second.size() * sizeof(ISphParticle), MPI_BYTE, vector.first, 0, MPI_COMM_WORLD, &request);
		MPI_Request_free(&request);
	}

	MPI_Barrier(MPI_COMM_WORLD);

	int flag;
	MPI_Status status;
	MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);

	std::vector<ISphParticle> all_new_particles;
	std::vector<ISphParticle> incoming_particles;

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
}

int SphManager::computeTargetDomain(const ISphParticle& particle) {
	Vector3 targetDomainCoords = particle.position % domain_dimensions;
	int x, y, z;
	x = static_cast<int>(targetDomainCoords.x);
	y = static_cast<int>(targetDomainCoords.y);
	z = static_cast<int>(targetDomainCoords.z);
	y = y << 10;
	z = z << 20;
	return (x + y + z);
}

int SphManager::computeTargetProcess(const ISphParticle& particle) {
	return computeTargetDomain(particle) % worldSize;
}

ParticleDomain& SphManager::getParticleDomain(const int& unique_id) {
	int count = domains.count(unique_id);

	if (count == 0) {
		int z = unique_id >> 20;
		int y = (unique_id - (z << 20)) >> 10;
		int x = unique_id - (z << 20) - (y << 10);
		domains.at(unique_id) = ParticleDomain(Vector3(x, y, z), domain_dimensions);
	}

	return domains.at(unique_id);
}

void SphManager::add_particles(const std::vector<ISphParticle>& new_particles) {
	for (ISphParticle particle : new_particles) {
		int domain_id = computeTargetDomain(particle);
		getParticleDomain(domain_id);
	}
}
