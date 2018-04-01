#include "CommandHandler.h"

CommandHandler::CommandHandler(int mpi_rank) : 
	mpi_rank(mpi_rank),
	sph_manager(SphManager(Vector3(DOMAIN_DIMENSION, DOMAIN_DIMENSION, DOMAIN_DIMENSION), TIMESTEPS, 0.03)){
	// generate slave_comm and slave_comm_size for simulation
	int color = 1337;
	if (mpi_rank == 0) {
		color = MPI_UNDEFINED;
	}
	MPI_Comm_split(MPI_COMM_WORLD, color, 0, &slave_comm);
	if (mpi_rank != 0) {
		MPI_Comm_size(slave_comm, &slave_comm_size);
	}
}

void CommandHandler::start() {
	CUICommand cui_command;

	do {
		if (mpi_rank != 0) {
			MPI_Bcast(&cui_command, sizeof(CUICommand), MPI_BYTE, 0, MPI_COMM_WORLD);
		}

		//Execute console input
		executeCommand(cui_command);
	} while (cui_command.getCommand() != CUICommand::EXIT);
}

void CommandHandler::handleCUICommand(CUICommand cui_command) {
	MPI_Bcast(&cui_command, sizeof(CUICommand), MPI_BYTE, 0, MPI_COMM_WORLD);
	executeCommand(cui_command);
}

void CommandHandler::executeCommand(CUICommand cui_command) {
	std::string file_path, time_for_move, source_position, sink_height;
	switch (cui_command.getCommand())
	{
	case (CUICommand::LOAD_MESH):
		if (mpi_rank == 0) {
			file_path = cui_command.getParameter(0).getValue();
			loaded_mesh = loadMesh(mpi_rank, file_path);

			// console feedback
			cout << "Mesh loaded." << endl;
			cout << "Vertices: " << loaded_mesh.getVertexCount() << " Faces: " << loaded_mesh.getFaceCount() << endl;
			printInputMessage();
		}
		MPI_Barrier(MPI_COMM_WORLD);
		break;
	case (CUICommand::LOAD_SHUTTER):
		if (mpi_rank == 0) {
			file_path = cui_command.getParameter(0).getValue();
			loaded_shutter = loadMesh(mpi_rank, file_path);

			// console feedback
			cout << "Mesh loaded." << endl;
			cout << "Vertices: " << loaded_shutter.getVertexCount() << " Faces: " << loaded_shutter.getFaceCount() << endl;
			printInputMessage();
		}
		MPI_Barrier(MPI_COMM_WORLD);
		break;
	case (CUICommand::GENERATE_PARTICLES):
		generateParticles(mpi_rank, sph_manager, loaded_mesh, SphParticle::ParticleType::STATIC);
		generateParticles(mpi_rank, sph_manager, loaded_shutter, SphParticle::ParticleType::SHUTTER);

		// console feedback
		MPI_Barrier(MPI_COMM_WORLD);
		if (mpi_rank == 0) {
			cout << "Static particles generated." << endl;
			printInputMessage();
		}
		break;
	case (CUICommand::MOVE_SHUTTER):
		time_for_move = cui_command.getParameter(0).getValue();
		moveShutter(mpi_rank, sph_manager, time_for_move);
		MPI_Barrier(MPI_COMM_WORLD);

		// console feedback
		if (mpi_rank == 0) {
			cout << "Stutter move set." << endl;
			printInputMessage();
		}
		break;
	case (CUICommand::SIMULATE):
		if (mpi_rank != 0) {
			simulate(mpi_rank, sph_manager);
		}
		else {
			createExport(mpi_rank, sph_manager);
		}
		MPI_Barrier(MPI_COMM_WORLD);

		// console feedback
		if (mpi_rank == 0) {
			cout << "Simulation finished." << endl;
			printInputMessage();
		}
		break;
	case (CUICommand::RENDER):
		render(mpi_rank, loaded_mesh, loaded_shutter, 0);
		MPI_Barrier(MPI_COMM_WORLD);

		// console feedback
		if (mpi_rank == 0) {
			cout << "Rendering finished." << endl;
			printInputMessage();
		}
	case (CUICommand::ADD_SOURCE):
		source_position = cui_command.getParameter(0).getValue();
		addSource(mpi_rank, sph_manager, source_position);
		MPI_Barrier(MPI_COMM_WORLD);

		// console feedback
		if (mpi_rank == 0) {
			cout << "Added source." << endl;
			printInputMessage();
		}
		break;
	case (CUICommand::ADD_SINK):
		sink_height = cui_command.getParameter(0).getValue();
		addSink(mpi_rank, sph_manager, sink_height);
		MPI_Barrier(MPI_COMM_WORLD);

		// console feedback
		if (mpi_rank == 0) {
			cout << "Added sink." << endl;
			printInputMessage();
		}
		break;
	default:
		break;
	}
}

void CommandHandler::printInputMessage() {
	std::cout << std::endl << "Please enter a command or enter 'help' to show a list of all commands" << std::endl;
}

Terrain CommandHandler::loadMesh(int rank, std::string file_path) {
	std::cout << "Loading Mesh: \"" << file_path << "\"" << std::endl;
	return TerrainParser::loadFromFile(file_path);
}

void CommandHandler::generateParticles(int rank, SphManager& sphManager, Terrain& loadedMesh, SphParticle::ParticleType pType) {
	StaticParticleGenerator gen;

	if (rank == 0)
		gen.sendAndGenerate(loadedMesh, pType);
	else
		gen.receiveAndGenerate(sphManager, pType);
}

void CommandHandler::createExport(int rank, SphManager& sph_manager) {
	int current_timestep = 1;
	unordered_map<int, vector<SphParticle>> export_map;

	int slave_comm_size;
	MPI_Comm_size(MPI_COMM_WORLD, &slave_comm_size);
	slave_comm_size--;

	while (current_timestep <= TIMESTEPS) {
		std::vector<SphParticle> all_particles_of_timestep;

		std::vector<int> number_of_incoming_particles = std::vector<int>(slave_comm_size);
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

void CommandHandler::moveShutter(int rank, SphManager& sphManager, std::string shutterMoveParam) {
	std::istringstream shutterMoveValues(shutterMoveParam);
	int shutterMoveFrame;
	shutterMoveValues >> shutterMoveFrame;
	std::cout << "Shutter opening at frame: " << shutterMoveFrame << std::endl;
}

void CommandHandler::simulate(int rank, SphManager& sph_manager) {
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


void CommandHandler::render(int rank, Terrain loadedMesh, Terrain loadedShutter, int shutterTime) {
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

void CommandHandler::addSource(int rank, SphManager& sphManager, std::string sourcePosParam) {
	std::istringstream sourcePosValues(sourcePosParam);
	double x, y, z;
	sourcePosValues >> x;
	sourcePosValues >> y;
	sourcePosValues >> z;
	Vector3 sourcePos = Vector3(x, y, z);
	std::cout << "New source: " << sourcePos << std::endl;
}

void CommandHandler::addSink(int rank, SphManager& sphManager, std::string sinkHeightParam) {
	std::istringstream sinkHeightValues(sinkHeightParam);
	double sinkHeight;
	sinkHeightValues >> sinkHeight;
	std::cout << "New sink height: " << sinkHeight << std::endl;
}
