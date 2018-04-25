#include "StaticParticleGenerator.h"

// density of how much the distance between static particles is
#define STATIC_PARTICLE_GENERATION_DENSITY 0.3

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
			//Check if it's next remote processors turn to receive faces
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

	//Process faces of processor 1
	//Send number of total faces
	int facesForP1 = terrain.getFaceCount() - facesToSend;
	std::cout << facesForP1 << " faces remaining for processor 1" << std::endl;

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
	std::cout << facesToReceive << " faces received at processor " << rank << std::endl;


	//Process faces
	auto generatedParticles = std::vector<SphParticle>();

	for (auto face : faces)
	{
		generateParticlesOnFace(face, STATIC_PARTICLE_GENERATION_DENSITY, generatedParticles, type);
	}

	//Send faces to simulation
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



void StaticParticleGenerator::generateParticlesOnFace(Face& face, double particleDistance, std::vector<SphParticle>& generatedParticles, SphParticle::ParticleType type)
{
	if (particleDistance < 0.0)
		return;

	Vector3 particlePosition = Vector3();

	double uneven_offset = particleDistance / 2;
	double curr_offset = 0.0;

	double x_distance = (face.b - face.a).length();
	double y_distance = (face.c - face.a).length();

	double x_perc, y_perc;

	bool uneven = false;

	//Create triangualted grid of particles
	for (int x = 0; x * particleDistance <= x_distance; x++)
	{
		x_perc = (x * particleDistance) / x_distance;
		curr_offset = uneven ? uneven_offset : 0.0;
		
		for (int y = 0; ((y * particleDistance) + curr_offset) <= y_distance; y++)
		{
			y_perc = ((y * particleDistance) + curr_offset) / y_distance;
			
			if (x_perc + y_perc >= 1.0)
				continue;

			particlePosition = face.a + ((face.b - face.a) * x_perc) + ((face.c - face.a) * y_perc);
			generatedParticles.push_back(SphParticle(particlePosition, type));

			Vector3 perpendicular = Vector3::perpendicular((face.b - face.a), (face.c - face.a));
			generatedParticles.push_back(SphParticle(particlePosition + (particleDistance * perpendicular), type));
			generatedParticles.push_back(SphParticle(particlePosition + (-particleDistance * perpendicular), type));
		}

		uneven = !uneven;
	}
}

/*
 * Generates static particles uniformly on a given face
 * particleDensity: How much space lies between 2 particles
 */
std::vector<SphParticle> StaticParticleGenerator::generateParticlesOnFace(Face& face, double particleDistance, SphParticle::ParticleType pType)
{
	std::vector<SphParticle> result = std::vector<SphParticle>();
	generateParticlesOnFace(face, particleDistance, result, pType);
	return result;
}
