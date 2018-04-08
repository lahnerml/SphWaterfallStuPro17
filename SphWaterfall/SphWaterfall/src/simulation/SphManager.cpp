#include "SphManager.h"
#include <chrono>
#include <thread>

#define PRESSURE_CONSTANT 20.0

SphManager::SphManager(const Vector3& domain_dimensions, int number_of_timesteps, double timestep_duration) :
	domain_dimensions(domain_dimensions),
	number_of_timesteps(number_of_timesteps),
	timestep_duration(timestep_duration),
	gravity_acceleration(Vector3(0.0, -9.81, 0.0)),
	sink_height(0.0)
{
	half_timestep_duration = timestep_duration / 2.0;

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

	if (mpi_rank == 0) {
		std::cout << "prepare simulation..." << std::endl;
	}

	exchangeParticles();
	if (mpi_rank == 0) {
		std::cout << "finished static exchange" << std::endl;
	}
	exchangeRimParticles(SphParticle::STATIC);
	if (mpi_rank == 0) {
		std::cout << "finished static rim exchange" << std::endl;
	}
	if (mpi_rank == 0) {
		std::cout << "starting simulation..." << std::endl;
	}

	int exchange_rim_particles_time, update_particles_time, exchange_particles_time, simulation_timestep_time, export_particles_time;
	std::chrono::steady_clock::time_point begin, end;

	for (int simulation_timestep = 1; simulation_timestep <= number_of_timesteps; simulation_timestep++) {
		if (mpi_rank == 0) {
			begin = std::chrono::steady_clock::now();
		}
		exchangeRimParticles(SphParticle::FLUID);
		if (mpi_rank == 0) {
			end = std::chrono::steady_clock::now();
			exchange_rim_particles_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
			std::cout << "finished rim exchange in " << exchange_rim_particles_time << "ms"<< std::endl;
			begin = std::chrono::steady_clock::now();
		}
		update();
		if (mpi_rank == 0) {
			end = std::chrono::steady_clock::now();
			update_particles_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
			std::cout << "finished update in " << update_particles_time << "ms" << std::endl;
			begin = std::chrono::steady_clock::now();
		}
		spawnSourceParticles();
		if (mpi_rank == 0) {
			end = std::chrono::steady_clock::now();
			update_particles_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
			std::cout << "finished particle spawn in " << update_particles_time << "ms" << std::endl;
			begin = std::chrono::steady_clock::now();
		}
		exchangeParticles();
		if (mpi_rank == 0) {
			end = std::chrono::steady_clock::now();
			exchange_particles_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
			std::cout << "finished exchange in " << exchange_particles_time << "ms" << std::endl;
			begin = std::chrono::steady_clock::now();
		}
		exportParticles();
		if (mpi_rank == 0) {
			end = std::chrono::steady_clock::now();
			export_particles_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
			std::cout << "finished export in " << export_particles_time << "ms" << std::endl;

			simulation_timestep_time = exchange_rim_particles_time + update_particles_time + exchange_particles_time + export_particles_time;
			std::cout << "finished simulation of timestep " << simulation_timestep << " in " << simulation_timestep_time << " ms" << std::endl;
		}
	}

	cleanUpFluidParticles();
}

