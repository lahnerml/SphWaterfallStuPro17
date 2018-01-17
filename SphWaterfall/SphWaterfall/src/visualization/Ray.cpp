#pragma once
#include "Ray.h"

Ray::Ray(Vector3 direction, Vector3 origin) {
	this->direction = direction;
	this->origin = origin;
}