#pragma once
#include "SphManager.h"

SphManager::SphManager(const Vector3& domain_dimensions) :
	domain_dimensions(domain_dimensions)
{
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	kernel = kernel_factory.getInstance(1);
	neighbour_search = neighbour_search_factory.getInstance(1);
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

void SphManager::clearAccelerations() {
	for (auto each_domain: domains) {
		for (auto each_particle: each_domain.second.getParticles()) {
			//each_particle.velocity = Vector3(0,0,0);
		}
	}
}

void SphManager::updateVelocity(ISphParticle& particle) {
	Vector3 current_accelleration = computeAcceleration(particle);

}

Vector3 SphManager::computeAcceleration(ISphParticle& particle) {
	Vector3 acceleration;
	Vector3 gravityAcceleration = Vector3(0, -9.81, 0);
	acceleration = gravityAcceleration + computeDensityAcceleration(particle) + computeViscosityAcceleration(particle);
	return acceleration;
}

void SphManager::computeDensity(ISphParticle& particle) {
	double density;
	density = 0;

	std::vector<ISphParticle> neighbours = neighbour_search->findNeigbours(particle, domains);

	for each (ISphParticle p in neighbours)
	{
		density += p.mass * kernel->computeKernelValue(particle.position - p.position);
	}

	particle.setDensity(density);
}

Vector3 SphManager::computeDensityAcceleration(ISphParticle& particle) {
	Vector3 densityAcceleration;

	std::vector<ISphParticle> neighbours = neighbour_search->findNeigbours(particle, domains);

	for each (ISphParticle p in neighbours)
	{
		densityAcceleration = (p.mass * ((computePressure(p) / (p.density*p.density)) + (computePressure(particle) / (particle.density*particle.density)))
			* kernel->computeKernelGradientValue(particle.position - p.position));
	}

	return densityAcceleration;
}

double SphManager::computePressure(ISphParticle& particle) {
	double pressure = 0.0;
	double refrenceDensityOfWater = 1.0;
	double pressureConstant = 1.0; //to be evaluated, is chosen arbitrarily at the moment

	pressure = pressureConstant *(particle.density - refrenceDensityOfWater);

	return pressure;
}


void SphManager::computeViscosity(ISphParticle& particle) {
	double viscosity;
	viscosity = 0; // to be implemented
	particle.setViscosity(viscosity);
}

Vector3 SphManager::computeViscosityAcceleration(ISphParticle& particle) {

	std::vector<ISphParticle> neighbours = neighbour_search->findNeigbours(particle, domains);

	double mu = 1.0;
	Vector3 sum = Vector3();

	for each (ISphParticle p in neighbours)
	{
		Vector3 rij = p.position - particle.position;
		double value = rij.length();
		sum += p.mass *  (((4 * mu*rij) * kernel->computeKernelGradientValue(rij)) / ((particle.density + p.density) * (value*value)) * (particle.velocity - p.velocity));
	}
	Vector3 viscosityAcceleration = ((1 / particle.density) * sum);

	return viscosityAcceleration;
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

Vector3 SphManager::unhash(const int& unique_id) const {
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
		domains.at(unique_id) = ParticleDomain(unhash(unique_id) * domain_dimensions, domain_dimensions);
	}

	return domains.at(unique_id);
}

void SphManager::add_particles(const std::vector<ISphParticle>& new_particles) {
	for (ISphParticle particle : new_particles) {
		int domain_id = computeTargetDomain(particle);
		getParticleDomain(domain_id);
	}
}

MPI_Request SphManager::requestRimParticles(const Vector3& neighbourDomain, const Vector3& source) {
	int domain_id = hash(neighbourDomain);
	int request_id = hash(source);

	MPI_Request request;
	MPI_Isend(&request_id, 1, MPI_INT, domain_id, request_id % world_size, MPI_COMM_WORLD, &request);
	return request;
}

void SphManager::sendRimParticles(const int& destination, const int& requester) {
	Vector3 request_coords = unhash(requester);
	Vector3 target_coords = unhash(destination);
	ParticleDomain destination_domain = getParticleDomain(destination);


	std::vector<std::vector<ISphParticle>> set;


	std::vector<ISphParticle> rim_particles;
	if (request_coords.x > target_coords.x) {
		rim_particles = destination_domain.getFrontRimParticles();
	}
	else if (request_coords.x < target_coords.x) {
		rim_particles = destination_domain.getBackRimParticles();
	}

	if (request_coords.y > target_coords.y) {
		rim_particles = destination_domain.getRightRimParticles();
	}
	else if (request_coords.y < target_coords.y) {
		rim_particles = destination_domain.getLeftRimParticles();
	}

	if (request_coords.z > target_coords.z) {
		rim_particles = destination_domain.getTopRimParticles();
	}
	else if (request_coords.z < target_coords.z) {
		rim_particles = destination_domain.getBottomRimParticles();
	}

	MPI_Request request;
	MPI_Isend(rim_particles.data(), rim_particles.size() * sizeof(ISphParticle), MPI_BYTE, requester & world_size, 0, MPI_COMM_WORLD, &request);
	MPI_Request_free(&request);
}
