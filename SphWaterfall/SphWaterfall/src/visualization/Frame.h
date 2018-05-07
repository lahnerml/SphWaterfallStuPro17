#pragma once
#include <mpi.h>
#include "Pixel.h"
#include <vector>

//Frame Klasse; enthält einen Array von Pixel um ein Bild der Größe N x M zu halten
class Frame {
	public:
		Frame(); //To genereate placeholder
		Frame(unsigned int width, unsigned int height);
		
		Pixel& getPixel(unsigned int x, unsigned int y);
		void setPixel(unsigned int x, unsigned int y, Pixel pixel);

		unsigned int getWidth();
		unsigned int getHeight();

		//MPI Funktionalitaet zum Verteilen auf Prozessoren
		static void MpiSendFrame(Frame frame, int dest);
		static Frame MpiReceiveFrame(int source);
				
	private:
		unsigned int width;
		unsigned int height;
		std::vector<Pixel> pixels;
};