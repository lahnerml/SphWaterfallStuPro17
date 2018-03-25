#pragma once
#include <vector>

#include "mpi.h"

#include "../geometry/Terrain.h"
#include "../data/SphParticle.h"
#include "../data/StaticParticle.h"
#include "../data/Vector3.h"
#include "../simulation/SphManager.h"

class StaticParticleGenerator {
public:
	StaticParticleGenerator();

	void sendAndGenerate(Terrain);
	void receiveAndGenerate(SphManager&);

	static std::vector<SphParticle> generateStaticParticles(Terrain);

	static void generateParticlesOnFace(Face&, double, std::vector<SphParticle>&);
	static std::vector<SphParticle> generateParticlesOnFace(Face&, double);
  
	static void detectDuplicate(SphParticle, SphParticle);
	static void removeDuplicate(SphParticle, SphParticle);
private:
};

