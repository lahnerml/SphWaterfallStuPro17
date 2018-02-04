#pragma once
#include "ISphNeighbourSearch.h"

class SphNeighbourSearch : public ISphNeighbourSearch {
public:
	SphNeighbourSearch(double q_max, int slave_comm_size);
	~SphNeighbourSearch();

	std::vector<SphParticle> findNeigbours(SphParticle particle, std::vector<SphParticle> neighbour_particles);
	std::set<int> findRelevantNeighbourDomains(SphParticle particle, Vector3 dimension);

private:
	double q_max;
	int slave_comm_size;

	bool isInInfluentialRadius(Vector3 particle, Vector3 potential_neighbour);
};