void SphManager::update() {

	int neighbour_search_time, local_density_calculation_time, velocity_and_position_update_time;
	std::chrono::steady_clock::time_point begin, end;

	if (mpi_rank == 0) {
		begin = std::chrono::steady_clock::now();
	}

	// neighbour search
	std::vector<SphParticle> each_neighbour_particles;
	std::unordered_map <int, std::vector<SphParticle>> each_neighbour_rim_particles;

	neighbour_particles.clear();

	for (auto& each_domain : domains) {
		if (each_domain.second.hasFluidParticles()) {
			// gets neighbour particle map from domain
			each_neighbour_rim_particles = each_domain.second.getNeighbourRimParticles();

			for (auto& each_particle : each_domain.second.getParticles()) {
				if (each_particle.getParticleType() == SphParticle::FLUID) {
					// gets particles of domain the particle is in
					each_neighbour_particles = each_domain.second.getParticles();

					for (auto& domain_id : neighbour_search->findRelevantNeighbourDomains(each_particle.position, domain_dimensions)) {
						// tests if domain of particle has neighbour particles for currently looked at neighbour domain
						if (each_neighbour_rim_particles.count(domain_id) != 0) {
							each_neighbour_particles.insert(each_neighbour_particles.end(),
								each_neighbour_rim_particles.at(domain_id).begin(),
								each_neighbour_rim_particles.at(domain_id).end());
						}
					}

					each_neighbour_particles = neighbour_search->findNeigbours(each_particle.position, each_neighbour_particles);
					neighbour_particles.push_back(std::pair<SphParticle, std::vector<SphParticle>>(each_particle, each_neighbour_particles));
				}
			}
		}
	}

	if (mpi_rank == 0) {
		end = std::chrono::steady_clock::now();
		neighbour_search_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
		std::cout << "finished neighbour search in " << neighbour_search_time << "ms" << std::endl;
		begin = std::chrono::steady_clock::now();
	}
	
	// compute and set local densities
	for (auto& each_domain : domains) {
		for (auto& each_particle : each_domain.second.getParticles()) {
			if (each_particle.getParticleType() == SphParticle::FLUID) {
				computeLocalDensity(each_particle);
			}
		}
	}
	
	if (mpi_rank == 0) {
		end = std::chrono::steady_clock::now();
		local_density_calculation_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
		std::cout << "finished density calculation in " << local_density_calculation_time << "ms" << std::endl;
		begin = std::chrono::steady_clock::now();
	}
	
	// compute and update Velocities and position
	for (auto& each_domain : domains) {
		if (each_domain.second.hasFluidParticles()) {
			std::vector<SphParticle> &particles = each_domain.second.getParticles();
			for (int i = 0; i < particles.size(); i++) {
				if (particles.at(i).getParticleType() == SphParticle::FLUID && updateVelocity(particles.at(i))) {
					particles.erase(particles.begin() + i);
					--i;
					//std::cout << "final particle: " << each_particle << " on processor " << mpi_rank + 1 << std::endl; // debug
				}
			}
		}
	}

	if (mpi_rank == 0) {
		end = std::chrono::steady_clock::now();
		velocity_and_position_update_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
		std::cout << "finished velocity and position update in " << velocity_and_position_update_time << "ms" << std::endl;
		begin = std::chrono::steady_clock::now();
	}
	
}

bool SphManager::updateVelocity(SphParticle& particle) {
	Vector3 accelleration_timestep_start = computeAcceleration(particle);
	particle.velocity += (half_timestep_duration * accelleration_timestep_start);
	Vector3 position_timestep_half = particle.position + (half_timestep_duration * particle.velocity);

	Vector3 accelleration_timestep_half = computeAcceleration(particle);
	Vector3 velocity_timestep_end = particle.velocity + (timestep_duration * accelleration_timestep_half);
	particle.position = position_timestep_half + (half_timestep_duration * velocity_timestep_end);
	return particle.position.y <= sink_height;
}

Vector3 SphManager::computeAcceleration(SphParticle& particle) {
	Vector3 acceleration = gravity_acceleration + computeDensityAcceleration(particle) + computeViscosityAcceleration(particle);
	return acceleration;
}

void SphManager::computeLocalDensity(SphParticle& particle) {
	double local_density = 0.0;
	std::vector<SphParticle> neighbours;
	for (auto& neighbour : neighbour_particles) {
		if (particle == neighbour.first) {
			neighbours = neighbour.second;
			break;
		}
	}

	for (SphParticle& neighbour_particle : neighbours) {
		local_density += neighbour_particle.mass * kernel->computeKernelValue(particle.position - neighbour_particle.position);
	}

	//if (local_density < FLUID_REFERENCE_DENSITY) {
	//	particle.local_density = FLUID_REFERENCE_DENSITY;
	//}
	//else {
		particle.local_density = local_density;
	//}
}

