#pragma once
#include "SphParticle.h"

SphParticle::SphParticle() {

}

SphParticle::SphParticle(Vector3 position) :
	position(position){
	this->mass = 0.0;
}

SphParticle::SphParticle(Vector3 position, double mass) :
	position(position),
	mass(mass){
}

SphParticle::~SphParticle() {

}
