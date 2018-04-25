#pragma once

#include <vector>

#include "../data/Vector3.h"
#include "Face.h"

//Basically a const mesh consisting of vertices and faces
class Terrain{
public:
	Terrain(std::vector<Vector3> &vertices, std::vector<int> &faces);
	Terrain();

	const size_t getVertexCount();
	const size_t getFaceCount();

	const Vector3 getVertex(int);
	const Face& getFace(int);

private:
	std::vector<Vector3> vertices;
	std::vector<Face> faces;
	Face nullFace;
};