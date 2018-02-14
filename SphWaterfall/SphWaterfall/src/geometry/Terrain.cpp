#pragma once

#include <vector>

#include "Terrain.h"


Terrain::Terrain(std::vector<Vector3> &vertices, std::vector<Vector3> &normals, std::vector<int> &faces) :
	vertices(vertices)
{
	for(int i = 0; i < faces.size(); i++)
	{
		//std::cout << i << ": (" << faces[i] << "," << faces[i + 1] << "," << faces[i + 2] << ")" << std::endl;
		this->faces.push_back(Face::Face(vertices[faces[i]], vertices[faces[i + 1]], vertices[faces[i + 2]]));
		i += 2;
		//std::cout << this->faces.at(this->faces.size() - 1);
		//TODO Debug output
	}
}

Terrain::Terrain() :
	vertices(std::vector<Vector3>()),
	faces(std::vector<Face>())
{
}


size_t Terrain::getVertexCount()
{
	return this->vertices.size();
}

size_t Terrain::getFaceCount()
{
	return this->faces.size();
}


Vector3 Terrain::getVertex(int index)
{
	if (index < 0 || index >= this->vertices.size())
		return Vector3::Vector3();

	return this->vertices[index];
}

Face Terrain::getFace(int faceId)
{
	if (faceId < 0 || faceId >= this->faces.size())
		return Face();

	return this->faces[faceId];
}
