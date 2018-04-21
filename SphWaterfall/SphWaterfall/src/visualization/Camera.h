#pragma once
#include "Frame.h"
#include "DebugObject.h"
#include "Shader.h"
#include "../data/SphParticle.h"
#include "../data/Vector3.h"
#include "ParticleObject.h"
#include <vector>
#include <string>
#include "../geometry/Terrain.h"
#include "util.h"
#include "Ray.h"
#include <math.h>

class Camera {
public:
	Camera();
	Camera(Vector3 location, Vector3 direction, unsigned int width, unsigned int height);

	//Renders a frame based on debug object # Green on Black
	void debugRenderFrame(std::vector<DebugObject> particles);

	//Renders a frame based on Particle Objects # Uses Geometry Frames as base  # Color, Shader, Gate Switch included
	Frame renderFrame(std::vector<ParticleObject> particles, int frameID);

	//Renders
	void renderGeometryFrames(Terrain terrain, Terrain gate);

	void outputDebugFrame(Frame f, const char* fileName);
	void setGateSwitchFrame(unsigned int frameID);

	Vector3 getLocation();
	Vector3 getDirection();

private:

	Pixel castDebugRay(Ray ray, std::vector<DebugObject> particles);
	Pixel castTerrainRay(Ray ray, Terrain& terrain);
	Pixel castTerrainGateRay(Ray ray, Terrain& terrain, Terrain& gate);

	Pixel castVolumeRay(Ray ray, std::vector<ParticleObject> particles, Pixel basePixel);

	Frame& getCurrentlyUsedBaseFrame(unsigned int frameID);

	Frame baseFrameOpen;
	Frame baseFrameClosed;
	Vector3 location;
	Vector3 direction;

	unsigned int width;
	unsigned int height;
	unsigned int switchFrameID;

};