#pragma once
#include "SphParticle.h"

SphParticle::SphParticle() :
	position(Vector3()),
	velocity(Vector3()),
	particle_type(SphParticle::ParticleType::FLUID) {
	this->mass = 1.0;
	this->local_density = 20.0;
}

SphParticle::SphParticle(Vector3 position) :
	position(position),
	velocity(Vector3()),
	particle_type(SphParticle::ParticleType::FLUID) {
	this->mass = 1.0;
	this->local_density = 20.0;

}

SphParticle::SphParticle(Vector3 position, Vector3 velocity) :
	position(position),
	velocity(velocity),
	particle_type(SphParticle::ParticleType::FLUID) {
	this->mass = 1.0;
	this->local_density = 20.0;

}

SphParticle::SphParticle(Vector3 position, SphParticle::ParticleType particle_type) :
	position(position),
	velocity(Vector3()),
	particle_type(particle_type) {
	this->mass = 1.0;
	this->local_density = 20.0;
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
	return this->particle_type;
}