#pragma once
#include "Vector3.h"

class ISphParticle {
	public:
		Vector3 position;
		// Changed by Magnus
		Vector3 velocity;
		double mass;
		double density;

		virtual void setDensity(double) = 0;
		virtual void setViscosity(double) = 0;
	private:
};