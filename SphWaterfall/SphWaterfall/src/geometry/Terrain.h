#pragma once
#include "../data/Vector3.h"
#include "Face.h"

class Terrain {
public:
	Terrain(int verticeCount, int faceCount);

	int getVertexCount();
	int getFaceCount();
	Vector3 getVertexPosition(int index);
	int getFaceVertexId(int index, int vertexNr);



private:
	Vector3 vertices[];
	Face faces[];


};