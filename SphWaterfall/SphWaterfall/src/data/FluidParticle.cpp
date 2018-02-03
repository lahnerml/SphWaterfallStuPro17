#pragma once
#include "FluidParticle.h"

FluidParticle::FluidParticle(Vector3 position) :
	SphParticle(position) {
	this->velocity = Vector3();
	this->mass = 0.0;
}

FluidParticle::FluidParticle(Vector3 position, Vector3 velocity) :
	SphParticle(position),
	velocity(velocity) {
	this->mass = 0.0;
}

FluidParticle::FluidParticle(Vector3 position, Vector3 velocity, double mass) :
	SphParticle(position,  mass),
	velocity(velocity){
}

FluidParticle::~FluidParticle() {

}

Vector3 FluidParticle::getVelocity() {
	return velocity;
}

void FluidParticle::setVelocity(Vector3 velocity) {
	this->velocity = velocity;
}