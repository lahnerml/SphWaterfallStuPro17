#pragma once
#include "SphNeighbourSearchFactory.h"

SphNeighbourSearchFactory::SphNeighbourSearchFactory() {

}

SphNeighbourSearchFactory::~SphNeighbourSearchFactory() {

}

static ISphNeighbourSearch* getInstance(int key)
{
	ISphNeighbourSearch* produced_neighbour_search;

	switch (key)
	{
	default:
		produced_neighbour_search = new SphNeighbourSearch();
		break;
	}

	return produced_neighbour_search;
}