#pragma once
#include "Pixel.h"

Pixel::Pixel() {
	this->red = 0;
	this->green = 0;
	this->blue = 0;
	this->shaderUsage = false;
	this->baseDepth = std::numeric_limits<unsigned int>::max();
}

Pixel::Pixel(unsigned short red, unsigned short green, unsigned short blue) {
	this->red = red > 255 ? 255 : (red < 0 ? 0 : red);
	this->green = green > 255 ? 255 : (green < 0 ? 0 : green);
	this->blue = blue > 255 ? 255 : (blue < 0 ? 0 : blue);
	this->shaderUsage = false;
	this->baseDepth = std::numeric_limits<unsigned int>::max();
}

void Pixel::setColor(unsigned short red, unsigned short green, unsigned short blue) {
	this->red = red > 255 ? 255 : (red < 0 ? 0 : red);
	this->green = green > 255 ? 255 : (green < 0 ? 0 : green);
	this->blue = blue > 255 ? 255 : (blue < 0 ? 0 : blue);
}

void Pixel::setRed(unsigned short red) {
	this->red = red > 255 ? 255 : (red < 0 ? 0 : red);
}

void Pixel::setGreen(unsigned short green) {
	this->green = green > 255 ? 255 : (green < 0 ? 0 : green);
}

void Pixel::setBlue(unsigned short blue) {
	this->blue = blue > 255 ? 255 : (blue < 0 ? 0 : blue);
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

bool Pixel::usesShader() {
	return this->shaderUsage;
}

void Pixel::setShaderUsage(bool shaderUsage) {
	this->shaderUsage = shaderUsage;
}

void Pixel::setBaseDepth(double baseDepth) {
	this->baseDepth = baseDepth;
}

double Pixel::getBaseDepth() {
	return this->baseDepth;
}
