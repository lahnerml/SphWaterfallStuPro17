#pragma once
#include <thread>

#include "mpi.h"

#include "cui/CUI.h"
#include "simulation/SimulationUtilities.h"
#include "data\SphParticle.h"
#include "particleGen/StaticParticleGenerator.h"
#include "visualization/VisualizationManager.h"
#include "data\ParticleIO.h"

CUI::AsyncCommand acmd;

void loadMesh(int rank, std::string fileName, Terrain& loadedMesh) {
	std::cout << "Loading Mesh: \"" << fileName << "\"" << std::endl;
	loadedMesh = TerrainParser::loadFromFile(fileName);
	cout << "Vertices: " << loadedMesh.getVertexCount() << " Faces: " << loadedMesh.getFaceCount() << endl;
	VisualizationManager::importTerrain(loadedMesh, true);
	VisualizationManager::importTerrain(loadedMesh, false);
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

void createExport(int rank, SphManager& sph_manager) {
		int current_timestep = 1;
		int current_frame = 1;

		unordered_map<int, vector<SphParticle>> export_map;

		while (current_timestep <= TIMESTEPS) {
			MPI_Barrier(MPI_COMM_WORLD);

			std::unordered_map<int, std::vector<SphParticle>> allParticles;

			std::vector<SphParticle> allParticlesOfTimestep;
			//std::pair <int, std::vector<SphParticle>> incomingPair;

			// receive until there is nothing left
			int flag;
			MPI_Status status;
			MPI_Iprobe(MPI_ANY_SOURCE, 99, MPI_COMM_WORLD, &flag, &status);

			int count = 0;
			int source;

			while (flag) {
				source = status.MPI_SOURCE;
				MPI_Get_count(&status, MPI_BYTE, &count);
				std::vector<SphParticle> incomingParticles = std::vector<SphParticle>(count / sizeof(SphParticle));

				MPI_Recv(incomingParticles.data(), count, MPI_BYTE, source, 99, MPI_COMM_WORLD, &status);
				allParticlesOfTimestep.insert(allParticlesOfTimestep.end(), incomingParticles.begin(), incomingParticles.end());

				// for (auto particle : allParticlesOfTimestep) { std::cout << current_timestep << " received in export: " << particle << std::endl; } // debug

				// next message
				MPI_Iprobe(MPI_ANY_SOURCE, 99, MPI_COMM_WORLD, &flag, &status);
			}
			
			export_map[current_frame] = allParticlesOfTimestep;

			current_frame++;
			current_timestep += 20;
		}
		ParticleIO::exportParticles(export_map, "test.test");
		
		std::cout << "Done exporting" << std::endl;
}

void simulate(int rank, SphManager& sph_manager) {
	cout << "command is simulate" << endl;

	if (rank == 1) {
		std::vector<SphParticle> particles;
		
		for (int i = 0; i < 15; i++) {
			for (int j = 0; j < 15; j++) {
				for (int k = 0; k < 15; k++) {
					//SphParticle particle = SphParticle(Vector3(1000.0 + (i/10.0), 1000.0 + (j/10.0), 1000.0 + (k/10.0)));
					SphParticle particle = SphParticle(Vector3(3.0 + i, 3.0 + j, 3.0 + k));
					particles.push_back(particle);
					//cout << particle.position << endl;
				}
			}
		}

		sph_manager.add_particles(particles);
	}
	sph_manager.simulate();
}


void render(int rank) {

	if (rank == 0) {
		cout << "Rendering in progress..." << endl;

	}
	VisualizationManager::init(Vector3(0, -20, 30), 800, 600);
	//VisualizationManager::renderFrames("test.test");
	VisualizationManager::renderFramesDistributed("test.test", rank);

	MPI_Barrier(MPI_COMM_WORLD);

	if (rank == 0) {
		cout << "Rendering complete" << endl; 
	}
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

	SphManager sphManager = SphManager(Vector3(Q_MAX, Q_MAX, Q_MAX), TIMESTEPS, 0.01);
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
			break;
		case CUI::ConsoleCommand::SIMULATE:
			if (rank != 0) {
				simulate(rank, sphManager);
				std::cout << "simulation finished from processor " << rank << " out of " << slave_comm_size << " processors" << std::endl;
			}
			else{
				createExport(rank, sphManager);
			}
			break;
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
