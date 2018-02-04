#pragma once
#include "ISphNeighbourSearch.h"

class SphNeighbourSearch : public ISphNeighbourSearch {
public:
	SphNeighbourSearch(double q_max);
	~SphNeighbourSearch();

	std::vector<SphParticle> findNeigbours(SphParticle particle, std::unordered_map<int, ParticleDomain> domains);
	std::vector<ParticleDomain> findRelevantNeighbourDomains(SphParticle particle, std::unordered_map<int, ParticleDomain> domains);

private:
	double q_max;

	bool isInInfluentialRadius(Vector3 particle, Vector3 potential_neighbour);
};