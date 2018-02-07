#include "Camera.h"
#include "util.h"
#include "../data/FluidParticle.h"

std::vector<Camera> cameras;

static void init(Vector3 cameraLocation, Vector3 cameraDirection, unsigned int frameWidth, unsigned int frameHeight) {
	Camera cam = Camera(cameraLocation, cameraDirection, frameWidth, frameHeight, cameras.size());
	cameras.emplace_back(cam);
}

static void addCamera(Vector3 cameraLocation, Vector3 cameraDirection, unsigned int frameWidth, unsigned int frameHeight) {
	Camera cam = Camera(cameraLocation, cameraDirection, frameWidth, frameHeight, cameras.size());
	cameras.emplace_back(cam);
}

static void debugRenderFrame(std::vector<FluidParticle> particles) {
	for (int i = 0; i < cameras.size(); i++) {
		cameras[i].volumeRenderFrame(convertSphParticles(particles), 1);
	}
}

static std::vector<FluidParticle> generateDebugParticles(int count) {
	std::vector<FluidParticle> particles;

	for (int i = 0; i < count; i++) {
		double x = double(rand() % 1000 - 500) / 100;
		double y = double(rand() % 1000 - 500) / 100;
		double z = double(rand() % 1000 - 500) / 100;

		FluidParticle f = FluidParticle(Vector3(x, y, z));
		particles.emplace_back(f);
	}
	return particles;

}