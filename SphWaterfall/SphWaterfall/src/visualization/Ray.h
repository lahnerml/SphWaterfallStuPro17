#pragma once
#include "../data/Vector3.h"

//Ray Klasse; enthält einen Ursprung und eine Richtung
class Ray {
public:
	Ray(Vector3 direction, Vector3 origin);

	Vector3 origin;
	Vector3 direction;
};