#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>

#include "../geometry/TerrainParser.h"
#include "../simulation/SphManager.h"
#include "../visualization/VisualizationManager.h"

class CUI {
public:
	CUI();
	void trim(std::string &str);
	bool readNextToken(std::queue<std::string> &tokens, std::string &nextToken);
	bool readNextCombinedToken(std::queue<std::string> &tokens, std::string &nextToken);
	void loadMesh(std::queue<std::string> &tokens);
	void simulate(std::queue<std::string> &tokens);
	void showHelp();
	void readCommands(int* buffer);
private:
	std::string inputLine, command;
	std::queue<std::string> tokens;
};