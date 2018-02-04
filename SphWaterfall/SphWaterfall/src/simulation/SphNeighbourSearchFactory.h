#pragma once
#include "SphNeighbourSearch.h"

class SphNeighbourSearchFactory {
public:
	SphNeighbourSearchFactory();
	~SphNeighbourSearchFactory();

	static ISphNeighbourSearch* getInstance(int key, double q_max);
private:
};