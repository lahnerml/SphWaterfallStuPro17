#pragma once
#include "SphNeighbourSearch.h"

SphNeighbourSearch::SphNeighbourSearch(double q_max, int slave_comm_size) :
	q_max(q_max),
	slave_comm_size(slave_comm_size){

}

SphNeighbourSearch::~SphNeighbourSearch() {

}

std::vector<SphParticle> SphNeighbourSearch::findNeigbours(SphParticle particle, std::vector<SphParticle> neighbour_particles) {
	std::vector<SphParticle> neighbours = std::vector<SphParticle>();

	std::cout << "find neigbours relevant neighbours found" << std::endl;

	for (auto each_particle : neighbour_particles) {
		if (isInInfluentialRadius(particle.position, each_particle.position)) {
			neighbours.push_back(each_particle);
		}
	}

	return neighbours;
}

std::set<int> SphNeighbourSearch::findRelevantNeighbourDomains(SphParticle particle, Vector3 dimension) {
	std::set<int> neighbour_domain_ids = std::set<int>();

	std::cout << "find relevant neighbours" << std::endl;

	// center point
	Vector3 testing_point = particle.position;
	neighbour_domain_ids.insert(hash(testing_point));

	computeDomainID(testing_point, dimension, slave_comm_size);
	// front point
	testing_point = particle.position + (Vector3(0, 0, 1) * q_max);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension, slave_comm_size));
	// back point
	testing_point = particle.position + (Vector3(0, 0, -1) * q_max);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension, slave_comm_size));
	// left point
	testing_point = particle.position + (Vector3(-1, 0, 0) * q_max);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension, slave_comm_size));
	// right point
	testing_point = particle.position + (Vector3(1, 0, 0) * q_max);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension, slave_comm_size));
	// top point
	testing_point = particle.position + (Vector3(0, 1, 0) * q_max);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension, slave_comm_size));
	// bottom point
	testing_point = particle.position + (Vector3(0, -1, 0) * q_max);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension, slave_comm_size));

	// front bottom point
	testing_point = particle.position + (Vector3(0, 1, 1) * q_max);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension, slave_comm_size));
	// front top point
	testing_point = particle.position + (Vector3(0, -1, 1) * q_max);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension, slave_comm_size));
	// front left point
	testing_point = particle.position + (Vector3(-1, 0, 1) * q_max);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension, slave_comm_size));
	// front right point
	testing_point = particle.position + (Vector3(1, 0, 1) * q_max);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension, slave_comm_size));
	// front left bottom point
	testing_point = particle.position + (Vector3(-1, -1, 1) * q_max);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension, slave_comm_size));
	// front right bottom point
	testing_point = particle.position + (Vector3(1, -1, 1) * q_max);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension, slave_comm_size));
	// front left top point
	testing_point = particle.position + (Vector3(-1, 1, 1) * q_max);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension, slave_comm_size));
	// front right top point
	testing_point = particle.position + (Vector3(1, 1, 1) * q_max);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension, slave_comm_size));

	// back top point
	testing_point = particle.position + (Vector3(0, 1, -1) * q_max);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension, slave_comm_size));
	// back bottom point
	testing_point = particle.position + (Vector3(0, -1, -1) * q_max);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension, slave_comm_size));
	// back left point
	testing_point = particle.position + (Vector3(-1, 0, -1) * q_max);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension, slave_comm_size));
	// back right point
	testing_point = particle.position + (Vector3(1, 0, -1) * q_max);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension, slave_comm_size));
	// back left bottom point
	testing_point = particle.position + (Vector3(-1, -1, -1) * q_max);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension, slave_comm_size));
	// back right bottom point
	testing_point = particle.position + (Vector3(1, -1, -1) * q_max);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension, slave_comm_size));
	// back left top point
	testing_point = particle.position + (Vector3(-1, 1, -1) * q_max);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension, slave_comm_size));
	// back right top point
	testing_point = particle.position + (Vector3(1, 1, -1) * q_max);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension, slave_comm_size));

	// left top point
	testing_point = particle.position + (Vector3(-1, 1, 0) * q_max);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension, slave_comm_size));
	// left bottom point
	testing_point = particle.position + (Vector3(-1, -1, 0) * q_max);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension, slave_comm_size));
	// right top point
	testing_point = particle.position + (Vector3(1, 1, 0) * q_max);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension, slave_comm_size));
	// right bottom point
	testing_point = particle.position + (Vector3(1, -1, 0) * q_max);
	neighbour_domain_ids.insert(computeDomainID(testing_point, dimension, slave_comm_size));
		
	return neighbour_domain_ids;
}

bool SphNeighbourSearch::isInInfluentialRadius(Vector3 particle, Vector3 potential_neighbour) {
	return (((potential_neighbour.x - particle.x) * (potential_neighbour.x - particle.x)) + 
		((potential_neighbour.y - particle.y) * (potential_neighbour.y - particle.y)) + 
		((potential_neighbour.z - particle.z) * (potential_neighbour.z - particle.z))) <  (q_max * q_max);
}
