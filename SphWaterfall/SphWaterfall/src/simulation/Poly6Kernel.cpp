#pragma once
#include "Poly6Kernel.h"

Poly6Kernel::Poly6Kernel(double aH, double aQMax) :
	h(aH),
	qMax(aQMax)
{

}

double Poly6Kernel::computeKernelValue(const Vector3& r) {

	double length_of_distance_square = r.calculateSquareMagnitude();
	return (315.0 / (64.0 * M_PI *  pow(h, 9.0))) * 0.0;
}

Vector3 Poly6Kernel::computeKernelGradientValue(const Vector3& r) {

	return Vector3();
}

double Poly6Kernel::getSmoothingLength() {
	return h;
}
