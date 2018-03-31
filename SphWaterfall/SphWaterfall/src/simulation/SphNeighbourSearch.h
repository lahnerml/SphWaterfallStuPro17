#pragma once
#include "ISphNeighbourSearch.h"

class SphNeighbourSearch : public ISphNeighbourSearch {
public:
	SphNeighbourSearch();
	~SphNeighbourSearch();

	std::vector<SphParticle> findNeigbours(const Vector3& particle_position, const std::vector<SphParticle> potential_neighbour_particles) const;
	std::set<int> findRelevantNeighbourDomains(const Vector3& particle_position, const Vector3& dimension) const;

private:
	bool isInInfluentialRadius(const Vector3& particle_position, const Vector3& potential_neighbour_particle_position) const;

};