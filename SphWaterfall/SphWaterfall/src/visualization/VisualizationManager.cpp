#pragma once
#include "VisualizationManager.h"

void VisualizationManager::init(Vector3 cameraLocation, unsigned int frameWidth, unsigned int frameHeight) {
	camera = Camera(cameraLocation, (cameraLocation*-1).normalize(), frameWidth, frameHeight);
	if (terrainOpen.getVertexCount() > 0 && terrainClosed.getVertexCount() > 0) {
		camera.renderGeometryFrames(terrainOpen, terrainClosed);
	}
	initilaized = true;
}

void VisualizationManager::debugRenderFrame(std::vector<SphParticle> particles, string fileName) {
	camera.debugRenderFrame(convertSphParticles(particles));
}

void VisualizationManager::importTerrain(Terrain t, bool open) {
	if (open) { terrainOpen = t; }
	else { terrainClosed = t; }
}

void VisualizationManager::renderFrames(string inputFileName) {
	if (!initilaized) {
		cout << "Please initialize the VisualizationManager before rendering!";
	}

	vector<vector<SphParticle>> frameParticles = ParticleIO::importParticles(inputFileName);

	for (int g = 0; g < frameParticles.size(); g++) {
		cout << "Rendering Frame #" << g << "...\n";
		vector<ParticleObject> frame = convertFluidParticles(frameParticles.at(g));

		Frame f = camera.renderFrame(frame, g);

		writeFrameToBitmap(f, (("output/frame_" + std::to_string(g)) + ".bmp").c_str(), f.getWidth(), f.getHeight());
	}
}

void VisualizationManager::generateFrames(int frameCount, int particleCount) {
	unordered_map<int, vector<SphParticle>> frames;

	cout << "Generating " << frameCount << " frames...\n";

	for (int i = 1; i <= frameCount; i++) {
		frames.insert_or_assign(i, generateDebugParticles(particleCount));
	}

	cout << "Frames generated\n";

	ParticleIO::exportParticles(frames, "test.particles");
}

vector<SphParticle> VisualizationManager::generateDebugParticles(int count) {
	vector<SphParticle> particles;

	for (int i = 0; i < count; i++) {
		double x = double(rand() % 10000 - 5000) / 1000;
		double y = double(rand() % 10000 - 5000) / 1000;
		double z = double(rand() % 10000 - 5000) / 1000;

		SphParticle f = SphParticle(Vector3(x, y, z), Vector3(0, 0, 0), 1);
		particles.emplace_back(f);
	}
	return particles;
}

bool VisualizationManager::initilaized = false;
Camera VisualizationManager::camera = Camera();
Terrain VisualizationManager::terrainOpen = Terrain();
Terrain VisualizationManager::terrainClosed = Terrain();