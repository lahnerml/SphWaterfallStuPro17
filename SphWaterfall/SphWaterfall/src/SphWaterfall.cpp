#include "mpi.h"

#include "cui/CUI.h"
#include <thread>


void loadMesh() {
	printf("Command is loadMesh");
}

void generateParticle() {
	printf("command is particleGen");
}

void moveShutter() {
	printf("command is moveShutter");
}

void simulate() {
	printf("command is simulate");
}

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);

	std::thread cuiThread;
	int *buffer;
	buffer = (int*)malloc(sizeof(int));

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);


	if (rank == 0) {
		cuiThread = std::thread(CUI::readCommands, buffer);
	}

	MPI_Bcast(buffer, 1, MPI_INT, 0, MPI_COMM_WORLD);
	printf("Hello world from processor rank %d out of %d processors, buffer is %d\n", rank, world_size, buffer[0]);

	if (*buffer == 1) {
		loadMesh();
	}
	else if (*buffer == 2) {
		generateParticle();
	}
	else if (*buffer == 3) {
		moveShutter();
	}
	else if (*buffer == 4) {
		simulate();
	}

	if (rank == 0) {
		system("pause");
		MPI_Wait;
	}

	if (rank == 0) {
		cuiThread.join();
	}
	MPI_Finalize();


}
