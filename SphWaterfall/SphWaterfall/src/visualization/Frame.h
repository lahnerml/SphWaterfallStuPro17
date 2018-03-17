#pragma once
#include "Pixel.h"
#include <vector>

class Frame {
	public:
		Frame(); //To genereate placeholder
		Frame(unsigned int width, unsigned int height);
		
		Pixel getPixel(unsigned int x, unsigned int y);
		void setPixel(unsigned int x, unsigned int y, Pixel pixel);

		unsigned int getWidth();
		unsigned int getHeight();
				
	private:
		unsigned int width;
		unsigned int height;
		std::vector<Pixel> pixels;
};