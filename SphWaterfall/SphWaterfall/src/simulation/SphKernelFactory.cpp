#pragma once
#include "SphKernelFactory.h"

SphKernelFactory::SphKernelFactory() {

}

SphKernelFactory::~SphKernelFactory() {

}

ISphKernel* SphKernelFactory::getInstance(int key)
{
	ISphKernel* produced_kernel;

	switch (key)
	{
	case (1): {
		produced_kernel = new Poly6Kernel(H, Q_MAX);
		break;
	}
	case (2): {
		produced_kernel = new WendlandKernel(H, Q_MAX);
		break;
	}
	default:
		produced_kernel = new WendlandKernel(H, Q_MAX);
		break;
	}

	return produced_kernel;
}