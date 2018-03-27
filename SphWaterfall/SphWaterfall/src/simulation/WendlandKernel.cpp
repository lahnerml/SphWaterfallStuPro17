#pragma once
#include "WendlandKernel.h"

WendlandKernel::WendlandKernel(double aH, double aQMax) :
	h(aH),
	qMax(aQMax)
{
	kernel_value_const = (256.0 * M_PI * pow(h, 3.0));
	kernel_gradient_const = (256.0 * M_PI * pow(h, 4.0));
}

double WendlandKernel::computeKernelValue(const Vector3& r) {
	double q = r.length() / h;
	if (q >= qMax) {
		return 0.0;
	}

	return 12.0 / kernel_value_const * pow(2.0 - q, 4.0) * (2.0 * q + 1.0);
}

Vector3 WendlandKernel::computeKernelGradientValue(const Vector3& r) {
	double q = r.length() / h;
	if ((q == 0) || (q >= qMax)) {
		return Vector3();
	}

	return 21.0 / kernel_gradient_const * (-10.0 * q) * pow(2.0 - q, 3.0) * r.normalize();
}

double WendlandKernel::getSmoothingLength() {
	return h;
}
