#pragma once
#include "../data/Vector3.h"
#include "ISphKernel.h"

//Quintic Wendlandkernel
class WendlandKernel : public ISphKernel {
public:
	double computeKernelValue(const Vector3&);
	Vector3 computeKernelGradientValue(const Vector3&);
	double getSmoothingLength();
private:
	double h;
	double qMax;
};