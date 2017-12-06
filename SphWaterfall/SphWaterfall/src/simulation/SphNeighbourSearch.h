#pragma once
#include "ISphNeighbourSearch.h"

class SphNeighbourSearch : public ISphNeighbourSearch {
public:
	SphNeighbourSearch();
	~SphNeighbourSearch();

	std::vector<ISphParticle> findNeigbours(ISphParticle, ParticleDomain domains[]);
	std::vector<ParticleDomain> findRelevantNeighbourDomains(ISphParticle, ParticleDomain domains[]); 
};