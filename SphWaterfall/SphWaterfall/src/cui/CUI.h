#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>

#include "../geometry/TerrainParser.h"
#include "../simulation/SphManager.h"
#include "../visualization/VisualizationManager.h"

namespace CUI {
	void trim(std::string &str);
	void readCommand(int* command_buffer);
}