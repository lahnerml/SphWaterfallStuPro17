#pragma once

#include <vector>

#include "../data/Vector3.h"
#include "Face.h"

class Terrain{
public:
	Terrain(std::vector<Vector3> vertices, std::vector<Vector3> normals, std::vector<Vector3> faces);

	int getVertexCount();
	int getFaceCount();
	Vector3 getVertexPosition(int index);
	int getFaceVertexId(int index, int vertexNr);

private:
	Vector3 vertices[];
	Vector3 normals[];
	Face faces[];


};