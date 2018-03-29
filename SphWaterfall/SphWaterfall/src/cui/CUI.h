#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>

#include "../geometry/TerrainParser.h"
#include "../simulation/SphManager.h"

namespace CUI {

	enum ConsoleCommand
	{
		NONE = -1,
		EXIT = 0,
		LOAD_MESH,
		LOAD_SHUTTER,
		GENERATE_PARTICLES,
		MOVE_SHUTTER,
		SIMULATE,
		RENDER,
		ADD_SOURCE,
		ADD_SINK
	};

	class AsyncCommand
	{
	public:
		AsyncCommand();

		ConsoleCommand readCmd();
		void writeCmd(ConsoleCommand cmd);

		ConsoleCommand readCmd(std::string& param);
		void writeCmd(ConsoleCommand cmd, std::string param);
		void printInputMessage();
		bool commandEmpty();
	private:
		std::mutex cmdLock;
		std::condition_variable cv;
		ConsoleCommand command;
		std::string param;
	};

	extern AsyncCommand acmd;

	void trim(std::string &str);
	void startCUI();
	void startWithStream(std::istream&, bool);
}