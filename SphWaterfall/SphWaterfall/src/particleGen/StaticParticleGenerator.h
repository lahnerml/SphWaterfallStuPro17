#pragma once
#include <vector>

#include "mpi.h"

#include "../geometry/Terrain.h"
#include "../data/SphParticle.h"
#include "../data/Vector3.h"
#include "../simulation/SphManager.h"

class StaticParticleGenerator {
public:
	StaticParticleGenerator();

	void sendAndGenerate(Terrain&, SphParticle::ParticleType);
	void receiveAndGenerate(SphManager&, SphParticle::ParticleType);

	static std::vector<SphParticle> generateStaticParticles(Terrain&);

	static void generateParticlesOnFace(Face&, double, std::vector<SphParticle>&, SphParticle::ParticleType);
	static std::vector<SphParticle> generateParticlesOnFace(Face&, double, SphParticle::ParticleType);
  
	static void detectDuplicate(SphParticle, SphParticle);
	static void removeDuplicate(SphParticle, SphParticle);
private:
};

