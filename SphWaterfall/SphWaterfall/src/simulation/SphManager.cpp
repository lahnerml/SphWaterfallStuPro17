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
	Vector3 gravity_acceleration = Vector3(0, -9.81, 0);
	acceleration = gravity_acceleration + computeDensityAcceleration(particle) + computeViscosityAcceleration(particle);
	return acceleration;
}

void SphManager::computeLocalDensity(ISphParticle& particle) {
	double local_density = 0;

	std::vector<ISphParticle> neighbours = neighbour_search->findNeigbours(particle, domains);

	for each (ISphParticle neighbour_particle in neighbours)
	{
		local_density += neighbour_particle.mass * kernel->computeKernelValue(particle.position - neighbour_particle.position);
	}

	particle.setDensity(local_density);
}

Vector3 SphManager::computeDensityAcceleration(ISphParticle& particle) {
	std::vector<ISphParticle> neighbours = neighbour_search->findNeigbours(particle, domains);
	Vector3 density_acceleration = Vector3();
	double particle_local_pressure = computeLocalPressure(particle);

	for each (ISphParticle neighbour_particle in neighbours)
	{
		density_acceleration += neighbour_particle.mass * 
			((computeLocalPressure(neighbour_particle) / (neighbour_particle.local_density*neighbour_particle.local_density)) + (particle_local_pressure / (particle.local_density*particle.local_density))) * 
			kernel->computeKernelGradientValue(particle.position - neighbour_particle.position);
	}

	return density_acceleration;
}

double SphManager::computeLocalPressure(ISphParticle& particle) {
	double local_pressure = 0.0;
	double refrence_density_of_water = 1.0;
	//double pressure_constant = 1.0; //to be evaluated, is chosen arbitrarily at the moment
	double reference_pressure = 101300.0; //not sure atm

	//local_pressure = pressure_constant *(particle.local_density - refrence_density_of_water); //old version meight be better
	local_pressure = reference_pressure *(std::pow((particle.local_density/refrence_density_of_water), 7.0) - 1); //other implementation of local pressure

	return local_pressure;
}

Vector3 SphManager::computeViscosityAcceleration(ISphParticle& particle) {
	std::vector<ISphParticle> neighbours = neighbour_search->findNeigbours(particle, domains);
	Vector3 sum = Vector3();

	for each (ISphParticle neighbour_particle in neighbours)
	{
		Vector3 rij = neighbour_particle.position - particle.position;
		sum += neighbour_particle.mass *  ((4 * 1.0 * rij * kernel->computeKernelGradientValue(rij)) / 
			((particle.local_density + neighbour_particle.local_density) * (rij.length()*rij.length()))) * 
			(particle.velocity - neighbour_particle.velocity);
	}

	return ((1 / particle.local_density) * sum);
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

	std::vector<ISphParticle> rim_particles;

	// if you know how to do better ... do it, cause this sucks

	if (request_coords.x > target_coords.x) {
		if (request_coords.y > target_coords.y) {
			if (request_coords.z > target_coords.z) {
				rim_particles = destination_domain.getFrontRightTopRimParticles();
			}
			else if (request_coords.z < target_coords.z) {
				rim_particles = destination_domain.getFrontRightBottomRimParticles();
			}
			else {
				rim_particles = destination_domain.getFrontRightRimParticles();
			}
		}
		else if (request_coords.y < target_coords.y) {
			if (request_coords.z > target_coords.z) {
				rim_particles = destination_domain.getFrontLeftTopRimParticles();
			}
			else if (request_coords.z < target_coords.z) {
				rim_particles = destination_domain.getFrontLeftBottomRimParticles();
			}
			else {
				rim_particles = destination_domain.getFrontLeftRimParticles();
			}
		}
		else {
			if (request_coords.z > target_coords.z) {
				rim_particles = destination_domain.getFrontTopRimParticles();
			}
			else if (request_coords.z < target_coords.z) {
				rim_particles = destination_domain.getFrontBottomRimParticles();
			}
			else {
				rim_particles = destination_domain.getFrontRimParticles();
			}
		}
	}
	else if (request_coords.x < target_coords.x) {
		if (request_coords.y > target_coords.y) {
			if (request_coords.z > target_coords.z) {
				rim_particles = destination_domain.getBackRightTopRimParticles();
			}
			else if (request_coords.z < target_coords.z) {
				rim_particles = destination_domain.getBackRightBottomRimParticles();
			}
			else {
				rim_particles = destination_domain.getBackRightRimParticles();
			}
		}
		else if (request_coords.y < target_coords.y) {
			if (request_coords.z > target_coords.z) {
				rim_particles = destination_domain.getBackLeftTopRimParticles();
			}
			else if (request_coords.z < target_coords.z) {
				rim_particles = destination_domain.getBackLeftBottomRimParticles();
			}
			else {
				rim_particles = destination_domain.getBackLeftRimParticles();
			}
		}
		else {
			if (request_coords.z > target_coords.z) {
				rim_particles = destination_domain.getBackTopRimParticles();
			}
			else if (request_coords.z < target_coords.z) {
				rim_particles = destination_domain.getBackBottomRimParticles();
			}
			else {
				rim_particles = destination_domain.getBackRimParticles();
			}
		}
	} 
	else if (request_coords.y > target_coords.y) {
		if (request_coords.z > target_coords.z) {
			rim_particles = destination_domain.getRightTopRimParticles();
		}
		else if (request_coords.z < target_coords.z) {
			rim_particles = destination_domain.getRightBottomRimParticles();
		}
		else {
			rim_particles = destination_domain.getRightRimParticles();
		}
	}
	else if (request_coords.y < target_coords.y) {
		if (request_coords.z > target_coords.z) {
			rim_particles = destination_domain.getLeftTopRimParticles();
		}
		else if (request_coords.z < target_coords.z) {
			rim_particles = destination_domain.getLeftBottomRimParticles();
		}
		else {
			rim_particles = destination_domain.getLeftRimParticles();
		}
	} 
	else if (request_coords.z > target_coords.z) {
		rim_particles = destination_domain.getTopRimParticles();
	}
	else if (request_coords.z < target_coords.z) {
		rim_particles = destination_domain.getBottomRimParticles();
	}

	MPI_Request request;
	MPI_Isend(rim_particles.data(), rim_particles.size() * sizeof(ISphParticle), MPI_BYTE, requester % world_size, 0, MPI_COMM_WORLD, &request);
	MPI_Request_free(&request);
}
