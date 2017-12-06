#pragma once
#include "SphNeighbourSearch.h"

SphNeighbourSearch::SphNeighbourSearch() {

}

SphNeighbourSearch::~SphNeighbourSearch() {

}

std::vector<ISphParticle> findNeigbours(ISphParticle, ParticleDomain domains[]) {
	return _Null_;
}

std::vector<ParticleDomain> findRelevantNeighbourDomains(ISphParticle, ParticleDomain domains[]){
	return _Null_;
}
