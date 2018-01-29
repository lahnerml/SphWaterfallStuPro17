#pragma once
#include "SphKernelFactory.h"

SphKernelFactory::SphKernelFactory() {

}

SphKernelFactory::~SphKernelFactory() {

}

static ISphKernel* getInstance(int figureKey)
{
	ISphKernel* produced_kernel;

	switch (figureKey)
	{
	default:
		produced_kernel = new WendlandKernel(0, 0);
		break;
	}

	return produced_kernel;
}