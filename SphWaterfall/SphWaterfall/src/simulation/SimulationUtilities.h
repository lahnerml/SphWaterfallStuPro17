#pragma once
#include "mpi.h"
#include "../data/Vector3.h"
#include "../data/SphParticle.h"

// Influence radius
#define Q_MAX 1.2
// Smoothing radius (Influence radius / 4)
#define H (Q_MAX / 4)
// Number of simulation timesteps
#define TIMESTEPS 301

// Sph Manager tags
#define META_TAG 0
#define EXCHANGE_TAG 1
#define EXPORT_TAG 2

namespace SimulationUtilities {
	int hash(const Vector3&);
	Vector3 unhash(const int&);	
	int computeProcessID(const Vector3 position, const Vector3 domain_dimension, const int comm_size);
	int computeProcessID(const int domain_id, const int comm_size);
	int computeDomainID(const Vector3 position, const Vector3 domain_dimension);

	extern MPI_Comm slave_comm;
	extern int slave_comm_size;
}