#pragma once
#include <thread>

#include "mpi.h"

#include "cui/CUI.h"
#include "simulation/SimulationUtilities.h"
#include "data\FluidParticle.h"
#include "particleGen/StaticParticleGenerator.h"

CUI::AsyncCommand acmd;

void loadMesh(int rank, std::string fileName, Terrain& loadedMesh) {
	std::cout << "Loading Mesh: \"" << fileName << "\"" << std::endl;
	loadedMesh = TerrainParser::loadFromFile(fileName);
	cout << "Vertices: " << loadedMesh.getVertexCount() << " Faces: " << loadedMesh.getFaceCount() << endl;
}

void generateParticles(int rank, SphManager& sphManager, Terrain& loadedMesh) {
	StaticParticleGenerator gen;

	if (rank == 0)
		gen.sendAndGenerate(loadedMesh);
	else
		gen.receiveAndGenerate(sphManager);
}

void moveShutter(int rank) {
	cout << "command is moveShutter" << endl;
}

void simulate(int rank, SphManager& sph_manager) {
	cout << "command is simulate" << endl;

	if (rank == 1) {
		std::vector<SphParticle> particles;
		
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				for (int k = 0; k < 10; k++) {
					SphParticle particle = FluidParticle(Vector3(1000.0 + (i/10.0), 1000.0 + (j/10.0), 1000.0 + (k/10.0)));
					particles.push_back(particle);
					//cout << particle.position << endl;
				}
			}
		}

		//particles.push_back(FluidParticle(Vector3(100.9, 100.0, 100.0)));
		//particles.push_back(FluidParticle(Vector3(101.0, 100.0, 100.0)));
		//particles.push_back(FluidParticle(Vector3(101.1, 100.0, 100.0)));
		//particles.push_back(FluidParticle(Vector3(101.2, 100.0, 100.0)));

		sph_manager.add_particles(particles);
	}
	sph_manager.simulate();
}

void render(int rank) {
	cout << "command is render" << endl;
}

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// generate slave_comm and slave_comm_size for simulation
	int color = 1337;
	if (rank == 0) {
		color = MPI_UNDEFINED;
	}
	MPI_Comm_split(MPI_COMM_WORLD, color, 0, &slave_comm);
	if (rank != 0) {
		MPI_Comm_size(slave_comm, &slave_comm_size);
	}

	std::thread cuiThread;
	int cmd = CUI::ConsoleCommand::NONE;
	std::string cmdParam;

	SphManager sphManager = SphManager(Vector3(Q_MAX, Q_MAX, Q_MAX), 5, 1.0);
	Terrain loadedMesh;

	if (rank == 0) {
		cuiThread = std::thread(CUI::startCUI);
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
			if (rank != 0) {
				simulate(rank, sphManager);
				std::cout << "simulation finished from processor " << rank << " out of " << slave_comm_size << " processors" << std::endl;
			}
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

	return 0;
}
