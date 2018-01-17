#pragma once
#include "StaticParticleGenerator.h"
#include "../data/SphParticle.h"

StaticParticleGenerator::StaticParticleGenerator() {

}

std::vector<ISphParticle> StaticParticleGenerator :: generateStaticParticles(Terrain source) {
	std::vector<ISphParticle> staticParticle;
	for (int i = 0; i < source.getVertexCount(); i++) {
		SphParticle(Vector3);
		staticParticle.push_back(Vector3);
		
	}
	return staticParticle;

}




