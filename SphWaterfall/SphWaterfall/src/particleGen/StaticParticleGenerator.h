#pragma once
#include <vector>

#include "../geometry/Terrain.h"
#include "../data/SphParticle.h"

class StaticParticleGenerator {
public:
	StaticParticleGenerator();
	std::vector<ISphParticle> generateStaticParticles(Face face, float particleDistance);

};