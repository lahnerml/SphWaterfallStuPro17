#include <thread>

#include "mpi.h"

#include "cui/CUI.h"
#include "data\FluidParticle.h"

void loadMesh() {
	printf("Command is loadMesh\n");
}

void generateParticle() {
	printf("command is particleGen\n");
}

void moveShutter() {
	printf("command is moveShutter\n");
}

void simulate() {
	SphManager sph_manager = SphManager(Vector3(10, 10, 10), 3, 1);
	std::vector<SphParticle> particles;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				SphParticle particle = FluidParticle(Vector3(i, j, k), Vector3(), 1.0);
				particles.push_back(particle);
				//std::cout << particle.position << std::endl;
			}
		}
	}
	sph_manager.add_particles(particles);
	sph_manager.simulate();
	printf("command is simulate\n");
}

void render() {
	printf("command is render\n");
}

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);

	bool exit_programm = false;
	std::thread cuiThread;
	int *command_buffer;
	command_buffer = (int*)malloc(sizeof(int));

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	while (!exit_programm) {
		if (rank == 0) {
			cuiThread = std::thread(CUI::readCommand, command_buffer);
			cuiThread.join();
		}

		MPI_Bcast(command_buffer, 1, MPI_INT, 0, MPI_COMM_WORLD);

		if (*command_buffer == 0) {
			exit_programm = true;
		} 
		else if (rank != 0) {
			printf("Hello world from processor rank %d out of %d processors, buffer is %d\n", rank, world_size, command_buffer[0]);

			if (*command_buffer == 1) {
				loadMesh();
			} 
			else if (*command_buffer == 2) {
				generateParticle();
			} 
			else if (*command_buffer == 3) {
				moveShutter();
			} 
			else if (*command_buffer == 4) {
				simulate();
			} 
			else if (*command_buffer == 5) {
				render();
			}
		}
	}

	if (rank == 0) {
		system("pause");
		MPI_Wait;
	}

	MPI_Finalize();
}
