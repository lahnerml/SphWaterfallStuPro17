#pragma once
#include <vector>
#include "Ray.h"
#include "../data/Vector3.h"
#include "Pixel.h"

class DebugObject {
public:
	DebugObject(Vector3 location, double radius);
	DebugObject();

	Vector3 getLocation();
	double getRadius();

	Pixel getColor();

	bool intersects(Ray &ray, double &distance);

private:
	Vector3 location;
	double radius;
};