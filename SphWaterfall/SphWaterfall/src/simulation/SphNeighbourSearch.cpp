#include "SphNeighbourSearch.h"

SphNeighbourSearch::SphNeighbourSearch() {

}

SphNeighbourSearch::~SphNeighbourSearch() {

}

std::vector<SphParticle> SphNeighbourSearch::findNeigbours(const Vector3& particle_position, const std::vector<SphParticle>& potential_neighbour_particles) const {
	std::vector<SphParticle> neighbours = std::vector<SphParticle>();

	for (auto each_particle : potential_neighbour_particles) {
		if (isInInfluentialRadius(particle_position, each_particle.position)) {
			neighbours.push_back(each_particle);

		}
	}

	//std::cout << "for particle: " << particle_position << " are neighbours: " << std::endl;

	return neighbours;
}

std::set<int> SphNeighbourSearch::findRelevantNeighbourDomains(const Vector3& particle_position, const Vector3& dimension) const {
	std::set<int> neighbour_domain_ids = std::set<int>();

	Vector3 testing_point;
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			for (int z = -1; z <= 1; z++) {
				testing_point = particle_position + (Vector3(x, y, z).normalize() * R_MAX);
				neighbour_domain_ids.insert(computeDomainID(testing_point, dimension));
			}
		}
	}
	return neighbour_domain_ids;
}
/* for particles are more likely to be inside of the influential radius*/
/*
bool SphNeighbourSearch::isInInfluentialRadius(const Vector3& particle_position, const Vector3& potential_neighbour_particle_position) const {
	Vector3 distance = (potential_neighbour_particle_position - particle_position).absolute();

	if (distance.x + distance.y + distance.z <= R_MAX) {
		return true;
	}
	if (distance.x > R_MAX || distance.y > R_MAX ||distance.z > R_MAX) {
		return false;
	}

	return ((distance.x * distance.x) + (distance.y * distance.y) + (distance.z) * (distance.z)) < (R_MAX * R_MAX);
}
*/

/* for particles are more likely to be outside of the influential radius*/
bool SphNeighbourSearch::isInInfluentialRadius(const Vector3& particle_position, const Vector3& potential_neighbour_particle_position) const {
	Vector3 distance = (potential_neighbour_particle_position - particle_position).absolute();

	if (distance.x > R_MAX || distance.y > R_MAX || distance.z > R_MAX) {
		return false;
	}
	if (distance.x + distance.y + distance.z <= R_MAX) {
		return true;
	}

	return ((distance.x * distance.x) + (distance.y * distance.y) + (distance.z) * (distance.z)) <= (R_MAX * R_MAX);
}
