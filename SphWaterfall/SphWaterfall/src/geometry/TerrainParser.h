#pragma once

#include <string>

#include "Terrain.h"

class TerrainParser {
public:
	Terrain loadFromFile(std::string fileName);
private:
	void readObjFile(std::string filename, std::vector<Vector3> &vertices, std::vector<Vector3> &normals, std::vector<int> &faces);
};