#pragma once

#include "Ray.h"
#include "../data/Vector3.h"

class ParticleObject {
public:
	ParticleObject(Vector3 location, double radius);
	ParticleObject();

	Vector3 getLocation();
	double getRadius();

	bool intersects(Ray &ray, double &distance, double &highestDistance);

private:
	Vector3 location;
	double radius;
};