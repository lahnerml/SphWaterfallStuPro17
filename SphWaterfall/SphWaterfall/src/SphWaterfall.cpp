#include "mpi.h"

#include "cui/CUI.h"
#include "cui/CommandHandler.h"

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);

	int mpi_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

	if (mpi_rank == 0) {
		CUI cui = CUI();
		cui.start();
		MPI_Barrier(MPI_COMM_WORLD);
	}
	else {
		CommandHandler command_handler = CommandHandler(mpi_rank);
		command_handler.start();
		MPI_Barrier(MPI_COMM_WORLD);
	}

	std::cout << "test " << mpi_rank << std::endl;

	MPI_Finalize();

	return 0;
}
