#pragma once

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

private:
	unsigned short red;
	unsigned short green;
	unsigned short blue;
};