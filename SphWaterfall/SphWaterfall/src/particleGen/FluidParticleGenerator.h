#pragma once
#include "../data/Vector3.h"
#include "../data/ISphParticle.h" 

class FluidParticleGenerator {
public:
	
	FluidParticleGenerator(double disposalHeight, Vector3 spawn);
	ISphParticle spawnParticle();
	void collectInactive();



private:
	ISphParticle activeParticles[];
	ISphParticle inactiveParticles[];
	double disposalHeight;
	Vector3 spawn;
};