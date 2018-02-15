#pragma once
#include "StaticParticleGenerator.h"

const double PARTICLE_DENSITY = 0.01;



StaticParticleGenerator::StaticParticleGenerator()
{
}



void StaticParticleGenerator::sendAndGenerate(Terrain terrain)
{
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int worldSize;
	MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

	// Only main processor shall pass and only if there are faces to process
	if (rank != 0)
		return;

	const int facesPerProcessor = terrain.getFaceCount() / worldSize;
	const int facesToSend = (worldSize - 1) * facesPerProcessor;
	int currentProcessor = 0;

	//If not enough faces -> tell processors to continue
	if (facesPerProcessor == 0)
	{
		for (currentProcessor = 1; currentProcessor < worldSize; currentProcessor++)
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
			if (currentProcessor != (f / facesPerProcessor) + 1)
			{
				currentProcessor = (f / facesPerProcessor) + 1;
				//Send number of total faces
				MPI_Send(&facesPerProcessor, 1, MPI_INT, currentProcessor, 0, MPI_COMM_WORLD);
			}

			//Send face
			Face::MpiSendFace(terrain.getFace(f), currentProcessor);
		}
	}
	//TODO Debug output
	std::cout << (terrain.getFaceCount() - facesToSend) << " faces remaining for processor " << rank << std::endl;


	//Process faces of main processor
	auto generatedParticles = std::vector<SphParticle>();

	for (int f = facesToSend; f < terrain.getFaceCount(); f++)
	{
		generateParticlesOnFace(terrain.getFace(f), PARTICLE_DENSITY, generatedParticles);
		//std::cout << terrain.getFace(f) << std::endl;
		//TODO Debug output
	}

	//TODO Reintegrate
	//manager.add_particles(generatedParticles);
}

void StaticParticleGenerator::receiveAndGenerate(SphManager manager)
{
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int worldSize;
	MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

	if (rank == 0)
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
		generateParticlesOnFace(face, PARTICLE_DENSITY, generatedParticles);
	}

	//TODO Reintegrate
	//manager.add_particles(generatedParticles);
}



std::vector<SphParticle> StaticParticleGenerator::generateStaticParticles(Terrain source)
{	
	std::vector<SphParticle> staticParticles = std::vector<SphParticle>();

	for (int i = 0; i < source.getVertexCount(); i++) {
		staticParticles.push_back(SphParticle(source.getVertex(i)));
	}

	return staticParticles;
}



void StaticParticleGenerator::generateParticlesOnFace(Face& face, double particleDensity, std::vector<SphParticle>& generatedParticles)
{
	Vector3 particlePosition = Vector3();

	if (0.0 >= particleDensity || 1.0 < particleDensity)
		return;

	//Create uniform grid of particles
	for (double x = 0.0; x <= 1.0; x += particleDensity)
	{
		for (double y = 0.0; y <= 1.0; y += particleDensity)
		{
			if (x + y > 1)
				continue;

			particlePosition = face.a + ((face.b - face.a) * x) + ((face.c - face.a) * y);
			generatedParticles.push_back(StaticParticle(particlePosition));
			//std::cout << particlePosition << std::endl;
			//TODO Debug output
		}
	}
}

/*
 * Generates static particles uniformly on a given face
 * face: The face to genrate the particles on
 * particleDensity: How much space lies between 2 particles 0.0 < density <= 1.0
 * A particleDenisty of 1.0 means that there will only be particles on the corners of the Face
 */
std::vector<SphParticle> StaticParticleGenerator::generateParticlesOnFace(Face& face, double particleDensity)
{
	std::vector<SphParticle> result = std::vector<SphParticle>();
	generateParticlesOnFace(face, particleDensity, result);
	return result;
}



void StaticParticleGenerator::detectDuplicate(SphParticle a, SphParticle b) {
	
}

void StaticParticleGenerator::removeDuplicate(SphParticle a, SphParticle b) {
	
}