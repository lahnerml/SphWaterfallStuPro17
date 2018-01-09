#pragma once
#include "../data/Vector3.h"

class ISphKernel {
public:
	virtual Vector3 computeKernelValue(Vector3) = 0;
	virtual Vector3 computeKernelGradientValue(Vector3) = 0;
	virtual double getSmoothingLength() = 0;
};