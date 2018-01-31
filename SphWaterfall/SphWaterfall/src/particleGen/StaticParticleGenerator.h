#pragma once
#include <vector>

#include "../geometry/Terrain.h"
#include "../data/ISphParticle.h"

class StaticParticleGenerator {
public:
	StaticParticleGenerator();
	std::vector<ISphParticle> generateStaticParticles(Terrain source);
	std::vector<ISphParticle> generateStaticParticles(Face face, float particleDistance);

};