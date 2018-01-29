#pragma once
#include "../data/ISphParticle.h"
#include "ParticleDomain.h"
#include <unordered_map>

class ISphNeighbourSearch {
public:
	virtual std::vector<ISphParticle> findNeigbours(ISphParticle, std::unordered_map<int, ParticleDomain> domains) = 0;
	virtual std::vector<ParticleDomain> findRelevantNeighbourDomains(ISphParticle, std::unordered_map<int, ParticleDomain> domains) = 0;
};