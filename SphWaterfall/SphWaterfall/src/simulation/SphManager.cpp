#pragma once
#include "SphManager.h"

#define REFERENCE_DENSITY 20.0
#define PRESSURE_CONSTANT 20.0

SphManager::SphManager(const Vector3& domain_dimensions, int number_of_timesteps, double timestep_duration) :
	domain_dimensions(domain_dimensions),
	number_of_timesteps(number_of_timesteps),
	timestep_duration(timestep_duration) 
{
	kernel = kernel_factory.getInstance(1);
	neighbour_search = neighbour_search_factory.getInstance(1);

	for (int i = 0; i < slave_comm_size + 1; i++) {
		add_particles_map[i] = std::vector<SphParticle>();
	}
}

SphManager::~SphManager() {

}

void SphManager::simulate() {
	MPI_Comm_rank(slave_comm, &mpi_rank);

	exchangeParticles();

	for (int simulation_timestep = 1; simulation_timestep <= number_of_timesteps; simulation_timestep++) {
		exchangeRimParticles();
		//std::cout << "processor " << mpi_rank + 1 << " after timestep " << simulation_timestep << " exchange rim particles" << std::endl; //debug
		update();
		//std::cout << "processor " << mpi_rank + 1 << " after timestep " << simulation_timestep << " update" << std::endl; //debug
		exchangeParticles();
		//std::cout << "processor " << mpi_rank + 1 << " after timestep " << simulation_timestep << " exchange particles" << std::endl; //debug

		//if ((simulation_timestep % 20) == 1) {
			exportParticles();
		//}
		
		if (mpi_rank == 0) {
			std::cout << "simulation of timestep " << simulation_timestep << " finished" << std::endl;
		}
	}
}

void SphManager::update() {
	// neighbour search
	std::vector<SphParticle> each_neighbour_particles;
	std::unordered_map <int, std::vector<SphParticle>> each_neighbour_rim_particles;
	int i = 0;
	for (auto& each_domain : domains) {
		each_neighbour_rim_particles = each_domain.second.getNeighbourRimParticles();
		for (auto& each_particle : each_domain.second.getParticles()) {
			if (each_particle.getParticleType() == SphParticle::ParticleType::FLUID) {
				each_neighbour_particles = each_domain.second.getParticles();
				for (auto& domain_id : neighbour_search->findRelevantNeighbourDomains(each_particle, domain_dimensions)) {
					if (each_neighbour_rim_particles.count(domain_id) != 0) {
						each_neighbour_particles.insert(each_neighbour_particles.end(),
							each_neighbour_rim_particles.at(domain_id).begin(),
							each_neighbour_rim_particles.at(domain_id).end());
					}
				}
				each_neighbour_particles = neighbour_search->findNeigbours(each_particle, each_neighbour_particles);
				neighbour_particles[i] = std::pair<SphParticle, std::vector<SphParticle>>(each_particle, each_neighbour_particles);
				i++;
			}
		}
	}
	
	//std::cout << "after neighbour search" << std::endl; // debug
	// compute and set local densities
	for (auto& each_domain : domains) {
		for (auto& each_particle : each_domain.second.getParticles()) {
			if (each_particle.getParticleType() == SphParticle::ParticleType::FLUID) {
				computeLocalDensity(each_particle);
			}
		}
	}
	//std::cout << "after compute local densities" < < std::endl; // debug
	// compute and update Velocities and position
	for (auto& each_domain : domains) {
		for (auto& each_particle : each_domain.second.getParticles()) {
			if (each_particle.getParticleType() == SphParticle::ParticleType::FLUID) {
				updateVelocity(each_particle);
				//std::cout << "final particle: " << each_particle << " on processor " << mpi_rank + 1 << std::endl; // debug
			}
		}
	}
	//std::cout << "after update velocity" << std::endl;
}

void SphManager::updateVelocity(SphParticle& particle) {
	Vector3 accelleration_timestep_start = computeAcceleration(particle);
	particle.velocity += ((timestep_duration / 2) * accelleration_timestep_start);
	Vector3 position_timestep_half = particle.position + ((timestep_duration / 2) * particle.velocity);

	Vector3 accelleration_timestep_half = computeAcceleration(particle);
	Vector3 velocity_timestep_end = particle.velocity + (timestep_duration * accelleration_timestep_half);
	particle.position = position_timestep_half + ((timestep_duration / 2) * velocity_timestep_end);
}

