#pragma once
#include "SphNeighbourSearch.h"

SphNeighbourSearch::SphNeighbourSearch() {

}

SphNeighbourSearch::~SphNeighbourSearch() {

}

std::vector<SphParticle> SphNeighbourSearch::findNeigbours(SphParticle particle, std::unordered_map<int, ParticleDomain> domains) {
	std::vector<SphParticle> neighbours;
	return neighbours;
}

std::vector<ParticleDomain> SphNeighbourSearch::findRelevantNeighbourDomains(SphParticle particle, std::unordered_map<int, ParticleDomain> domains) {
	std::vector<ParticleDomain> relevantNeighbourDomain;
	return relevantNeighbourDomain;
}
