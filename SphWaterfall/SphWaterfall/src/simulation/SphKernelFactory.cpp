#pragma once
#include "SphKernelFactory.h"

SphKernelFactory::SphKernelFactory() {

}

SphKernelFactory::~SphKernelFactory() {

}

ISphKernel* SphKernelFactory::getInstance(int key, double h, double q_max)
{
	ISphKernel* produced_kernel;

	switch (key)
	{
	default:
		produced_kernel = new WendlandKernel(h, q_max);
		break;
	}

	return produced_kernel;
}