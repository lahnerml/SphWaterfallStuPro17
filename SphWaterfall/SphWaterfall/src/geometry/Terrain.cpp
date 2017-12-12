#pragma once

#include <vector>

#include "Terrain.h"

Terrain::Terrain(std::vector<Vector3> vertices, std::vector<Vector3> normals, std::vector<int> faces) :
	vertices(vertices),
	normals(normals)
{
	for (int i = 0; i < faces.size / 3; i++)
	{
		this->faces.push_back(Face::Face(face[i * 3], face[(i * 3) + 1], face[(i * 3) + 2]));
	}
}

int Terrain::getVertexCount()
{
	return this->vertices.size;
}

int Terrain::getFaceCount()
{
	return this->faces.size;
}

Vector3 Terrain::getVertexPosition(int index)
{
	if (index < 0 || index >= this->vertices.size)
		return Vector3::Vector3();
	return this->vertices[index];
}

int Terrain::getFaceVertexId(int index, int vertexNr)
{
	if (index < 0 || index >= this->faces.size)
		return Vector3::Vector3();
	return this->faces[index];
}
