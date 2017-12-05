#include "Camera.h"

std::vector<Camera> cameras;

void init(Vector3 cameraLocation, Vector3 cameraDirection, unsigned int frameWidth, unsigned int frameHeight) {
	Camera mainCam = Camera(cameraLocation, cameraDirection, frameWidth, frameHeight);
	cameras.emplace_back(mainCam);
}

void debugRenderFrame(std::vector<ISphParticle> particles) {
	//TODO: implement debug rendering
}