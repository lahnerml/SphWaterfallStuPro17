#pragma once
#include "SphKernel.h"

SphKernel::SphKernel() {

}

SphKernel::~SphKernel() {

}

Vector3 SphKernel::computeKernelValue(Vector3) {
	return Vector3(); //placeholder
}

Vector3 SphKernel::computeKernelGradientValue(Vector3) {
	return Vector3(); //placeholder
}

double SphKernel::getSmoothingLength() {
	return 0.0; //placeholder
}
