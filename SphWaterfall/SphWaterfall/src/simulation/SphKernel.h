#pragma once
#include "ISphKernel.h"

class SphKernel : public ISphKernel{
public:
	SphKernel();
	~SphKernel();

	Vector3 computeKernelValue(Vector3);
	Vector3 computeKernelGradientValue(Vector3);
	double getSmoothingLength();
private:
};