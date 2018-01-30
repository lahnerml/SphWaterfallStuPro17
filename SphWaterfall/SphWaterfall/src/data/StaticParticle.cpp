#pragma once
#include "StaticParticle.h"

StaticParticle::StaticParticle() :
	position(Vector3())
{
}

StaticParticle::StaticParticle(Vector3 position) :
	position(position)
{
}

StaticParticle::~StaticParticle() {

}

ISphParticle::ParticleType StaticParticle::getParticleType()
{
	return ISphParticle::ParticleType::STATIC;
}
