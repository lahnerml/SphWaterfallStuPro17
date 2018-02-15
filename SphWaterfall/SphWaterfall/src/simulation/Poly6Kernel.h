#pragma once
#define _USE_MATH_DEFINES

#include <math.h>

#include "../data/Vector3.h"
#include "ISphKernel.h"

//Quintic Wendlandkernel
class Poly6Kernel : public ISphKernel {
public:
	Poly6Kernel(double aH, double aQMax);
	double computeKernelValue(const Vector3&);
	Vector3 computeKernelGradientValue(const Vector3&);
	double getSmoothingLength();
private:
	double h;
	double qMax;
};