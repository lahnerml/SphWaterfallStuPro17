#pragma once
#include "SphParticle.h"

SphParticle::SphParticle() :
	position(Vector3()),
	velocity(Vector3()),
	pType(ISphParticle::ParticleType::FLUID)
{
}

SphParticle::SphParticle(Vector3 position) :
	position(position),
	velocity(Vector3()),
	pType(ISphParticle::ParticleType::FLUID)
{
}

SphParticle::SphParticle(Vector3 position, Vector3 velocity) :
	position(position),
	velocity(velocity),
	pType(ISphParticle::ParticleType::FLUID)
{
}

SphParticle::SphParticle(Vector3 position, ISphParticle::ParticleType particleType) :
	position(position),
	velocity(Vector3()),
	pType(particleType)
{
}

SphParticle::~SphParticle() {

}

ISphParticle::ParticleType SphParticle::getParticleType()
{
	return this->pType;
}