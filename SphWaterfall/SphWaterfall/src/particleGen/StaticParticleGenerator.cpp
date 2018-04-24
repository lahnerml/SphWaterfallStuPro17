#include "StaticParticleGenerator.h"

// density of how much the distance between static particles is
#define STATIC_PARTICLE_GENERATION_DENSITY 0.3

StaticParticleGenerator::StaticParticleGenerator()
{
}

void StaticParticleGenerator::sendAndGenerate(Terrain& terrain, SphParticle::ParticleType type)
{
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int worldSize;
	MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

	// Only main processor shall pass and only if there are faces to process
	if (rank != 0)
		return;
	if (worldSize < 2)
		return;

	const int facesPerProcessor = terrain.getFaceCount() / (worldSize-1);
	const int facesToSend = (worldSize - 2) * facesPerProcessor;
	int currentProcessor = 0;

	//If not enough faces -> tell processors to continue
	if (facesPerProcessor == 0)
	{
		for (currentProcessor = 2; currentProcessor < worldSize; currentProcessor++)
		{
			//Tell processor to skip 'Waiting for faces'
			int noFacesToSend = 0;
			MPI_Send(&noFacesToSend, 1, MPI_INT, currentProcessor, 0, MPI_COMM_WORLD);
		}
	}
	else {
		//Send faces to remote processors
		for (int f = 0; f < facesToSend; f++)
		{
			//Check if remote processor has to change
			if (currentProcessor != (f / facesPerProcessor) + 2)
			{
				currentProcessor = (f / facesPerProcessor) + 2;
				std::cout << "Sending number of faces (" << facesPerProcessor << ") to processor " << currentProcessor << std::endl;
				//Send number of total faces
				MPI_Send(&facesPerProcessor, 1, MPI_INT, currentProcessor, 0, MPI_COMM_WORLD);
			}

			//Send face
			Face::MpiSendFace(terrain.getFace(f), currentProcessor);

		}
	}
	//TODO Debug output
	int facesForP1 = terrain.getFaceCount() - facesToSend;
	std::cout << facesForP1 << " faces remaining for processor 1" << std::endl;


	//Process faces of processor 1
	//Send number of total faces
	MPI_Send(&facesForP1, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

	for (int f = facesToSend; f < terrain.getFaceCount(); f++)
	{
		Face::MpiSendFace(terrain.getFace(f), 1);
	}
}

void StaticParticleGenerator::receiveAndGenerate(SphManager& manager, SphParticle::ParticleType type)
{
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int worldSize;
	MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

	if (rank == 0)
		return;
	if (worldSize < 2)
		return;

	int facesToReceive = 0;
	MPI_Recv(&facesToReceive, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	auto faces = std::vector<Face>();


	//Receive faces
	for (int f = 0; f < facesToReceive; f++)
	{
		faces.push_back(Face::MpiReceiveFace(0));
	}
	//TODO Debug output
	std::cout << facesToReceive << " faces received at processor " << rank << std::endl;


	//Process faces
	auto generatedParticles = std::vector<SphParticle>();

	for (auto face : faces)
	{
		generateParticlesOnFace(face, STATIC_PARTICLE_GENERATION_DENSITY, generatedParticles, type);
	}

	//for (auto each_particle : generatedParticles) { std::cout << "static particle: " << each_particle << std::endl; } //debug

	//TODO Reintegrate
	manager.add_particles(generatedParticles);
}



std::vector<SphParticle> StaticParticleGenerator::generateStaticParticles(Terrain& source)
{	
	std::vector<SphParticle> staticParticles = std::vector<SphParticle>();

	for (int i = 0; i < source.getVertexCount(); i++) {
		staticParticles.push_back(SphParticle(source.getVertex(i)));
	}

	return staticParticles;
}



void StaticParticleGenerator::generateParticlesOnFace(Face& face, double particleDensity, std::vector<SphParticle>& generatedParticles, SphParticle::ParticleType type)
{
	if (particleDensity < 0.0)
		return;

	Vector3 particlePosition = Vector3();

	double uneven_offset = particleDensity / 2;
	double curr_offset = 0.0;

	double x_distance = (face.b - face.a).length();
	double y_distance = (face.c - face.a).length();

	double x_perc, y_perc;

	bool uneven = false;

	//Create triangualted grid of particles
	for (int x = 0; x * particleDensity <= x_distance; x++)
	{
		x_perc = (x * particleDensity) / x_distance;
		curr_offset = uneven ? uneven_offset : 0.0;
		
		for (int y = 0; ((y * particleDensity) + curr_offset) <= y_distance; y++)
		{
			y_perc = ((y * particleDensity) + curr_offset) / y_distance;
			
			if (x_perc + y_perc >= 1.0)
				continue;

			particlePosition = face.a + ((face.b - face.a) * x_perc) + ((face.c - face.a) * y_perc);
			generatedParticles.push_back(SphParticle(particlePosition, SphParticle::STATIC));
			generatedParticles.push_back(generateMoonParticle(particlePosition, particleDensity, type));
		}

		uneven = !uneven;
	}
}

/*
 * Generates static particles uniformly on a given face
 * face: The face to genrate the particles on
 * particleDensity: How much space lies between 2 particles 0.0 < density <= 1.0
 * A particleDenisty of 1.0 means that there will only be particles on the corners of the Face
 */
std::vector<SphParticle> StaticParticleGenerator::generateParticlesOnFace(Face& face, double particleDensity, SphParticle::ParticleType pType)
{
	std::vector<SphParticle> result = std::vector<SphParticle>();
	generateParticlesOnFace(face, particleDensity, result, pType);
	return result;
}


void StaticParticleGenerator::detectDuplicate(SphParticle a, SphParticle b) {
	
}

void StaticParticleGenerator::removeDuplicate(SphParticle a, SphParticle b) {
	
}

SphParticle StaticParticleGenerator::generateMoonParticle(Vector3 particle_position, double particle_density, SphParticle::ParticleType particle_type)
{
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_real_distribution<double> distribution(-particle_density / 2, particle_density / 2);
	auto random = std::bind(distribution, generator);
	return SphParticle(Vector3(particle_position.x + random(), particle_position.y + random(), particle_position.z + random()), particle_type);
}
