#pragma once
#include "SphNeighbourSearch.h"

class SphNeighbourSearchFactory {
public:
	SphNeighbourSearchFactory();
	~SphNeighbourSearchFactory();

	static ISphNeighbourSearch* getInstance(int figureKey);
private:
};