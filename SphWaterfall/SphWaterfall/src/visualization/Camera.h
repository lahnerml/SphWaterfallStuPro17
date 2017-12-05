#pragma once
#include "Frame.h"
#include "Vector3.h"
#include "ISphParticle.h"
#include <vector>
#include <string>

class Camera {
public:
	Camera(Vector3 location, Vector3 direction, unsigned int width, unsigned int height);

	void debugRenderFrame(std::vector<ISphParticle> particles);

	void renderFrame(std::vector<ISphParticle> particles); //Vector is only a placeholder here as the data structure isnt decided yet

	void mergeFramesAndFlushVideo(std::string file);

	Vector3 getLocation();
	Vector3 getDirection();

private:
	Vector3 location;
	Vector3 direction;
	unsigned int width;
	unsigned int height;
	std::vector<Frame> frames;

	Frame getFrame(unsigned int frameID);
};