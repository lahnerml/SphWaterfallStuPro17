#pragma once
#include "WendlandKernel.h"

class SphKernelFactory {
public:
	SphKernelFactory();
	~SphKernelFactory();

	static ISphKernel* getInstance(int key);
private:
};