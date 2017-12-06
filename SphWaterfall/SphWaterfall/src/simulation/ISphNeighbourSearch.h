#pragma once
#include "../data/ISphParticle.h"
#include "ParticleDomain.h"

class ISphNeighbourSearch {
public:
	virtual std::vector<ISphParticle> findNeigbours(ISphParticle, ParticleDomain domains[]) = 0; // usage of vector as array needs review
	virtual std::vector<ParticleDomain> findRelevantNeighbourDomains(ISphParticle, ParticleDomain domains[]) = 0; // ^
};