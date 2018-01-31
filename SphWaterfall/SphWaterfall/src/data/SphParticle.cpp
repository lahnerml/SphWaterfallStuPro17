#pragma once
#include "SphParticle.h"

SphParticle::SphParticle() :
	position(Vector3()),
	velocity(Vector3())
{
}

SphParticle::SphParticle(Vector3 position) :
	position(position),
	velocity(Vector3())
{
}

SphParticle::SphParticle(Vector3 position, Vector3 velocity) :
	position(position),
	velocity(velocity)
{
}

SphParticle::~SphParticle() {

}

ISphParticle::ParticleType SphParticle::getParticleType()
{
	return ISphParticle::ParticleType::FLUID;
}