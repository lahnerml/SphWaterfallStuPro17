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

/*
 * Generates static particles uniformly on a given face
 * face: The face to genrate the particles on
 * particleDensity: How much space lies between 2 particles
 */
std::vector<ISphParticle> StaticParticleGenerator::generateStaticParticles(Terrain terrain, int faceId, float particleDensity)
{
	std::vector<ISphParticle> result = std::vector<ISphParticle>();
	Vector3 particlePosition = Vector3();
	
	Vector3 vertexA = terrain.getVertexPosition(terrain.getFace(faceId).a);
	Vector3 vertexB = terrain.getVertexPosition(terrain.getFace(faceId).b);
	Vector3 vertexC = terrain.getVertexPosition(terrain.getFace(faceId).c);

	//Create uniform grid of particles
	for (double x = 0.0; x <= 1.0; x += particleDensity)
	{
		for (double y = 0.0; y <= 1.0; y += particleDensity)
		{
			if (x + y < 1)
				continue;

			particlePosition = vertexA + (vertexB * x) + (vertexC * y);
		}
	}
}