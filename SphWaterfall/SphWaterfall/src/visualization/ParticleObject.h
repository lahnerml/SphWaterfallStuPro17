#pragma once

#include <mpi.h>
#include <math.h>
#include "Ray.h"
#include "Pixel.h"
#include "../data/Vector3.h"

class ParticleObject {
public:
	ParticleObject(Vector3 location, double radius);
	ParticleObject();

	Vector3 getLocation();
	double getRadius();

	bool intersects(Ray &ray, double &distance, double &highestDistance);

	static void MpiSendPObject(ParticleObject pObj, int dest);
	static ParticleObject MpiReceivePObject(int source);

private:
	Vector3 location;
	double radius;
};