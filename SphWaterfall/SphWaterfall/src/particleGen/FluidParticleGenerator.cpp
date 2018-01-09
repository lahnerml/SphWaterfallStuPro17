#pragma once
#include "FluidParticleGenerator.h"

FluidParticleGenerator::FluidParticleGenerator(double disposalHeight, Vector3 spawn) {
	this-> disposalHeight = disposalHeight;
	this-> spawn = spawn;
}

ISphParticle FluidParticleGenerator::spawnParticle() {
	//TODO
	return ISphParticle();
}

void FluidParticleGenerator::collectInactive() {
	//TODO
}

