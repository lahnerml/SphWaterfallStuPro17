#pragma once
#include "Vector3.h"

class ISphParticle {
	public:
		Vector3 position;
		Vector3 velocity;
		double mass;
		double local_density;
		double viscosity;

		void setDensity(double);
		void setViscosity(double);
	private:
};