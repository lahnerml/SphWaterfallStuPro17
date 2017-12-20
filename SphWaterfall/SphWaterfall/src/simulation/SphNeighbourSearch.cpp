#pragma once
#include "SphNeighbourSearch.h"

SphNeighbourSearch::SphNeighbourSearch() {

}

SphNeighbourSearch::~SphNeighbourSearch() {

}

std::vector<ISphParticle> findNeigbours(ISphParticle, ParticleDomain domains[]) {
	std::vector<ISphParticle> neighbours;
	return neighbours;
}

std::vector<ParticleDomain> findRelevantNeighbourDomains(ISphParticle, ParticleDomain domains[]){
	std::vector<ParticleDomain> relevantNeighbourDomain;
	return relevantNeighbourDomain;
}
