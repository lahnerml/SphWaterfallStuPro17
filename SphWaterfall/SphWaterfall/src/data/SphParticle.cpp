#pragma once
#include "SphParticle.h"

SphParticle::SphParticle() :
	position(Vector3()),
	velocity(Vector3()),
	particle_type(SphParticle::ParticleType::FLUID) {
	this->mass = FLUID_MASS;
	this->local_density = FLUID_REFERENCE_DENSITY;
}

SphParticle::SphParticle(Vector3 position) :
	position(position),
	velocity(Vector3()),
	particle_type(SphParticle::ParticleType::FLUID) {
	this->mass = FLUID_MASS;
	this->local_density = FLUID_REFERENCE_DENSITY;

}

SphParticle::SphParticle(Vector3 position, Vector3 velocity) :
	position(position),
	velocity(velocity),
	particle_type(SphParticle::ParticleType::FLUID) {
	this->mass = FLUID_MASS;
	this->local_density = FLUID_REFERENCE_DENSITY;
}

SphParticle::SphParticle(Vector3 position, Vector3 velocity, double mass) :
	position(position),
	velocity(velocity),
	mass(mass),
	particle_type(SphParticle::ParticleType::FLUID) {
	this->local_density = FLUID_REFERENCE_DENSITY;
}

SphParticle::SphParticle(Vector3 position, SphParticle::ParticleType particle_type) :
	position(position),
	velocity(Vector3()),
	particle_type(particle_type) {
	if (particle_type == SphParticle::ParticleType::STATIC) {
		this->mass = STATIC_MASS;
		this->local_density = STATIC_REFERENCE_DENSITY;
	}
	else {
		this->mass = FLUID_MASS;
		this->local_density = FLUID_REFERENCE_DENSITY;
	}
}

SphParticle::~SphParticle() {

}

bool operator==(const SphParticle& a, const SphParticle& b) {
	return ((a.position == b.position) && (a.velocity == b.velocity));
}

std::ostream& operator<<(std::ostream & out, const SphParticle& particle) {
	return out << "position: " << particle.position <<  "  velocity: " << particle.velocity << "  local_density:" << particle.local_density;
}

SphParticle::ParticleType SphParticle::getParticleType() const {
	return this->particle_type;
}