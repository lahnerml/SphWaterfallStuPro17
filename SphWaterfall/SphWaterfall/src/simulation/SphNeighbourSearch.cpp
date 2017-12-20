#pragma once
#include "SphNeighbourSearch.h"

SphNeighbourSearch::SphNeighbourSearch() {

}

SphNeighbourSearch::~SphNeighbourSearch() {

}

std::vector<ISphParticle> SphNeighbourSearch::findNeigbours(ISphParticle particle, ParticleDomain domains[]) {
	std::vector<ISphParticle> neighbours;
	return neighbours;
}

std::vector<ParticleDomain> SphNeighbourSearch::findRelevantNeighbourDomains(ISphParticle particle, ParticleDomain domains[]) {
	std::vector<ParticleDomain> relevantNeighbourDomain;
	return relevantNeighbourDomain;
}
