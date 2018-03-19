#pragma once
#include "SphParticle.h"

SphParticle::SphParticle() :
	position(Vector3()),
	velocity(Vector3()),
	pType(SphParticle::ParticleType::FLUID)
{
}

SphParticle::SphParticle(Vector3 position) :
	position(position),
	pType(SphParticle::ParticleType::FLUID) {
	this->velocity = Vector3();
	this->mass = 1.0;
	this->local_density = 20.0;

}

SphParticle::SphParticle(Vector3 position, Vector3 velocity) :
	position(position),
	velocity(velocity),
	pType(SphParticle::ParticleType::FLUID) {
	this->mass = 1.0;
	this->local_density = 20.0;

}

SphParticle::SphParticle(Vector3 position, Vector3 velocity, double mass) :
	position(position),
	velocity(velocity),
	mass(mass){
	this->local_density = 20.0;
}

SphParticle::SphParticle(Vector3 position, SphParticle::ParticleType particleType) :
	position(position),
	velocity(Vector3()),
	pType(particleType)
{
}

SphParticle::~SphParticle() {

}


bool operator==(const SphParticle a, const SphParticle b)
{
	return ((a.position == b.position) && (a.velocity == b.velocity)); // TODO: local density can't be a value because otherwise calculation won't work
}

std::ostream& operator<<(std::ostream & out, const SphParticle& particle)
{
	return out << "position: " << particle.position <<  "  velocity: " << particle.velocity << "  local_density:" << particle.local_density;
}
SphParticle::ParticleType SphParticle::getParticleType()
{
	return this->pType;
}

void SphParticle::makeInactive()
{
	if (this->pType == SphParticle::ParticleType::FLUID)
		this->pType = SphParticle::ParticleType::INACTIVE;
}