Vector3 SphManager::computeAcceleration(SphParticle& particle) {
	Vector3 acceleration;
	Vector3 gravity_acceleration = Vector3(0.0, -9.81, 0.0);
	acceleration = gravity_acceleration + computeDensityAcceleration(particle) + computeViscosityAcceleration(particle);
	return acceleration;
}

void SphManager::computeLocalDensity(SphParticle& particle) {
	double local_density = 0.0;
	// std::vector<SphParticle> neighbours = neighbour_particles.at(particle); TODO: implement Hash for SphParticle (operator())
	std::vector<SphParticle> neighbours;
	for (auto& neighbour : neighbour_particles) {
		if (particle == neighbour.second.first) {
			neighbours = neighbour.second.second;
		}
	}

	for (SphParticle& neighbour_particle : neighbours) {
		local_density += neighbour_particle.mass * kernel->computeKernelValue(particle.position - neighbour_particle.position);
	}

	if (local_density < REFERENCE_DENSITY) {
		particle.local_density = REFERENCE_DENSITY;
	}
	else {
		particle.local_density = local_density;
	}
}

Vector3 SphManager::computeDensityAcceleration(SphParticle& particle) {
	// std::vector<SphParticle> neighbours = neighbour_particles.at(particle); TODO: implement Hash for SphParticle (operator())
	std::vector<SphParticle> neighbours;
	for (auto& neighbour : neighbour_particles) {
		if (particle == neighbour.second.first) {
			neighbours = neighbour.second.second;
		}
	}

	Vector3 density_acceleration = Vector3();
	double particle_local_pressure = computeLocalPressure(particle);

	for (SphParticle& neighbour_particle : neighbours) {
		density_acceleration -= (neighbour_particle.mass / particle.mass) *
			((particle_local_pressure + computeLocalPressure(neighbour_particle)) / (2 * particle.local_density * neighbour_particle.local_density)) * 
			(kernel->computeKernelGradientValue(particle.position - neighbour_particle.position));
	}

	//std::cout << "after density acceleration:" << density_acceleration << std::endl; //debug
	return density_acceleration;
}

/*
Vector3 SphManager::computeDensityAcceleration(SphParticle& particle) {
	// std::vector<SphParticle> neighbours = neighbour_particles.at(particle); TODO: implement Hash for SphParticle (operator())
	std::vector<SphParticle> neighbours;
	for (auto& neighbour : neighbour_particles) {
		if (particle == neighbour.second.first) {
			neighbours = neighbour.second.second;
		}
	}

	Vector3 density_acceleration = Vector3();
	double particle_local_pressure = computeLocalPressure(particle);

	for (SphParticle& neighbour_particle : neighbours) {
		density_acceleration += neighbour_particle.mass *
			((computeLocalPressure(neighbour_particle) / (neighbour_particle.local_density * neighbour_particle.local_density)) +
			(particle_local_pressure / (particle.local_density * particle.local_density))) *
			kernel->computeKernelGradientValue(particle.position - neighbour_particle.position);
	}
	std::cout << "after density acceleration:" << density_acceleration << std::endl; //debug
	return density_acceleration;
}
*/

double SphManager::computeLocalPressure(SphParticle& particle) {
	return PRESSURE_CONSTANT * (particle.local_density - REFERENCE_DENSITY);
}

Vector3 SphManager::computeViscosityAcceleration(SphParticle& particle) {
	// std::vector<SphParticle> neighbours = neighbour_particles.at(particle); TODO: implement Hash for SphParticle (operator())
	std::vector<SphParticle> neighbours;
	for (auto& neighbour : neighbour_particles) {
		if (particle == neighbour.second.first) {
			neighbours = neighbour.second.second;
		}
	}

	Vector3 viscosity_acceleration = Vector3();
	Vector3 rij;
	for (SphParticle& neighbour_particle : neighbours)
	{
		rij = neighbour_particle.position - particle.position;
		if (rij.length() != 0.0) {
			viscosity_acceleration += neighbour_particle.mass * ( (4.0 * 1.0 * rij * kernel->computeKernelGradientValue(rij)) /
				((particle.local_density + neighbour_particle.local_density) * (rij.length() * rij.length())) ) *
				(particle.velocity - neighbour_particle.velocity);
		}
	}

	viscosity_acceleration *= 1 / particle.local_density;

	//std::cout << "after viscosity acceleration:" << viscosity_acceleration << std::endl; //debug
	return viscosity_acceleration;
}

