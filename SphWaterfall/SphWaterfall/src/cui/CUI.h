#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <algorithm>

#include "../geometry/TerrainParser.h"
#include "../simulation/SphManager.h"
#include "AsyncCommand.h"
#include "CommandParameter.h"
#include "CUICommand.h"

namespace CUI {
	extern AsyncCommand asyncCommand;
	extern CUICommand current_command;

	void startCUI();
	void startWithStream(std::istream&, bool);

	bool readNextToken(std::queue<std::string> &tokens, std::string &nextToken);
	bool readNextCombinedToken(std::queue<std::string> &tokens, std::string &nextToken);
	void trim(std::string &string);
	std::string trimQuotemarks(std::string string);

	void printInputMessage();

	/* -_-_-_Commands Begin_-_-_- */
	void printCommand();
	void loadMesh(std::queue<std::string> &tokens);
	void loadShutter(std::queue<std::string> &tokens);
	void moveShutter(std::queue<std::string> &tokens);
	void loadConfig(std::queue<std::string> &tokens);
	void addSource(std::queue<std::string> &tokens);
	void addSink(std::queue<std::string> &tokens);
	void showHelp();
	/* -_-_-_Commands End_-_-_- */
}