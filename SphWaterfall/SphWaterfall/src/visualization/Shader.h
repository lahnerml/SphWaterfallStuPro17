#pragma once
#include "Frame.h"

class Shader {
public:
	static Frame applyGaussianSmoothing(Frame f, int shaderDepth, int matrixSize);
};