void SphManager::exchangeRimParticles() {
	// target domain id, source domain id, rim particles from source in direction of target domain
	std::unordered_map<int, std::unordered_map<int, std::vector<SphParticle>>> target_map;
	int target_domain_id, source_domain_id;

	for (auto& each_domain : domains) {
		source_domain_id = each_domain.first;
		Vector3 source_domain_origin = unhash(source_domain_id);

		each_domain.second.clearRimParticles();
		
		target_domain_id = hash(source_domain_origin + Vector3(-1, -1, -1));
		target_map[target_domain_id][source_domain_id] = each_domain.second.getFrontLeftBottomRimParticles();
		target_domain_id = hash(source_domain_origin + Vector3(-1, -1, 0));
		target_map[target_domain_id][source_domain_id] = each_domain.second.getLeftBottomRimParticles();
		target_domain_id = hash(source_domain_origin + Vector3(-1, -1, 1));
		target_map[target_domain_id][source_domain_id] = each_domain.second.getBackLeftBottomRimParticles();
		target_domain_id = hash(source_domain_origin + Vector3(-1, 0, -1));
		target_map[target_domain_id][source_domain_id] = each_domain.second.getFrontLeftRimParticles();
		target_domain_id = hash(source_domain_origin + Vector3(-1, 0, 0));
		target_map[target_domain_id][source_domain_id] = each_domain.second.getLeftRimParticles();
		target_domain_id = hash(source_domain_origin + Vector3(-1, 0, 1));
		target_map[target_domain_id][source_domain_id] = each_domain.second.getBackLeftRimParticles();
		target_domain_id = hash(source_domain_origin + Vector3(-1, 1, -1));
		target_map[target_domain_id][source_domain_id] = each_domain.second.getFrontLeftTopRimParticles();
		target_domain_id = hash(source_domain_origin + Vector3(-1, 1, 0));
		target_map[target_domain_id][source_domain_id] = each_domain.second.getLeftTopRimParticles();
		target_domain_id = hash(source_domain_origin + Vector3(-1, 1, 1));
		target_map[target_domain_id][source_domain_id] = each_domain.second.getBackLeftTopRimParticles();
		target_domain_id = hash(source_domain_origin + Vector3(0, -1, -1));
		target_map[target_domain_id][source_domain_id] = each_domain.second.getFrontBottomRimParticles();
		target_domain_id = hash(source_domain_origin + Vector3(0, -1, 0));
		target_map[target_domain_id][source_domain_id] = each_domain.second.getBottomRimParticles();
		target_domain_id = hash(source_domain_origin + Vector3(0, -1, 1));
		target_map[target_domain_id][source_domain_id] = each_domain.second.getBackBottomRimParticles();
		target_domain_id = hash(source_domain_origin + Vector3(0, 0, -1));
		target_map[target_domain_id][source_domain_id] = each_domain.second.getFrontRimParticles();
		target_domain_id = hash(source_domain_origin + Vector3(0, 0, 1));
		target_map[target_domain_id][source_domain_id] = each_domain.second.getBackRimParticles();
		target_domain_id = hash(source_domain_origin + Vector3(0, 1, -1));
		target_map[target_domain_id][source_domain_id] = each_domain.second.getFrontTopRimParticles();
		target_domain_id = hash(source_domain_origin + Vector3(0, 1, 0));
		target_map[target_domain_id][source_domain_id] = each_domain.second.getTopRimParticles();
		target_domain_id = hash(source_domain_origin + Vector3(0, 1, 1));
		target_map[target_domain_id][source_domain_id] = each_domain.second.getBackTopRimParticles();
		target_domain_id = hash(source_domain_origin + Vector3(1, -1, -1));
		target_map[target_domain_id][source_domain_id] = each_domain.second.getFrontRightBottomRimParticles();
		target_domain_id = hash(source_domain_origin + Vector3(1, -1, 0));
		target_map[target_domain_id][source_domain_id] = each_domain.second.getRightBottomRimParticles();
		target_domain_id = hash(source_domain_origin + Vector3(1, -1, 1));
		target_map[target_domain_id][source_domain_id] = each_domain.second.getBackRightBottomRimParticles();
		target_domain_id = hash(source_domain_origin + Vector3(1, 0, -1));
		target_map[target_domain_id][source_domain_id] = each_domain.second.getFrontRightRimParticles();
		target_domain_id = hash(source_domain_origin + Vector3(1, 0, 0));
		target_map[target_domain_id][source_domain_id] = each_domain.second.getRightRimParticles();
		target_domain_id = hash(source_domain_origin + Vector3(1, 0, 1));
		target_map[target_domain_id][source_domain_id] = each_domain.second.getBackRightRimParticles();
		target_domain_id = hash(source_domain_origin + Vector3(1, 1, -1));
		target_map[target_domain_id][source_domain_id] = each_domain.second.getFrontRightTopRimParticles();
		target_domain_id = hash(source_domain_origin + Vector3(1, 1, 0));
		target_map[target_domain_id][source_domain_id] = each_domain.second.getRightTopRimParticles();
		target_domain_id = hash(source_domain_origin + Vector3(1, 1, 1));
		target_map[target_domain_id][source_domain_id] = each_domain.second.getBackRightTopRimParticles();
	}

	std::vector<SphParticle> incoming_rim_particles;
	std::unordered_map<int, std::unordered_map<int, std::vector<SphParticle>>> new_rim_particles;
	int count = 10;

	//for (auto target : target_map) { for (auto source : target.second) { std::cout << "source: " << unhash(source.first) << "  target: " << unhash(target.first) << std::endl; } }

	for (auto& target : target_map) {
		for (auto& source : target.second) {
			int target_process_id = computeProcessID(target.first, slave_comm_size);
			if (!source.second.empty()) {
				if (target_process_id == mpi_rank) {
					new_rim_particles[target.first][source.first] = source.second;
				}
				else {
					// target domain id, source domain id, tag der richtigen Nachricht
					std::array<int, 4> meta = { target.first, source.first, count, source.second.size()};
					MPI_Send(meta.data(), meta.size(), MPI_INT, target_process_id, META_TAG, slave_comm);

					//for (auto particle : source.second) { std::cout << particle << std::endl; } // debug
					// send particles
					MPI_Send(source.second.data(), source.second.size() * sizeof(SphParticle), MPI_BYTE, target_process_id, count, slave_comm);

					// increment unique tag
					count++;
				}
			}
		}	
	}

	MPI_Barrier(slave_comm);

	// receive until there is nothing left
	int flag, useless_flag;
	MPI_Status status;
	MPI_Iprobe(MPI_ANY_SOURCE, META_TAG, slave_comm, &flag, &status);
	
	while (flag) {
		int source = status.MPI_SOURCE;
		
		std::array<int, 4> meta = std::array<int, 4>();
		MPI_Recv(meta.data(), meta.size(), MPI_INT, source, META_TAG, slave_comm, &status);

		int tag = meta[2];
		count = meta[3] * sizeof(SphParticle);
		incoming_rim_particles = std::vector<SphParticle>(meta[3]);

		MPI_Recv(incoming_rim_particles.data(), count, MPI_BYTE, source, tag, slave_comm, MPI_STATUS_IGNORE);

		//for (auto particle : incoming_rim_particles) { std::cout << particle << std::endl; } // debug

		new_rim_particles[meta[0]][meta[1]] = incoming_rim_particles;

		// next message
		MPI_Iprobe(MPI_ANY_SOURCE, META_TAG, slave_comm, &flag, &status);
	}

	for (auto& target : new_rim_particles) {
		if (domains.count(target.first) != 0) {
			getParticleDomain(target.first).setNeighbourRimParticles(target.second);
			//for (auto neighbour : getParticleDomain(target.first).getNeighbourRimParticles()) { for (auto particle : neighbour.second) { std::cout << particle << std::endl; } } // debug
		}
	}

	new_rim_particles.clear();

	MPI_Barrier(slave_comm);
}

