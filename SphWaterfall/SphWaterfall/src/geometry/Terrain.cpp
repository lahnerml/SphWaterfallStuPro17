#pragma once

#include <vector>

#include "Terrain.h"

Terrain::Terrain(std::vector<Vector3> &vertices, std::vector<Vector3> &normals, std::vector<int> &faces) :
	vertices(vertices),
	normals(normals)
{
	std::vector<int>::iterator it;
	for (it = faces.begin(); it != faces.end(); it++) {
		this->faces.push_back(Face::Face(*it, *++it, *++it));
	}
}

Terrain::Terrain()
{
	vertices = std::vector<Vector3>();
	normals = std::vector<Vector3>();
	faces = std::vector<Face>();
}

size_t Terrain::getVertexCount()
{
	return this->vertices.size();
}

size_t Terrain::getFaceCount()
{
	return this->faces.size();
}

Vector3 Terrain::getVertexPosition(int index)
{
	if (index < 0 || index >= this->vertices.size())
		return Vector3::Vector3();

	return this->vertices[index];
}

int Terrain::getFaceVertexId(int index, int vertexNr)
{
	if (index < 0 || index >= this->faces.size()
		|| vertexNr < 0 || vertexNr > 2)
		return -1;

	return 0; //this->faces[index].vertices[vertexNr];
	//TODO Implement .vertices[]
}
