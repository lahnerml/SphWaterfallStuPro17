#pragma once
#include "ISphNeighbourSearch.h"

class SphNeighbourSearch : public ISphNeighbourSearch {
public:
	SphNeighbourSearch();
	~SphNeighbourSearch();

	std::vector<SphParticle> findNeigbours(SphParticle, std::unordered_map<int, ParticleDomain> domains);
	std::vector<ParticleDomain> findRelevantNeighbourDomains(SphParticle, std::unordered_map<int, ParticleDomain> domains);
};