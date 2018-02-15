#pragma once
#include "WendlandKernel.h"
#include "Poly6Kernel.h";
#include "SimulationUtilities.h";

class SphKernelFactory {
public:
	SphKernelFactory();
	~SphKernelFactory();

	static ISphKernel* getInstance(int key);
private:
};