#include <thread>

#include "mpi.h"

#include "cui/CUI.h"
#include "simulation/SimulationUtilities.h"
#include "data/SphParticle.h"
#include "particleGen/StaticParticleGenerator.h"
#include "visualization/VisualizationManager.h"
#include "data/ParticleIO.h"

Terrain& loadMesh(int rank, std::string fileName) {
	std::cout << "Loading Mesh: \"" << fileName << "\"" << std::endl;
	return TerrainParser::loadFromFile(fileName);
	
}

void generateParticles(int rank, SphManager& sphManager, Terrain& loadedMesh, SphParticle::ParticleType pType) {
	StaticParticleGenerator gen;

	if (rank == 0)
		gen.sendAndGenerate(loadedMesh, pType);
	else
		gen.receiveAndGenerate(sphManager, pType);
}

void createExport(int rank, SphManager& sph_manager) {
		int current_timestep = 1;
		unordered_map<int, vector<SphParticle>> export_map;

		int slave_comm_size;
		MPI_Comm_size(MPI_COMM_WORLD, &slave_comm_size);
		slave_comm_size--;

		while (current_timestep <= TIMESTEPS) {
			std::vector<SphParticle> all_particles_of_timestep;

			std::vector<int> number_of_incoming_particles = std::vector<int> (slave_comm_size);
			std::unordered_map<int, std::vector<SphParticle>> incoming_particles;

			MPI_Barrier(MPI_COMM_WORLD);

			for (int i = 0; i < slave_comm_size; i++) {
				MPI_Recv(&number_of_incoming_particles[i], 1, MPI_INT, i + 1, EXPORT_PARTICLES_NUMBER_TAG, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
				incoming_particles[i] = std::vector<SphParticle>(number_of_incoming_particles.at(i));
			}

			for (int i = 0; i < slave_comm_size; i++) {
				if (number_of_incoming_particles.at(i) != 0) {
					MPI_Recv(incoming_particles[i].data(), number_of_incoming_particles.at(i) * sizeof(SphParticle), MPI_BYTE, i + 1, EXPORT_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					all_particles_of_timestep.insert(all_particles_of_timestep.end(), incoming_particles.at(i).begin(), incoming_particles.at(i).end());
				}
			}

			export_map[current_timestep] = all_particles_of_timestep;
			ParticleIO::exportParticlesToVTK(all_particles_of_timestep, "vtk/particles", current_timestep);

			current_timestep++;
		}
		ParticleIO::exportParticles(export_map, "test.test");
		
		std::cout << "Done exporting" << std::endl;
}

void moveShutter(int rank, SphManager& sphManager, std::string shutterMoveParam)
{
	std::istringstream shutterMoveValues(shutterMoveParam);
	int shutterMoveFrame;
	shutterMoveValues >> shutterMoveFrame;
	std::cout << "Shutter opening at frame: " << shutterMoveFrame << std::endl;
}

void simulate(int rank, SphManager& sph_manager) {
	cout << "command is simulate" << endl;

	if (rank == 1) {
		std::vector<SphParticle> particles;
		
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 20; j++) {
				for (int k = 0; k < 20; k++) {
					//SphParticle particle = SphParticle(Vector3(1000.0 + (i/10.0), 1000.0 + (j/10.0), 1000.0 + (k/10.0)));
					SphParticle particle = SphParticle(Vector3(3.0 + i, 3.0 + j, 3.0 + k));
					particles.push_back(particle);
					//cout << particle.position << endl;
				}
			}
		}
		

		//particles.push_back(SphParticle(Vector3(10.0, 10.0, 10.0)));
		sph_manager.add_particles(particles);
	}
	sph_manager.simulate();
}


void render(int rank, Terrain loadedMesh, Terrain loadedShutter, int shutterTime) {

	if (rank == 0) {
		cout << "Rendering in progress..." << endl;
	}
	VisualizationManager::importTerrain(loadedMesh, true);
	VisualizationManager::importTerrain(loadedShutter, false);

	VisualizationManager::init(Vector3(10, 5, -20), 200, 200);
	//VisualizationManager::renderFrames("test.test");
	VisualizationManager::renderFramesDistributed("test.test", rank);

	MPI_Barrier(MPI_COMM_WORLD);

	if (rank == 0) {
		cout << "Rendering complete" << endl; 
	}
}

