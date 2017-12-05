#pragma once
#include "Frame.h"
#include "Pixel.h"

Frame::Frame() {
	this->width = 0;
	this->height = 0;
}

Frame::Frame(unsigned int width, unsigned int height) {
	this->width = width;
	this->height = height;
}

Pixel Frame::getPixel(unsigned int x, unsigned int y) {
	if (pixels.size >= y*height + x) {
		return Pixel();
		// ^ TODO changend by Magnus
	}
	return pixels.at(y*height + x);
}

