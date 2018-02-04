#pragma once
#include "WendlandKernel.h"

class SphKernelFactory {
public:
	SphKernelFactory();
	~SphKernelFactory();

	static ISphKernel* getInstance(int key, double h, double q_max);
private:
};