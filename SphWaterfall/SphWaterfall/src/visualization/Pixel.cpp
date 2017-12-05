#pragma once
#include "Pixel.h"

Pixel::Pixel() {
	this->red = 0;
	this->green = 0;
	this->blue = 0;
}

Pixel::Pixel(unsigned short red, unsigned short green, unsigned short blue) {
	this->red = red;
	this->green = green;
	this->blue = blue;
}

void Pixel::setColor(unsigned short red, unsigned short green, unsigned short blue) {
	this->red = red;
	this->green = green;
	this->blue = blue;
}

void Pixel::setRed(unsigned short red) {
	this->red = red;
}

void Pixel::setGreen(unsigned short green) {
	this->green = green;
}

void Pixel::setBlue(unsigned short blue) {
	this->blue = blue;
}

unsigned short Pixel::getRedValue() {
	return this->red;
}

unsigned short Pixel::getGreenValue() {
	return this->green;
}

unsigned short Pixel::getBlueValue() {
	return this->blue;
}
