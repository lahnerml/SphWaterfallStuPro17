#pragma once
#include "../data/Vector3.h";
#include "../data/SphParticle.h";

namespace SimulationUtilities {
	int hash(const Vector3&);
	Vector3 unhash(const int&);	
	int computeProcessID(const Vector3 position, const Vector3 domain_dimension, const int comm_size);
	int computeDomainID(const Vector3 position, const Vector3 domain_dimension);

}