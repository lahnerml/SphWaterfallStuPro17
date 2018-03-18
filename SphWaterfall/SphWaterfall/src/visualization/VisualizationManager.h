#pragma once

#include "Camera.h"
#include "util.h"
#include "../data/SphParticle.h"
#include "../data/ParticleIO.h"
#include <string>


class VisualizationManager {
public:
	static void init(Vector3 cameraLocation, unsigned int frameWidth, unsigned int frameHeight);

	static void importTerrain(Terrain t, bool open);

	static void debugRenderFrame(std::vector<SphParticle> particles, string fileName);

	static void renderFrames(string inputFileName);

	static void generateFrames(int frameCount, int particleCount);

	static void debug();

private:
	static vector<SphParticle> generateDebugParticles(int count);

	static bool initilaized;

	static Camera camera;
	static Terrain terrainOpen;
	static Terrain terrainClosed;
};
