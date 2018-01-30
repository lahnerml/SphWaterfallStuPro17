#pragma once
#include "SphParticle.h"

SphParticle::SphParticle() :
	position(Vector3()),
	velocity(Vector3()),
	pType(ParticleType::FLUID)
{
}

SphParticle::SphParticle(Vector3 position) :
	position(position),
	pType(ParticleType::FLUID)
{
}

SphParticle::SphParticle(Vector3 position, Vector3 velocity) :
	position(position),
	velocity(velocity),
	pType(ParticleType::FLUID)
{
}

SphParticle::SphParticle(Vector3 position, SphParticle::ParticleType pType) :
	position(position),
	velocity(Vector3()),
	pType(pType)
{
}

SphParticle::~SphParticle() {

}

SphParticle::ParticleType SphParticle::getParticleType()
{
	return this->pType;
}