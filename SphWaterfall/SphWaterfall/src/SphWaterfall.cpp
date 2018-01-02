#include <iostream>
#include "mpi.h"
#include <vector>
#include <memory>

int main(int argc, char** argv) {
	// Initialize the MPI environment
	MPI_Init(&argc, &argv);

	// Get the number of processes
	int worldSize;
	MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

	// Get the rank of the process
	int worldRank;
	MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);

	// Get the name of the processor
	char processorName[MPI_MAX_PROCESSOR_NAME];
	int nameLen;
	MPI_Get_processor_name(processorName, &nameLen);

	double *buffer;
	buffer = (double*)malloc(sizeof(double));
	if (worldRank == 0) {
		buffer[0] = 7.0;
	}

	MPI_Bcast(buffer, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	// Print off a hello world message
	printf("Hello world from processor %s, rank %d"
		" out of %d processors, my result is %f\n",
		processorName, worldRank, worldSize, buffer[0] * worldRank);

	

	MPI_Wait;

	// Finalize the MPI environment.
	MPI_Finalize();

}