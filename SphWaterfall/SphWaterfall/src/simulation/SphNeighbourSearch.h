#pragma once
#include "ISphNeighbourSearch.h"

class SphNeighbourSearch : public ISphNeighbourSearch {
public:
	SphNeighbourSearch();
	~SphNeighbourSearch();

	std::vector<ISphParticle> findNeigbours(ISphParticle, std::unordered_map<int, ParticleDomain> domains);
	std::vector<ParticleDomain> findRelevantNeighbourDomains(ISphParticle, std::unordered_map<int, ParticleDomain> domains);
};