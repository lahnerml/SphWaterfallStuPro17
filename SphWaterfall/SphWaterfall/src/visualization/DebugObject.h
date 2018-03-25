#pragma once
#include <math.h>
#include "Ray.h"
#include "Pixel.h"
#include "../data/Vector3.h"

class DebugObject {
public:
	DebugObject(Vector3 location, double radius);
	DebugObject();

	Vector3 getLocation();
	double getRadius();

	bool intersects(Ray &ray, double &distance, double &highestDistance);

private:
	Vector3 location;
	double radius;
};