void addSource(int rank, SphManager& sphManager, std::string sourcePosParam)
{
	std::istringstream sourcePosValues(sourcePosParam);
	double x, y, z;
	sourcePosValues >> x;
	sourcePosValues >> y;
	sourcePosValues >> z;
	Vector3 sourcePos = Vector3(x, y, z);
	std::cout << "New source: " << sourcePos << std::endl;
}

void addSink(int rank, SphManager& sphManager, std::string sinkHeightParam)
{
	std::istringstream sinkHeightValues(sinkHeightParam);
	double sinkHeight;
	sinkHeightValues >> sinkHeight;
	std::cout << "New sink height: " << sinkHeight << std::endl;
}

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

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
	int command = AsyncCommand::NONE;
	std::string commandParameter;

	SphManager sphManager = SphManager(Vector3(DOMAIN_DIMENSION, DOMAIN_DIMENSION, DOMAIN_DIMENSION), TIMESTEPS, 0.03);
	Terrain loadedMesh;
	Terrain loadedShutter;

	if (rank == 0) {
		cuiThread = std::thread(CUI::startCUI);
	}

	while (command != AsyncCommand::EXIT) {
		if (rank == 0) {
			command = CUI::asyncCommand.readCommand(commandParameter);
		}
		MPI_Bcast(&command, 1, MPI_INT, 0, MPI_COMM_WORLD);

		//Execute console input
		switch (command)
		{
		case AsyncCommand::LOAD_MESH:
			if (rank == 0) {
				loadedMesh = loadMesh(rank, commandParameter);
				cout << "Mesh loaded." << endl;
				cout << "Vertices: " << loadedMesh.getVertexCount() << " Faces: " << loadedMesh.getFaceCount() << endl;
				CUI::printInputMessage();
			}
			MPI_Barrier(MPI_COMM_WORLD);
			break;
		case AsyncCommand::LOAD_SHUTTER:
			loadedShutter = loadMesh(rank, commandParameter);
			MPI_Barrier(MPI_COMM_WORLD);
			if (rank == 0) {
				cout << "Mesh loaded." << endl;
				cout << "Vertices: " << loadedShutter.getVertexCount() << " Faces: " << loadedShutter.getFaceCount() << endl;
				CUI::printInputMessage();
			}
			break;
		case AsyncCommand::GENERATE_PARTICLES:
			generateParticles(rank, sphManager, loadedMesh, SphParticle::ParticleType::STATIC);
			generateParticles(rank, sphManager, loadedShutter, SphParticle::ParticleType::SHUTTER);
			MPI_Barrier(MPI_COMM_WORLD);
			if (rank == 0) {
				cout << "Static particles generated." << endl;
				CUI::printInputMessage();
			}
			break;
		case AsyncCommand::MOVE_SHUTTER:
			moveShutter(rank, sphManager, commandParameter);
			MPI_Barrier(MPI_COMM_WORLD);
			if (rank == 0) {
				cout << "Stutter move set." << endl;
				CUI::printInputMessage();
			}
			break;
		case AsyncCommand::SIMULATE:
			if (rank != 0) {
				simulate(rank, sphManager);
			}
			else{
				createExport(rank, sphManager);
			}
			MPI_Barrier(MPI_COMM_WORLD);
			if (rank == 0) {
				cout << "Simulation finished." << endl;
				CUI::printInputMessage();
			}
			break;
		case AsyncCommand::RENDER:
			render(rank, loadedMesh, loadedShutter, 0);
			MPI_Barrier(MPI_COMM_WORLD);
			if (rank == 0) {
				cout << "Rendering finished." << endl;
				CUI::printInputMessage();
			}
		case AsyncCommand::ADD_SOURCE:
			addSource(rank, sphManager, commandParameter);
			MPI_Barrier(MPI_COMM_WORLD);
			if (rank == 0) {
				cout << "Added source." << endl;
				CUI::printInputMessage();
			}
			break;
		case AsyncCommand::ADD_SINK:
			addSink(rank, sphManager, commandParameter);
			MPI_Barrier(MPI_COMM_WORLD);
			if (rank == 0) {
				cout << "Added sink." << endl;
				CUI::printInputMessage();
			}
			break;
		default:
			break;
		}
	}

	if (rank == 0) {
		cuiThread.join();
		MPI_Wait;
	}

	MPI_Finalize();

	return 0;
}
