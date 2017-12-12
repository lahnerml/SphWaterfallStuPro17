#pragma once

#include <vector>

#include "Terrain.h"

Terrain::Terrain(std::vector<Vector3> vertices, std::vector<Vector3> normals, std::vector<Vector3> faces) :
	vertices(vertices),
	normals(normals),
	faces(faces)
{

}

int Terrain::getVertexCount()
{
	return 0;
}

int Terrain::getFaceCount()
{
	return 0;
}

Vector3 Terrain::getVertexPosition(int index)
{
	return Vector3();
}

int Terrain::getFaceVertexId(int index, int vertexNr)
{
	return 0;
}
