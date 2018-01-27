#pragma once
#include "Vector3.h"

class ISphParticle {
	public:
		Vector3 position;
		Vector3 velocity;
		double mass;
		double density;

	private:
};