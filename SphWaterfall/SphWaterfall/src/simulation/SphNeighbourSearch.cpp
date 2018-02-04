#pragma once
#include "SphNeighbourSearch.h"

SphNeighbourSearch::SphNeighbourSearch(double q_max) :
	q_max(q_max){

}

SphNeighbourSearch::~SphNeighbourSearch() {

}

std::vector<SphParticle> SphNeighbourSearch::findNeigbours(SphParticle particle, std::unordered_map<int, ParticleDomain> domains) {
	std::vector<SphParticle> neighbours = std::vector<SphParticle>();
	std::vector<ParticleDomain> neighbour_domains = findRelevantNeighbourDomains(particle, domains);

	for (auto neighbour_domain : neighbour_domains) {
		for (auto each_particle : neighbour_domain.getParticles()) {
			if (isInInfluentialRadius(particle.position, each_particle.position)) {
				neighbours.push_back(each_particle);
			}
		}
	}

	return neighbours;
}

std::vector<ParticleDomain> SphNeighbourSearch::findRelevantNeighbourDomains(SphParticle particle, std::unordered_map<int, ParticleDomain> domains) {
	std::vector<ParticleDomain> relevant_neighbour_domains = std::vector<ParticleDomain>();
	std::set<int> neighbour_domain_ids = std::set<int>();
	double dimension = (domains.begin()->second.getDimensions().x) / 2;

	// center point
	Vector3 testing_point = particle.position;
	neighbour_domain_ids.insert(hash(testing_point));

	// front point
	testing_point = particle.position + (Vector3(0, 0, 1) * q_max);
	neighbour_domain_ids.insert(hash(testing_point));
	// back point
	testing_point = particle.position + (Vector3(0, 0, -1) * q_max);
	neighbour_domain_ids.insert(hash(testing_point));
	// left point
	testing_point = particle.position + (Vector3(1, 0, 0) * q_max);
	neighbour_domain_ids.insert(hash(testing_point));
	// right point
	testing_point = particle.position + (Vector3(-1, 0, 0) * q_max);
	neighbour_domain_ids.insert(hash(testing_point));
	// top point
	testing_point = particle.position + (Vector3(0, 1, 0) * q_max);
	neighbour_domain_ids.insert(hash(testing_point));
	// bottom point
	testing_point = particle.position + (Vector3(0, -1, 0) * q_max);
	neighbour_domain_ids.insert(hash(testing_point));

	// front bottom point
	testing_point = particle.position + (Vector3(0, 1, 1) * q_max);
	neighbour_domain_ids.insert(hash(testing_point));
	// front top point
	testing_point = particle.position + (Vector3(0, -1, 1) * q_max);
	neighbour_domain_ids.insert(hash(testing_point));
	// front left point
	testing_point = particle.position + (Vector3(1, 0, 1) * q_max);
	neighbour_domain_ids.insert(hash(testing_point));
	// front right point
	testing_point = particle.position + (Vector3(-1, 0, 1) * q_max);
	neighbour_domain_ids.insert(hash(testing_point));
	// front left bottom point
	testing_point = particle.position + (Vector3(1, -1, 1) * q_max);
	neighbour_domain_ids.insert(hash(testing_point));
	// front right bottom point
	testing_point = particle.position + (Vector3(-1, -1, 1) * q_max);
	neighbour_domain_ids.insert(hash(testing_point));
	// front left top point
	testing_point = particle.position + (Vector3(1, 1, 1) * q_max);
	neighbour_domain_ids.insert(hash(testing_point));
	// front right top point
	testing_point = particle.position + (Vector3(-1, 1, 1) * q_max);
	neighbour_domain_ids.insert(hash(testing_point));

	// back top point
	testing_point = particle.position + (Vector3(0, 0, -1) * q_max);
	neighbour_domain_ids.insert(hash(testing_point));
	// back bottom point
	testing_point = particle.position + (Vector3(0, 0, -1) * q_max);
	neighbour_domain_ids.insert(hash(testing_point));
	// back leftpoint
	testing_point = particle.position + (Vector3(0, 0, -1) * q_max);
	neighbour_domain_ids.insert(hash(testing_point));
	// back left point
	testing_point = particle.position + (Vector3(0, 0, -1) * q_max);
	neighbour_domain_ids.insert(hash(testing_point));
	// back left bottom point
	testing_point = particle.position + (Vector3(0, 0, -1) * q_max);
	neighbour_domain_ids.insert(hash(testing_point));
	// back right bottom point
	testing_point = particle.position + (Vector3(0, 0, -1) * q_max);
	neighbour_domain_ids.insert(hash(testing_point));
	// back left top point
	testing_point = particle.position + (Vector3(0, 0, -1) * q_max);
	neighbour_domain_ids.insert(hash(testing_point));
	// back right top point
	testing_point = particle.position + (Vector3(0, 0, -1) * q_max);
	neighbour_domain_ids.insert(hash(testing_point));

	// left top point
	testing_point = particle.position + (Vector3(1, 1, 0) * q_max);
	neighbour_domain_ids.insert(hash(testing_point));
	// left bottom point
	testing_point = particle.position + (Vector3(1, -1, 0) * q_max);
	neighbour_domain_ids.insert(hash(testing_point));
	// right top point
	testing_point = particle.position + (Vector3(-1, 1, 0) * q_max);
	neighbour_domain_ids.insert(hash(testing_point));
	// right bottom point
	testing_point = particle.position + (Vector3(-1, -1, 0) * q_max);
	neighbour_domain_ids.insert(hash(testing_point));
	
	for (int neighbout_domain_id : neighbour_domain_ids) {
		relevant_neighbour_domains.push_back(domains.at(neighbout_domain_id));
	}

	return relevant_neighbour_domains;
}

bool SphNeighbourSearch::isInInfluentialRadius(Vector3 particle, Vector3 potential_neighbour) {
	return (((potential_neighbour.x - particle.x) * (potential_neighbour.x - particle.x)) + 
		((potential_neighbour.y - particle.y) * (potential_neighbour.y - particle.y)) + 
		((potential_neighbour.z - particle.z) * (potential_neighbour.z - particle.z))) <  (q_max * q_max);
}
