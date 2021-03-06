#pragma once
#include "../data/SphParticle.h"
#include "ParticleDomain.h"
#include "SimulationUtilities.h"

#include <unordered_map>
#include <set>

using namespace SimulationUtilities;

class ISphNeighbourSearch {
public:
	virtual std::vector<SphParticle*> findNeigbours(const Vector3& particle_position, std::vector<SphParticle*>& potential_neighbour_particles) const = 0;
	virtual std::set<int> findRelevantNeighbourDomains(const Vector3& particle_position, const Vector3& dimension) const = 0;

private:
};