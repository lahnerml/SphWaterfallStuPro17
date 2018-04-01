#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "../geometry/TerrainParser.h"
#include "../data/NullableWrapper.h"
//#include "AsyncCommand.h"
#include "CUICommand.h"
#include "CommandHandler.h"

class CUI {
	public:
		//AsyncCommand asyncCommand;

		CUI();

		void start();
		void printInputMessage();

	private:
		CommandHandler command_handler;
		CUICommand current_command;
		bool exit_programm;

		void startWithStream(std::istream&, bool);

		void parseCommand(std::string);
		void cleanAndExecuteCommand(bool);

		void trim(std::string& string);
		std::string trimQuotemarks(std::string& string);

		/* -_-_-_Commands Begin_-_-_- */
		void printCommand();
		void loadConfig();
		void showHelp();
		bool cleanLoadMesh();
		bool cleanLoadShutter();
		bool cleanMoveShutter();
		bool cleanAddSource();
		bool cleanAddSink();
		/* -_-_-_Commands End_-_-_- */
};