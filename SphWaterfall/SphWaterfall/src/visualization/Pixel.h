#pragma once

#include <limits>

class Pixel {
public:
	Pixel();
	Pixel(unsigned short red, unsigned short green, unsigned short blue);

	unsigned short getRedValue();
	unsigned short getGreenValue();
	unsigned short getBlueValue();

	void setColor(unsigned short red, unsigned short green, unsigned short blue);
	void setRed(unsigned short red);
	void setGreen(unsigned short green);
	void setBlue(unsigned short blue);

	bool usesShader();
	void setShaderUsage(bool shaderUsage);

	void setBaseDepth(double baseDepth);
	double getBaseDepth();

private:
	unsigned short red;
	unsigned short green;
	unsigned short blue;
	double baseDepth;
	bool shaderUsage;
};