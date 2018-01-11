#include <iostream>

#include "mpi.h"

#include "cui/CUI.h"

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);

	int *buffer;
	buffer = (int*)malloc(sizeof(int));

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int worldSize;
	MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

	CUI cui = CUI::CUI();
	if (rank == 0) {
		cui.readCommands(buffer);
	}

	MPI_Bcast(buffer, 1, MPI_INT, 0, MPI_COMM_WORLD);
	printf("Hello world from processor rank %d out of %d processors, command is %d\n", rank, worldSize, buffer[0]);
	if (rank == 0) {
		system("pause");
		MPI_Wait;
	}

	MPI_Finalize();
}