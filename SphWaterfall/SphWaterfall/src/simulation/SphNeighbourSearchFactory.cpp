#pragma once
#include "SphNeighbourSearchFactory.h"

SphNeighbourSearchFactory::SphNeighbourSearchFactory() {

}

SphNeighbourSearchFactory::~SphNeighbourSearchFactory() {

}

ISphNeighbourSearch* SphNeighbourSearchFactory::getInstance(int key, double q_max)
{
	ISphNeighbourSearch* produced_neighbour_search;

	switch (key)
	{
	default:
		produced_neighbour_search = new SphNeighbourSearch(q_max);
		break;
	}

	return produced_neighbour_search;
}