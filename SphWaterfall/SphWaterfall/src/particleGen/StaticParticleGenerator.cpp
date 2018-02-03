#pragma once
#include "StaticParticleGenerator.h"
#include "../data/FluidParticle.h"

StaticParticleGenerator::StaticParticleGenerator() {

}

std::vector<SphParticle> StaticParticleGenerator :: generateStaticParticles(Terrain source) {
	std::vector<SphParticle> staticParticles;
	for (int i = 0; i < source.getVertexCount(); i++) {
		staticParticles.push_back(FluidParticle(source.getVertexPosition(i)));
		
	}
	return staticParticles;

}