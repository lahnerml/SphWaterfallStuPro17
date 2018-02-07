#pragma once
#include "SphParticle.h"

SphParticle::SphParticle() {

}

SphParticle::SphParticle(Vector3 position) :
	position(position){
	this->velocity = Vector3();
	this->mass = 0.0;
}

SphParticle::SphParticle(Vector3 position, Vector3 velocity) :
	position(position),
	velocity(velocity) {
	this->mass = 0.0;
}

SphParticle::SphParticle(Vector3 position, Vector3 velocity, double mass) :
	position(position),
	velocity(velocity),
	mass(mass){
}

SphParticle::~SphParticle() {

}

bool operator==(const SphParticle a, const SphParticle b)
{
	return ((a.position == b.position) && (a.velocity == b.velocity) && (a.mass == b.mass) && (a.local_density == b.local_density));
}

std::ostream& operator<<(std::ostream & out, const SphParticle& particle)
{
	return out << "position: " << particle.position <<  "  velocity: " << particle.velocity << "  mass:" << particle.mass << "  local_density:" << particle.local_density;
}