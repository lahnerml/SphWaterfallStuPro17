#pragma once
#include "SphManager.h"

#define H 1.0
#define Q_MAX 2.0

SphManager::SphManager(const Vector3& domain_dimensions, double simulation_time, double timestep_duration) :
	domain_dimensions(domain_dimensions),
	simulation_time(simulation_time),
	timestep_duration(timestep_duration)
{
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	kernel = kernel_factory.getInstance(1, H, Q_MAX);
	neighbour_search = neighbour_search_factory.getInstance(1);
}

SphManager::~SphManager() {

}

void SphManager::simulate() {
	double remaining_simulation_time = simulation_time;
	while (remaining_simulation_time > 0) {
		update(timestep_duration);
		remaining_simulation_time -= timestep_duration;
	}
}

void SphManager::update(double timestep_duration) {
	// compute and set local densities
	for (auto& each_domain : domains) {
		for (auto& each_particle : each_domain.second.getParticles()) {
			computeLocalDensity(each_particle);
		}
	}
	// compute and update Velocities and position
	for (auto& each_domain : domains) {
		for (auto& each_particle : each_domain.second.getParticles()) {
			std::cout << each_particle.position << std::endl;
			updateVelocity(each_particle, timestep_duration);
		}
	}
}

void SphManager::updateVelocity(SphParticle& particle, double timestep_duration) {
	Vector3 accelleration_timestep_start = computeAcceleration(particle);

	particle.velocity = particle.velocity + ((timestep_duration / 2) * accelleration_timestep_start);
	Vector3 position_timestep_half = particle.position + ((timestep_duration / 2) * particle.velocity);

	Vector3 accelleration_timestep_half = computeAcceleration(particle);

	Vector3 velocity_timestep_end = particle.velocity + (timestep_duration * accelleration_timestep_half);
	particle.position = position_timestep_half + ((timestep_duration / 2) * velocity_timestep_end);
}

Vector3 SphManager::computeAcceleration(SphParticle& particle) {
	Vector3 acceleration;
	Vector3 gravity_acceleration = Vector3(0, -9.81, 0);
	acceleration = gravity_acceleration + computeDensityAcceleration(particle) + computeViscosityAcceleration(particle);
	return acceleration;
}

void SphManager::computeLocalDensity(SphParticle& particle) {
	double local_density = 0;

	std::vector<SphParticle> neighbours = neighbour_search->findNeigbours(particle, domains);

	for (SphParticle neighbour_particle : neighbours) {
		local_density += neighbour_particle.mass * kernel->computeKernelValue(particle.position - neighbour_particle.position);
	}

	particle.local_density = local_density;
}

Vector3 SphManager::computeDensityAcceleration(SphParticle& particle) {
	std::vector<SphParticle> neighbours = neighbour_search->findNeigbours(particle, domains);
	Vector3 density_acceleration = Vector3();
	double particle_local_pressure = computeLocalPressure(particle);

	for (SphParticle neighbour_particle : neighbours) {
		density_acceleration += neighbour_particle.mass * 
			((computeLocalPressure(neighbour_particle) / (neighbour_particle.local_density * neighbour_particle.local_density)) + 
			(particle_local_pressure / (particle.local_density * particle.local_density))) * 
			kernel->computeKernelGradientValue(particle.position - neighbour_particle.position);
	}

	return density_acceleration;
}

double SphManager::computeLocalPressure(SphParticle& particle) {
	double local_pressure = 0.0;
	double refrence_density_of_water = 1.0;
	//double pressure_constant = 1.0; //to be evaluated, is chosen arbitrarily at the moment
	double reference_pressure = 101300.0; //not sure atm

	//local_pressure = pressure_constant *(particle.local_density - refrence_density_of_water); //old version meight be better
	local_pressure = reference_pressure * (std::pow((particle.local_density / refrence_density_of_water), 7.0) - 1); //other implementation of local pressure
	//std::cout << local_pressure << " " << (std::pow((particle.local_density / refrence_density_of_water), 7.0) - 1) << std::endl; //debug
	return local_pressure;
}

Vector3 SphManager::computeViscosityAcceleration(SphParticle& particle) {
	std::vector<SphParticle> neighbours = neighbour_search->findNeigbours(particle, domains);
	Vector3 viscosity_acceleration = Vector3();

	for (SphParticle neighbour_particle : neighbours)
	{
		Vector3 rij = neighbour_particle.position - particle.position;
		viscosity_acceleration += neighbour_particle.mass * ((4 * 1.0 * rij * kernel->computeKernelGradientValue(rij)) / 
			((particle.local_density + neighbour_particle.local_density) * (rij.length() * rij.length()))) * 
			(particle.velocity - neighbour_particle.velocity);
	}
	if (particle.local_density == 0) {
		return Vector3();
	}
	return ((1 / particle.local_density) * viscosity_acceleration);
}

void SphManager::findNeighbourDomains(ParticleDomain) {

}

void SphManager::exchangeParticles() {
	std::unordered_map<int, std::vector<SphParticle>> target_map;

	for (auto each_domain : domains) {
		std::vector<SphParticle> outside_particles = each_domain.second.removeParticlesOutsideDomain();
		for (SphParticle each_particle : outside_particles) {
			int target_id = computeTargetProcess(each_particle);
			int count = target_map.count(target_id);

			if (count == 0) {
				target_map.at(target_id) = std::vector<SphParticle>();
			}

			std::vector<SphParticle> list = target_map.at(target_id);
			list.push_back(each_particle);
		}
	}

	std::vector<SphParticle> all_new_particles;
	std::vector<SphParticle> incoming_particles;

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
		MPI_Isend(vector.second.data(), vector.second.size() * sizeof(SphParticle), MPI_BYTE, vector.first, 0, MPI_COMM_WORLD, &request);
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

int SphManager::computeTargetDomain(const SphParticle& particle) const{
	Vector3 targetDomainCoords = (particle.position / domain_dimensions).roundDownward();
	//std::cout << "x:" << targetDomainCoords.x << " y:" << targetDomainCoords.y << " z:" << targetDomainCoords.z << std::endl; // debug output
	return hash(targetDomainCoords);
}

int SphManager::computeTargetProcess(const SphParticle& particle) const {
	return computeTargetDomain(particle) % world_size;
}

ParticleDomain& SphManager::getParticleDomain(const int& unique_id) {
	int count = domains.count(unique_id);
	if (count == 0) {
		domains[unique_id] = ParticleDomain(unhash(unique_id) * domain_dimensions, domain_dimensions);
	}
	return domains.at(unique_id);
}

void SphManager::add_particles(const std::vector<SphParticle>& new_particles) {
	for (SphParticle particle : new_particles) {
		int domain_id = computeTargetDomain(particle);
		//std::cout << domain_id << std::endl; // Debug output
		getParticleDomain(domain_id).addParticle(particle);
		//std::cout << particle.position.x << particle.position.y << particle.position.z << std::endl; // debug output
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

	std::vector<SphParticle> rim_particles;

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
	MPI_Isend(rim_particles.data(), rim_particles.size() * sizeof(SphParticle), MPI_BYTE, requester % world_size, 0, MPI_COMM_WORLD, &request);
	MPI_Request_free(&request);
}
