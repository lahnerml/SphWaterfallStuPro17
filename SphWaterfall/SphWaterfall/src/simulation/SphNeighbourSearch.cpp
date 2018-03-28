#include "SphNeighbourSearch.h"

SphNeighbourSearch::SphNeighbourSearch(){

}

SphNeighbourSearch::~SphNeighbourSearch() {

}

std::vector<SphParticle> SphNeighbourSearch::findNeigbours(SphParticle particle, std::vector<SphParticle> neighbour_particles) {
	std::vector<SphParticle> neighbours = std::vector<SphParticle>();

	//std::cout << "find neigbours" << std::endl;

	for (auto each_particle : neighbour_particles) {
		//if (isInInfluentialRadius(particle.position, each_particle.position)) {
			neighbours.push_back(each_particle);
		//}
	}
	
	return neighbours;
}

std::set<int> SphNeighbourSearch::findRelevantNeighbourDomains(SphParticle particle, Vector3 dimension) {
	std::set<int> neighbour_domain_ids = std::set<int>();

	//std::cout << "find relevant neighbours" << std::endl;

	Vector3 testing_point;
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			for (int z = -1; z <= 1; z++) {
				testing_point = particle.position + (Vector3(x, y, z).normalize() * R_MAX);
				neighbour_domain_ids.insert(computeDomainID(testing_point, dimension));
			}
		}
	}
	return neighbour_domain_ids;
}

bool SphNeighbourSearch::isInInfluentialRadius(Vector3 particle, Vector3 potential_neighbour) {
	return (((potential_neighbour.x - particle.x) * (potential_neighbour.x - particle.x)) + 
		((potential_neighbour.y - particle.y) * (potential_neighbour.y - particle.y)) + 
		((potential_neighbour.z - particle.z) * (potential_neighbour.z - particle.z))) <  (R_MAX * R_MAX);
}
