#pragma once
#include <vector>

#include "../data/ISphParticle.h"

class StaticParticleGenerator {
public:
	std::vector<ISphParticle> generateStaticParticles(Terrain source);
};