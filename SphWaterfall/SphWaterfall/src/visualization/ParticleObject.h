#pragma once

#include <mpi.h>
#include <math.h>
#include "Ray.h"
#include "Pixel.h"
#include "../data/Vector3.h"

//Simple Klasse welche Daten für eine Kugel im Raum beinhaltet
class ParticleObject {
public:
	ParticleObject(Vector3 location, double radius);
	ParticleObject();

	Vector3 getLocation();
	double getRadius();

	//Berechnet einen Schnittpunkt zwischen dem ParticleObject und einem Ray
	bool intersects(Ray &ray, double &distance, double &waterDepth, double maxDepth);

	//MPI Funktionalitaet zum Verteilen über Prozessoren
	static void MpiSendPObject(ParticleObject pObj, int dest);
	static ParticleObject MpiReceivePObject(int source);

private:
	Vector3 location;
	double radius;
};