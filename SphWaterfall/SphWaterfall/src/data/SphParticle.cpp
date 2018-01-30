#pragma once
#include "SphParticle.h"

SphParticle::SphParticle() :
	pos(Vector3()),
	vel(Vector3())
{
}

SphParticle::SphParticle(Vector3 position) :
	pos(position),
	vel(Vector3())
{
}

SphParticle::SphParticle(Vector3 position, Vector3 velocity) :
	pos(position),
	vel(velocity)
{
}

SphParticle::~SphParticle() {

}


Vector3 SphParticle::position()
{
	return this->position;
}

void SphParticle::position(Vector3 position)
{
	this->position = position;
}

Vector3 SphParticle::velocity()
{
	return this->velocity;
}

void SphParticle::velocity(Vector3 velocity)
{
	this->velocity = velocity;
}

ISphParticle::ParticleType SphParticle::getParticleType()
{
	return ISphParticle::ParticleType::FLUID;
}