Vector3 SphManager::computeDensityAcceleration(SphParticle& particle) {
	std::vector<SphParticle> neighbours;
	for (auto& neighbour : neighbour_particles) {
		if (particle == neighbour.first) {
			neighbours = neighbour.second;
			break;
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

double SphManager::computeLocalPressure(SphParticle& particle) {
	return PRESSURE_CONSTANT * (particle.local_density - FLUID_REFERENCE_DENSITY);
}

Vector3 SphManager::computeViscosityAcceleration(SphParticle& particle) {
	std::vector<SphParticle> neighbours;
	for (auto& neighbour : neighbour_particles) {
		if (particle == neighbour.first) {
			neighbours = neighbour.second;
			break;
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

void SphManager::exchangeRimParticles(SphParticle::ParticleType particle_type) {
	// target domain id, source domain id, rim particles from source in direction of target domain
	std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::vector<SphParticle>>>> target_source_map;
	std::vector<std::vector<int>> meta_list;
	std::vector<int> size_list;
	std::unordered_map<int, std::unordered_map<int, std::vector<SphParticle>>> new_rim_particles;
	int target_domain_id, source_domain_id;

	for (auto& each_domain : domains) {
		each_domain.second.clearNeighbourRimParticles(particle_type);
		if (each_domain.second.hasFluidParticles() || (particle_type != SphParticle::FLUID && each_domain.second.size() > 0)) {
			source_domain_id = each_domain.first;

			for (auto& each_target : each_domain.second.getRimParticleTargetMap(particle_type)) {
				if (!each_target.second.empty()) {
					int target_process_id = computeProcessID(each_target.first);
					if (target_process_id == mpi_rank) {
						new_rim_particles[each_target.first][source_domain_id] = each_target.second;
					}
					else {
						target_source_map[target_process_id][each_target.first][source_domain_id] = each_target.second;
					}
				}
			}
		}
	}

	// send meta meta data	
	for (int i = 0; i < slave_comm_size; i++) {
		if (i != mpi_rank) {
			int size = 0;
			for (auto& target : target_source_map[i]) {
				size += static_cast<int>(target.second.size());
			}
			size_list.push_back(size);

			MPI_Request request;
			//std::cout << mpi_rank << " send meta meta for " << size_list.back() << " to " << i << std::endl;
			MPI_Isend(&size_list.back(), 1, MPI_INT, i, META_META_RIM_TAG, slave_comm, &request);
		}
	}

	//std::cout << mpi_rank << " finished sending meta meta" << std::endl;

	// receive meta meta from all other processors and post meta receives
	for (int i = 0; i < slave_comm_size; i++) {
		if (i != mpi_rank) {
			int meta_count;
			MPI_Request request;
			MPI_Recv(&meta_count, 1, MPI_INT, i, META_META_RIM_TAG, slave_comm, MPI_STATUS_IGNORE);
			meta_list.push_back(std::vector<int>(1 + meta_count * 4));
			MPI_Irecv(meta_list.back().data() + 1, 4 * meta_count, MPI_INT, i, META_RIM_TAG, slave_comm, &request);
			meta_list.back()[0] = i;
			//std::cout << mpi_rank << " post meta receive size " << 4 * meta_count << " to " << i << std::endl;
		}
	}
	//std::cout << mpi_rank << " finished posting meta receives" << std::endl;
	size_list.clear();

	// send meta data
	for (int i = 0; i < slave_comm_size; i++) {
		if (i != mpi_rank) {
			int count = RIM_TAG;
			std::vector<int> meta;
			for (auto& target : target_source_map[i]) {
				for (auto& source : target.second) {
					// target domain id, source domain id, tag der richtigen Nachricht, größe der richtigen Nachricht
					meta.push_back(target.first);
					meta.push_back(source.first);
					meta.push_back(count);
					meta.push_back(static_cast<int>(source.second.size()));
					// increment unique tag
					count++;
				}
			}
			//std::cout << mpi_rank << " trying to send meta " << meta.size() << " to " << i << std::endl;
			MPI_Ssend(meta.data(), meta.size(), MPI_INT, i, META_RIM_TAG, slave_comm);
		}
	}

	//std::cout << mpi_rank << " finished sending meta" << std::endl;
	MPI_Barrier(slave_comm);
	// post receive for particles
	for (auto& each_meta : meta_list) {
		int process_id = each_meta[0];
		int meta_count = each_meta.size() / 4;
		for (int i = 0; i < meta_count; i++) {
			int target = each_meta[1 + i * 4];
			int source = each_meta[2 + i * 4];
			int tag = each_meta[3 + i * 4];
			int count = each_meta[4 + i * 4];
			new_rim_particles[target][source] = std::vector<SphParticle>(count);
			MPI_Request request;
			MPI_Irecv(new_rim_particles[target][source].data(), count * sizeof(SphParticle), MPI_BYTE, process_id, tag, slave_comm, &request);
		}
		//std::cout << mpi_rank << " post receive Tag " << each_meta[3] << " for " << each_meta[4] << " to " << each_meta[0] << std::endl;
	}

	//std::cout << mpi_rank << " finished posting receives" << std::endl;

	// send particles
	
	for (int i = 0; i < slave_comm_size; i++) {
		if (i != mpi_rank) {
			int count = RIM_TAG;
			for (auto& target : target_source_map[i]) {
				for (auto& source : target.second) {
					//std::cout << mpi_rank << " trying to send Tag " << count << " with " << source.second.size() << " to " << i << std::endl;
					MPI_Ssend(source.second.data(), source.second.size() * sizeof(SphParticle), MPI_BYTE, i, count, slave_comm);
					// increment unique tag
					count++;
				}
			}
		}
	}

	//std::cout << mpi_rank << " finished sending particles" << std::endl;
	MPI_Barrier(slave_comm);
	for (auto& target : new_rim_particles) {
		if (domains.count(target.first) != 0) {
			getParticleDomain(target.first).addNeighbourRimParticles(target.second);
		}
	}

	new_rim_particles.clear();
}

void SphManager::setSink(const double& sink_height)
{
	this->sink_height = sink_height;
}

void SphManager::addSource(const Vector3& source)
{
	sources.push_back(source);
}

const Vector3& SphManager::getDomainDimensions() const
{
	return domain_dimensions;
}

void SphManager::exchangeParticles() {
	std::unordered_map<int, std::vector<SphParticle>> target_map;
	std::vector<SphParticle> all_new_particles;
	std::vector<std::vector<SphParticle>> incoming_particles;
	int target_id;

	// adds particles that are provided by the addParticles method
	for (int i = 0; i < slave_comm_size; i++) {
		target_map[i] = add_particles_map.at(i);
		add_particles_map.at(i).clear();
	}
	
	// adds particles from domains
	for (auto& each_domain : domains) {
		if (each_domain.second.hasFluidParticles()) {
			std::vector<SphParticle> outside_particles = each_domain.second.removeParticlesOutsideDomain();

			for (auto& each_particle : outside_particles) {
				target_id = computeProcessID(each_particle.position, domain_dimensions);

				if (target_map.at(target_id).empty()) {
					target_map[target_id] = std::vector<SphParticle>();
				}

				target_map[target_id].push_back(each_particle);
			}
		}
	}

	// don't send to yourself
	int count = target_map.count(mpi_rank);
	if (count != 0) {
		all_new_particles = target_map.at(mpi_rank);
		target_map.erase(mpi_rank);
	}
	//std::cout << mpi_rank << " started exchange" << std::endl;

	// send meta data
	for (int i = 0; i < slave_comm_size; i++) {
		int size = target_map[i].size();
		if (i != mpi_rank) {
			MPI_Request request;
			MPI_Isend(&size, 1, MPI_INT, i, META_EXCHANGE_TAG, slave_comm, &request);
		}
	}
	//std::cout << mpi_rank << " finished sending meta" << std::endl;

	// receive meta from all other processors and post receives
	for (int i = 0; i < slave_comm_size; i++) {
		if (i != mpi_rank) {
			int size;
			MPI_Recv(&size, 1, MPI_INT, i, META_EXCHANGE_TAG, slave_comm, MPI_STATUS_IGNORE);
			if (size != 0) {
				incoming_particles.push_back(std::vector<SphParticle>(size));
				MPI_Request request;
				MPI_Irecv(incoming_particles.back().data(), size * sizeof(SphParticle), MPI_BYTE, i, EXCHANGE_TAG, slave_comm, &request);
			}
		}
	}
	//std::cout << mpi_rank << " finished sending posting receives" << std::endl
	// send particles
	for (auto& vector : target_map) {
		if (vector.second.size() != 0) {
			MPI_Ssend(vector.second.data(), vector.second.size() * sizeof(SphParticle), MPI_BYTE, vector.first, EXCHANGE_TAG, slave_comm);
		}		
	}
	//std::cout << mpi_rank << " finished sending particles" << std::endl;
	MPI_Barrier(slave_comm);
	add_particles(all_new_particles);
	for (auto& each_new_particle_list : incoming_particles) {
		add_particles(each_new_particle_list);
	}
}

void SphManager::cleanUpAllParticles() {
	for (auto& each_domain : domains) {
		each_domain.second.clearParticles();
		each_domain.second.clearNeighbourRimParticles();
	}
	//for (auto& each_domain : domains) {for (auto& each_particle : each_domain.second.getParticles()) {std::cout << "particle in domain: " << each_particle << std::endl;}} // debug
}

void SphManager::cleanUpFluidParticles() {
	for (auto& each_domain : domains) {
		if (each_domain.second.hasFluidParticles()) {
			each_domain.second.clearParticles(SphParticle::FLUID);
		}
		each_domain.second.clearNeighbourRimParticles(SphParticle::FLUID);
	}
	//for (auto& each_domain : domains) {for (auto& each_particle : each_domain.second.getParticles()) {std::cout << "particle in domain: " << each_particle << std::endl;}} // debug
}

void SphManager::cleanUpStaticParticles() {
	for (auto& each_domain : domains) {
		if (each_domain.second.hasStaticParticles()) {
			each_domain.second.clearParticles(SphParticle::STATIC);
		}
		each_domain.second.clearNeighbourRimParticles(SphParticle::STATIC);
	}
	//for (auto& each_domain : domains) {for (auto& each_particle : each_domain.second.getParticles()) {std::cout << "particle in domain: " << each_particle << std::endl;}} // debug
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
		int process_id = computeProcessID(domain_id);

		if (process_id == mpi_rank) {
			getParticleDomain(domain_id).addParticle(particle);
		}
		else {
			add_particles_map[process_id].push_back(particle);
		}
		//std::cout << particle.position << std::endl; // debug
	}
}

void SphManager::spawnSourceParticles() {

	if (sources.empty()) {
		return;
	}

	std::vector<SphParticle> new_particles;
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_real_distribution<double> distribution(-SOURCE_SIZE, SOURCE_SIZE);
	auto random = std::bind(distribution, generator);

	for (auto& each_source : sources) {
		new_particles.push_back(SphParticle(each_source + Vector3(random(), random(), random())));
	}
	add_particles(new_particles);
}

void SphManager::exportParticles() {
	std::vector<SphParticle> particles_to_export;
	
	for (auto& each_domain : domains) {
		if (each_domain.second.hasFluidParticles()) {
			for (SphParticle& each_particle : each_domain.second.getParticles()) {
				if (each_particle.getParticleType() == SphParticle::FLUID) {
					particles_to_export.push_back(each_particle);
				}
			}
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);

	//for (auto each_particle : particles_to_export) { std::cout << "export particle: " << each_particle << std::endl; } // debug 

	// send number of particles to master
	int number_of_particles_to_send = static_cast<int>(particles_to_export.size());

	MPI_Send(&number_of_particles_to_send, 1, MPI_INT, 0, EXPORT_PARTICLES_NUMBER_TAG, MPI_COMM_WORLD);

	//send particles to master
	if (number_of_particles_to_send != 0) {
		MPI_Send(particles_to_export.data(), number_of_particles_to_send * sizeof(SphParticle), MPI_BYTE, 0, EXPORT_TAG, MPI_COMM_WORLD);
	}

}