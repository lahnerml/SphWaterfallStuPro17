#pragma once
#include "SphParticle.h"

SphParticle::SphParticle() {

}

SphParticle::SphParticle(Vector3 position) {

}

SphParticle::SphParticle(Vector3 position, Vector3 velocity) :
	velocity(velocity) {
	this->position = position;
}

SphParticle::~SphParticle() {

}
