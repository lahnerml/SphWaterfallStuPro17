#pragma once
#include "StaticParticle.h"

StaticParticle::StaticParticle() :
	pos(Vector3())
{
}

StaticParticle::StaticParticle(Vector3 position) :
	pos(position)
{
}

StaticParticle::~StaticParticle() {

}


Vector3 StaticParticle::position()
{
	return this->position;
}

void StaticParticle::position(Vector3 position)
{
	this->position = position;
}

Vector3 StaticParticle::velocity()
{
	return this->velocity;
}

void StaticParticle::velocity(Vector3 velocity)
{
}

IStaticParticle::ParticleType StaticParticle::getParticleType()
{
	return IStaticParticle::ParticleType::STATIC;
}