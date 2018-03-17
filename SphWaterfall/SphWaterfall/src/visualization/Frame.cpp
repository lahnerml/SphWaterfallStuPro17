#pragma once
#include "Frame.h"

Frame::Frame() {
	this->width = 0;
	this->height = 0;
}

Frame::Frame(unsigned int width, unsigned int height) {
	this->width = width;
	this->height = height;
	for (int i = 0; i < this->width*this->height; i++) {
		this->pixels.emplace_back(Pixel(0, 0, 0));
	}
}

Pixel Frame::getPixel(unsigned int x, unsigned int y) {
	if (x >= this->width || y >= this->height) return Pixel(255, 255, 0);
	return pixels.at(y*width + x);
}

void Frame::setPixel(unsigned int x, unsigned int y, Pixel pixel) {
	if (x >= this->width || y >= this->height) return;
	this->pixels[y*width + x] = pixel;
}

unsigned int Frame::getWidth() {
	return this->width;
}

unsigned Frame::getHeight() {
	return this->height;
}