void SphManager::exchangeParticles() {
	std::unordered_map<int, std::vector<SphParticle>> target_map;
	std::vector<SphParticle> all_new_particles;
	std::vector<SphParticle> incoming_particles;
	int target_id;

	// adds particles that are provided by the addParticles method
	for (int i = 0; i < slave_comm_size; i++) {
		target_map[i] = add_particles_map.at(i);
		add_particles_map.at(i).clear();
	}
	
	// adds particles from domains
	for (auto& each_domain : domains) {
		std::vector<SphParticle> outside_particles = each_domain.second.removeParticlesOutsideDomain();

		for (auto& each_particle : outside_particles) {
			target_id = computeProcessID(each_particle.position, domain_dimensions, slave_comm_size);

			if (target_map.at(target_id).empty()) {
				target_map[target_id] = std::vector<SphParticle>();
			}

			target_map[target_id].push_back(each_particle);
		}
	}

	// don't send to yourself
	int count = target_map.count(mpi_rank);
	if (count != 0) {
		all_new_particles = target_map.at(mpi_rank);
		target_map.erase(mpi_rank);
	}
	MPI_Request request;
	for (auto& vector : target_map) {
		//for (auto particle : vector.second) { std::cout << "sending: " << particle << std::endl; } // debug
		MPI_Isend(vector.second.data(), vector.second.size() * sizeof(SphParticle), MPI_BYTE, vector.first, EXCHANGE_TAG, slave_comm, &request);
	}

	MPI_Barrier(slave_comm);

	// receive until there is nothing left
	int flag;
	MPI_Status status;
	MPI_Iprobe(MPI_ANY_SOURCE, EXCHANGE_TAG, slave_comm, &flag, &status);

	int source;
	while (flag) {
		source = status.MPI_SOURCE;
		MPI_Get_count(&status, MPI_BYTE, &count);
		incoming_particles = std::vector<SphParticle>(count / sizeof(SphParticle));

		MPI_Recv(incoming_particles.data(), count, MPI_BYTE, source, EXCHANGE_TAG, slave_comm, MPI_STATUS_IGNORE);
		all_new_particles.insert(all_new_particles.end(), incoming_particles.begin(), incoming_particles.end());
		//for (auto particle : incoming_particles) { std::cout << "incoming: " << particle << std::endl; } // debug

		// next message
		MPI_Iprobe(MPI_ANY_SOURCE, EXCHANGE_TAG, slave_comm, &flag, &status);
	}

	//for (auto particle : all_new_particles) { std::cout << "all new: " << particle << std::endl; } // debug
	
	add_particles(all_new_particles);
	MPI_Barrier(slave_comm);
}

