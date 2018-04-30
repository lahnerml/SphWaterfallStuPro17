#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "Terrain.h"
#include "../data/Vector3.h"

class TerrainParser {
public:
	static Terrain loadFromFile(std::string fileName);
};