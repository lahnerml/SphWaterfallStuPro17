#include "../data/ISphParticle.h"
#include "Camera.h"
#include <map>
#include <pair>
#include <vector>

const std::map<long, std::vector<ISphParticle>> frameMap;
long lowestFrame = 0;
const std::vector<Camera> cameras;
const double MASS = 2;

bool rawFrameIn(std::vector<ISphParticle> &particles, long frameID) {
	std::pair<auto, auto> result = map.insert(std::pair<long, std::vector<ISphParticle>>(frameID, particles));
	if (frameID < lowestFrame) {
		lowestFrame = frameID;
	}
	return result.second;
}

bool registerCamera(Camera camera) {
	cameras.emplace_back(camera);
}

long findNextFrame() {
	lowestFrame += 1;
	try {
		frameMap.at(lowestFrame)
	}
	catch {
		return findNextFrame()
	}
	return lowestFrame;
}

void renderNextFrame() {
	//Implement Condition if we wanna wait and not do everything parallel
	
	std::vector<auto> nextParticles = frameMap.at(findNextFrame());
	for (int i = 0; i < cameras.size; i++) {
		cameras.at(i).debugRenderFrame(convertSphParticles(nextParticles));
	}
}


std::vector<DebugObject> convertSphParticles(std::vector<ISphParticle> particles) {
	std::vector<DebugObject> output;
	for (int i = 0; i < particles.size; i++) {
		output.emplace_back(DebugObject(particles[i].position, MASS));
	}
}