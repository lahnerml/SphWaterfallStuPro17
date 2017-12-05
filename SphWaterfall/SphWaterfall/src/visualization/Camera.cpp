#pragma once
#include "Camera.h"

Camera::Camera(Vector3 location, Vector3 direction, unsigned int width, unsigned int height) {
	this->width = width;
	this->height = height;

	this->location = location;
	this->direction = direction;
}

void Camera::debugRenderFrame(std::vector<ISphParticle> particles) {
	//TODO: render frame in debug rendering
}

void Camera::renderFrame(std::vector<ISphParticle> particles) {
	//TODO: implement rendering algorithm
}

void Camera::mergeFramesAndFlushVideo(std::string file) {
	//TODO: merge all Frames to a video
}

Vector3 Camera::getLocation() {
	return this->location;
}

Vector3 Camera::getDirection() {
	return this->direction;
}

Frame Camera::getFrame(unsigned int frameID) {
	if (frames.size >= frameID) {
		return Frame();
		// ^ TODO changend by Magnus
	}
	return frames.at(frameID);
}

//TODO: Impelement Frame Iterator?