ParticleDomain& SphManager::getParticleDomain(const int& domain_id) {
	int count = domains.count(domain_id);

	if (count == 0) {
		domains[domain_id] = ParticleDomain((unhash(domain_id) * domain_dimensions), domain_dimensions);
	}
	return domains.at(domain_id);
}

ParticleDomain& SphManager::getParticleDomain(const Vector3& particle_position) {
	int domain_id = computeDomainID(particle_position, domain_dimensions);

	return getParticleDomain(domain_id);
}

void SphManager::add_particles(const std::vector<SphParticle>& new_particles) {
	for (SphParticle particle : new_particles) {
		//std::cout << domain_id << std::endl; // debug
		int domain_id = computeDomainID(particle.position, domain_dimensions);
		int process_id = computeProcessID(domain_id, slave_comm_size);

		if (process_id == mpi_rank) {
			getParticleDomain(domain_id).addParticle(particle);
		}
		else {
			add_particles_map[process_id].push_back(particle);
		}
		//std::cout << particle.position << std::endl; // debug
	}
}

void SphManager::exportParticles() {
	//std::pair <int, std::vector<SphParticle>> particles_to_export;
	//particles_to_export.first = number_of_timesteps;
	std::vector<SphParticle> particles_to_export;
	
	for (auto& each_domain : domains) {
		for (auto each_particle : each_domain.second.getParticles()) {
			if (each_particle.getParticleType() == SphParticle::ParticleType::FLUID) {
				particles_to_export.push_back(each_particle);
			}
		}
	}

	// for (auto each_particle : particles_to_export) { std::cout << "final particle: " << each_particle << std::endl; } // debug 

	//send particles to master
	MPI_Send(particles_to_export.data(), particles_to_export.size() * sizeof(SphParticle), MPI_BYTE, 0, EXPORT_TAG, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);
}