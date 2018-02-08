#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <mutex>

#include "../geometry/TerrainParser.h"
#include "../simulation/SphManager.h"

using namespace std;

namespace CUI {

	enum ConsoleCommand
	{
		NONE = -1,
		EXIT = 0,
		LOAD_MESH = 1,
		GENERATE_PARTICLES = 2,
		MOVE_SHUTTER = 3,
		SIMULATE = 4,
		RENDER = 5
	};

	class AsyncCommand
	{
	public:
		AsyncCommand();

		ConsoleCommand aReadCmd();
		void aWriteCmd(ConsoleCommand cmd);

		ConsoleCommand aReadCmd(std::string& param);
		void aWriteCmd(ConsoleCommand cmd, std::string param);
	private:
		std::mutex cmdLock;
		ConsoleCommand command;
		std::string param;
	};

	extern AsyncCommand acmd;

	void trim(std::string &str);
	void startCUI();
}