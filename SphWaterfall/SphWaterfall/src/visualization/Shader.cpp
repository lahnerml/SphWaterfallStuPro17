#pragma once
#include "Shader.h"

Frame Shader::applyGaussianSmoothing(Frame f, int shaderDepth, int matrixSize) {
	std::cout << "Shading Frame\n";
	Frame newFrame;
	for (int k = 0; k < shaderDepth; k++) {
		newFrame = Frame(f.getWidth(), f.getHeight());

		for (int x = 0; x < f.getWidth(); x++) {
			for (int y = 0; y < f.getHeight(); y++) {
				if (f.getPixel(x, y).usesShader()) {
					unsigned short r = 0;
					unsigned short g = 0;
					unsigned short b = 0;

					unsigned short divider = 0;
					for (int i = -(matrixSize/2); i <= (matrixSize / 2); i++) {
						for (int j = -(matrixSize / 2); j <= (matrixSize / 2); j++) {
							if (x + i < 0 || x + i > f.getWidth() || y + j < 0 || y + j > f.getHeight()) continue;

							if (!f.getPixel(x + i, y + j).usesShader()) continue;
							
							r += f.getPixel(x + i, y + j).getRedValue();
							g += f.getPixel(x + i, y + j).getGreenValue();
							b += f.getPixel(x + i, y + j).getBlueValue();
							divider++;
						}
					}

					Pixel p = Pixel(r / divider, g / divider, b / divider);
					p.setShaderUsage(true);
					newFrame.setPixel(x, y, p);
				}
				else {
					newFrame.setPixel(x, y, f.getPixel(x, y));
				}
			}
		}
		f = newFrame;
	}
	std::cout << "Shading Done\n";

	return newFrame;
}
