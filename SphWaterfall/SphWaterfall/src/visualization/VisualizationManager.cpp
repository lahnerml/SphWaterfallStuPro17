#include "VisualizationManager.h"

void VisualizationManager::init(Vector3 cameraLocation, unsigned int frameWidth, unsigned int frameHeight) {
	Vector3 cameraDir = (cameraLocation*-1);
	cameraDir = Vector3(0, 0, 1);
	camera = Camera(cameraLocation, cameraDir.normalize(), frameWidth, frameHeight);
	if (terrainOpen.getVertexCount() > 0 && terrainClosed.getVertexCount() > 0) {
		camera.renderGeometryFrames(terrainOpen, terrainClosed);
	}
	initilaized = true;
}

void VisualizationManager::debugRenderFrame(std::vector<SphParticle> particles, string fileName) {
	camera.debugRenderFrame(convertSphParticles(particles));
}

void VisualizationManager::importTerrain(Terrain t, bool open) {
	if (open) { terrainOpen = t; }
	else { terrainClosed = t; }
}

void VisualizationManager::renderFrames(string inputFileName) {
	if (!initilaized) {
		cout << "Please initialize the VisualizationManager before rendering!\n";
	}

	vector<vector<SphParticle>> frameParticles = ParticleIO::importParticles(inputFileName);

	for (int g = 0; g < frameParticles.size(); g++) {
		cout << "Rendering Frame #" << g << "\n";
		vector<ParticleObject> frame = convertFluidParticles(frameParticles.at(g));

		Frame f = camera.renderFrame(frame, g);

		writeFrameToBitmap(f, (("output/frame_" + std::to_string(g)) + ".bmp").c_str(), f.getWidth(), f.getHeight());
	}
}

void VisualizationManager::renderFramesDistributed(string inputFileName, int rank) {
	if (!initilaized) {
		cout << "Please initialize the VisualizationManager before rendering!\n";
	}

	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	if (rank == 0) {
		vector<vector<SphParticle>> frameParticles = ParticleIO::importParticles(inputFileName);
		for (int i = 1; i < world_size; i++) {
			unsigned int buf[1] =
			{
				static_cast<unsigned int>(frameParticles.size())
			};
			MPI_Send(buf, 1, MPI_UNSIGNED, i, 0, MPI_COMM_WORLD);
		}

		for (int g = 0; g < frameParticles.size(); g++) {
				int target = (g % (world_size - 1)) + 1;
				vector<ParticleObject> frame = convertFluidParticles(frameParticles.at(g));

				//Send frame size
				unsigned int buf[1] =
				{
					static_cast<unsigned int>(frame.size())
				};
				MPI_Send(buf, 1, MPI_UNSIGNED, target, 0, MPI_COMM_WORLD);

				for (int i = 0; i < frame.size(); i++) {
					ParticleObject::MpiSendPObject(frame[i], target);
				}
		}
	}
	else {
		unsigned int buf[1];
		MPI_Recv(buf, 1, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		int frameCount = buf[0];

		vector<vector<ParticleObject>> frameParticles;


		for (int g = 0; g < frameCount; g++) {
			if ((g % (world_size - 1)) + 1 == rank) {
				unsigned int frameSize[1];
				MPI_Recv(frameSize, 1, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				vector<ParticleObject> frame;
				for (int i = 0; i < frameSize[0]; i++) {
					frame.emplace_back(ParticleObject::MpiReceivePObject(0));
				}
				frameParticles.emplace_back(frame);
			}
		}

		int counter = 0;

		//Here it stops working for some reason..

		for (int i = 0; i < frameCount; i++) {
			if ((i % (world_size - 1)) + 1 == rank) {
				if (counter >= frameParticles.size()) continue;
				vector<ParticleObject> frame = frameParticles.at(counter);

				Frame f = camera.renderFrame(frame, i);

				writeFrameToBitmap(f, (("output/frame_" + std::to_string(i)) + ".bmp").c_str(), f.getWidth(), f.getHeight());
				counter++;
			}
		}
	}
}

void VisualizationManager::generateFrames(int frameCount, int particleCount) {
	unordered_map<int, vector<SphParticle>> frames;

	cout << "Generating " << frameCount << " frames...\n";

	for (int i = 1; i <= frameCount; i++) {
		frames.insert_or_assign(i, generateDebugParticles(particleCount));
	}

	cout << "Frames generated\n";

	ParticleIO::exportParticles(frames, "test.particles");
}

vector<SphParticle> VisualizationManager::generateDebugParticles(int count) {
	vector<SphParticle> particles;

	for (int i = 0; i < count; i++) {
		double x = double(rand() % 10000 - 5000) / 1000;
		double y = double(rand() % 10000 - 5000) / 1000;
		double z = double(rand() % 10000 - 5000) / 1000;

		SphParticle f = SphParticle(Vector3(x, y, z), Vector3(0, 0, 0), 1);
		particles.emplace_back(f);
	}
	return particles;
}

void VisualizationManager::debug() {
	unordered_map<int, vector<SphParticle>> frames;

	vector<SphParticle> part;
	part.emplace_back(SphParticle(Vector3(3, 0, 0)));

	for (int i = 1; i <= 1; i++) {
		frames.insert_or_assign(i, part);
	}

	ParticleIO::exportParticles(frames, "test.particles");
}

bool VisualizationManager::initilaized = false;
Camera VisualizationManager::camera = Camera();
Terrain VisualizationManager::terrainOpen = Terrain();
Terrain VisualizationManager::terrainClosed = Terrain();