#pragma once
#include "Frame.h"
#include "DebugObject.h"
#include "../data/SphParticle.h"
#include "../data/Vector3.h"
#include "ParticleObject.h"
#include <vector>
#include <string>
#include "../geometry/Terrain.h"

class Camera {
public:
	Camera(Vector3 location, Vector3 direction, unsigned int width, unsigned int height, unsigned int ID);

	void debugRenderFrame(std::vector<DebugObject> particles);

	Frame renderFrame(std::vector<ParticleObject> particles); 
	void renderGeometryFrame(Terrain terrain);

	void outputDebugFrame(Frame f, const char* fileName);

	Vector3 getLocation();
	Vector3 getDirection();

private:

	Pixel castDebugRay(Ray ray, std::vector<DebugObject> particles);
	Pixel castVolumeRay(Ray ray, std::vector<ParticleObject> particles);
	Pixel castGeometryRay(Ray ray, Terrain terrain);

	Frame baseFrame;
	Vector3 location;
	Vector3 direction;
	unsigned int width;
	unsigned int height;
	unsigned int ID;

};