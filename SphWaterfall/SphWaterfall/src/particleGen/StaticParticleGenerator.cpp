#pragma once
#include "StaticParticleGenerator.h"

StaticParticleGenerator::StaticParticleGenerator() {

}

	for (int i = 0; i < source.getVertexCount(); i++) {
		staticParticles.push_back(SphParticle(source.getVertex(i)));
		
	}
	return staticParticles;

}

/*
 * Generates static particles uniformly on a given face
 * face: The face to genrate the particles on
 * particleDensity: How much space lies between 2 particles 0.0 < density <= 1.0
 */
std::vector<ISphParticle> StaticParticleGenerator::generateStaticParticles(Terrain terrain, int faceId, float particleDensity)
{
	std::vector<ISphParticle> result = std::vector<ISphParticle>();
	Vector3 particlePosition = Vector3();

	if (0.0 >= particleDensity || 1.0 < particleDensity)
		return std::vector<ISphParticle>();

	//Create uniform grid of particles
	for (double x = 0.0; x <= 1.0; x += particleDensity)
	{
		for (double y = 0.0; y <= 1.0; y += particleDensity)
		{
			if (x + y < 1)
				continue;

			particlePosition = face.a + (face.b * x) + (face.c * y);
			result.push_back(StaticParticle(particlePosition));
		}
	}

}


StaticParticleGenerator StaticParticleGenerator::detectDuplicate(ISphParticle a, ISphParticle b) {
	
}

StaticParticleGenerator StaticParticleGenerator::removeDuplicate(ISphParticle a, ISphParticle b) {
	
}