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
	}
	else {
		CommandHandler command_handler = CommandHandler(mpi_rank);
		command_handler.start();
	}

	MPI_Finalize();

	return 0;
}
