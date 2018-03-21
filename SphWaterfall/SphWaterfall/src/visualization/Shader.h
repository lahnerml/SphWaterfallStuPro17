#pragma once
#include <iostream>
#include "Frame.h"

class Shader {
public:
	static Frame applyGaussianSmoothing(Frame f, int shaderDepth, int matrixSize);
};