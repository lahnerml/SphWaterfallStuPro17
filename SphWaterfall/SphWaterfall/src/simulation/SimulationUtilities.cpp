#pragma once
#include "SimulationUtilities.h"

namespace SimulationUtilities {

	int SimulationUtilities::hash(const Vector3& vector) {
		int x, y, z;
		x = static_cast<int>(vector.x);
		y = static_cast<int>(vector.y);
		z = static_cast<int>(vector.z);
		y = y << 10;
		z = z << 20;
		return (x + y + z);
	}

	Vector3 SimulationUtilities::unhash(const int& unique_id) {
		int z = unique_id >> 20;
		int y = (unique_id - (z << 20)) >> 10;
		int x = unique_id - (z << 20) - (y << 10);
		return Vector3(x, y, z);
	}

	int SimulationUtilities::computeProcessID(const Vector3 position, const Vector3 domain_dimension) {
		Vector3 targetDomainCoords = (position / domain_dimension).roundDownward();
		//std::cout << targetDomainCoords << std::endl; // debug output
		return hash(targetDomainCoords);
	}

	int SimulationUtilities::computeDomainID(const Vector3 position, const Vector3 domain_dimension, const int comm_size) {
		return abs(computeProcessID(position, domain_dimension) % comm_size);
	}
}
