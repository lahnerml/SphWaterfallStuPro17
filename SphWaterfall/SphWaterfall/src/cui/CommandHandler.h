#pragma once
#include <thread>

#include "mpi.h"

#include "CUICommand.h"
#include "../simulation/SimulationUtilities.h"
#include "../data/SphParticle.h"
#include "../particleGen/StaticParticleGenerator.h"
#include "../geometry/TerrainParser.h"
#include "../visualization/VisualizationManager.h"
#include "../data/ParticleIO.h"

class CommandHandler {
	public:
		CommandHandler(int mpi_rank);

		void start();
		void handleCUICommand(CUICommand);


		void printInputMessage();
	private:
		const int mpi_rank;
		CUICommand current_command;

		SphManager sph_manager;
		Terrain loaded_mesh, loaded_shutter;

		void executeCommand(CUICommand);

		Terrain loadMesh(int rank, std::string fileName);
		void generateParticles(int rank, SphManager& sphManager, Terrain& loadedMesh, SphParticle::ParticleType pType);
		void createExport(int rank, SphManager& sph_manager);
		void moveShutter(int rank, SphManager& sphManager, std::string shutterMoveParam);
		void simulate(int rank, SphManager& sph_manager);
		void render(int rank, Terrain loadedMesh, Terrain loadedShutter, int shutterTime);
		void addSource(int rank, SphManager& sphManager, std::string sourcePosParam);
		void addSink(int rank, SphManager& sphManager, std::string sinkHeightParam);
};
