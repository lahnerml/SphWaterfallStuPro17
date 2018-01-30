#pragma once
#include "SphParticle.h"

SphParticle::SphParticle() {

}

SphParticle::SphParticle(Vector3 position) {
	this->position = position;
}

SphParticle::SphParticle(Vector3 position, Vector3 velocity) :
	position(position),
	velocity(velocity) {
}

SphParticle::SphParticle(Vector3 position, Vector3 velocity, double mass) :
	position(position),
	velocity(velocity),
	mass(mass){
}

SphParticle::~SphParticle() {

}


void ISphParticle::setDensity(double local_density) {
	this->local_density = local_density;
}

void ISphParticle::setViscosity(double viscosity) {
	this->viscosity = viscosity;
}
