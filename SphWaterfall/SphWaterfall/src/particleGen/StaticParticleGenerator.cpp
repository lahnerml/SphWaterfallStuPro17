#pragma once
#include "StaticParticleGenerator.h"
#include "../data/SphParticle.h"

StaticParticleGenerator::StaticParticleGenerator() {

}

std::vector<ISphParticle> StaticParticleGenerator :: generateStaticParticles(Terrain source) {
	std::vector<ISphParticle> staticParticles;
	for (int i = 0; i < source.getVertexCount(); i++) {
		staticParticles.push_back(SphParticle(source.getVertexPosition(i)));
		
	}
	return staticParticles;

}




