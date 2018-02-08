#pragma once
#include <thread>

#include "mpi.h"

#include "cui/CUI.h"
//#include "simulation/SimulationUtilities.h"
#include "data\FluidParticle.h"

CUI::AsyncCommand acmd;

void loadMesh(int rank, std::string fileName, Terrain& loadedMesh) {
	std::cout << "Loading Mesh: \"" << fileName << "\"" << std::endl;
	loadedMesh = TerrainParser::loadFromFile(fileName);
	cout << "Vertices: " << loadedMesh.getVertexCount() << " Faces: " << loadedMesh.getFaceCount() << endl;
}

void generateParticles(int rank, SphManager& sphManager, Terrain& loadedMesh) {
	cout << "command is particleGen" << endl;
}

void moveShutter(int rank) {
	cout << "command is moveShutter" << endl;
}

void simulate(int rank, SphManager& sphManager) {
	cout << "command is simulate" << endl;

	
	std::vector<SphParticle> particles;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			for (int k = 0; k < 5; k++) {
				SphParticle particle = FluidParticle(Vector3(i, j, k), Vector3(), 1.0);
				particles.push_back(particle);
				//cout << particle.position << endl;
			}
		}
	}
	sphManager.add_particles(particles);
	sphManager.simulate();
}

void render(int rank) {
	cout << "command is render" << endl;
}

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);

	std::thread cuiThread;
	int cmd = CUI::ConsoleCommand::NONE;
	std::string cmdParam;

	SphManager sphManager = SphManager(Vector3(2, 2, 2), 5, 1);
	Terrain loadedMesh;

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	int color = 1337;
	if (rank == 0) {
		color = MPI_UNDEFINED;
		cuiThread = std::thread(CUI::startCUI);
	}

	MPI_Comm_split(MPI_COMM_WORLD, color, 0, &slave_comm);
	if (rank != 0) {
		MPI_Comm_size(slave_comm, &slave_comm_size);
	}

	while (cmd != CUI::ConsoleCommand::EXIT) {
		if (rank == 0) {
			cmd = CUI::acmd.aReadCmd(cmdParam);
		}
		MPI_Bcast(&cmd, 1, MPI_INT, 0, MPI_COMM_WORLD);


		//Execute console input
		switch (cmd)
		{
		case CUI::ConsoleCommand::LOAD_MESH:
			loadMesh(rank, cmdParam, loadedMesh);
			std::cout << "mesh loading finished from processor " << rank << " out of " << slave_comm_size << " processors" << std::endl;
			break;
		case CUI::ConsoleCommand::GENERATE_PARTICLES:
			generateParticles(rank, sphManager, loadedMesh);
			std::cout << "particle generation finished from processor " << rank << " out of " << slave_comm_size << " processors" << std::endl;
			break;
		case CUI::ConsoleCommand::MOVE_SHUTTER:
			moveShutter(rank);
			std::cout << "moveing shutter finished from processor " << rank << " out of " << slave_comm_size << " processors" << std::endl;
		case CUI::ConsoleCommand::SIMULATE:
			simulate(rank, sphManager);
			std::cout << "simulation finished from processor " << rank << " out of " << slave_comm_size << " processors" << std::endl;
		case CUI::ConsoleCommand::RENDER:
			render(rank);
			std::cout << "rendering finished from processor " << rank << " out of " << slave_comm_size << " processors" << std::endl;
		default:
			break;
		}

		if (cmd != CUI::ConsoleCommand::EXIT && cmd != CUI::ConsoleCommand::NONE)
			CUI::acmd.aWriteCmd(CUI::ConsoleCommand::NONE);
	}

	if (rank == 0) {
		cuiThread.join();
	}

	if (rank == 0) {
		system("pause");
		MPI_Wait;
	}

	MPI_Finalize();
}
