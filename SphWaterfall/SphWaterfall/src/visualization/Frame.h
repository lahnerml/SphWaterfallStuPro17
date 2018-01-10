#pragma once
#include "Pixel.h"
#include <vector>

class Frame {
	public:
		Frame(); //To genereate placeholder
		Frame(unsigned int width, unsigned int height, long frameID);
		
		long getFrameID();
		Pixel getPixel(unsigned int x, unsigned int y);
		void setPixel(unsigned int x, unsigned int y, Pixel pixel);
				
	private:
		unsigned int width;
		unsigned int height;
		std::vector<Pixel> pixels;

		long frameID;
};