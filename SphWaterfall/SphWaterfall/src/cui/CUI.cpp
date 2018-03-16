#pragma once
#include "CUI.h"
//#include "../visualization/VisualizationManager.h"

namespace CUI {

	AsyncCommand acmd;

	void trim(std::string &str) {
		int pos1 = str.find_first_not_of(" ");
		int pos2 = str.find_last_not_of(" ");
		str = str.substr(pos1, pos2 - pos1 + 1);
	}

	bool readNextToken(std::queue<std::string> &tokens, std::string &nextToken)
	{
		if (tokens.empty()) {
			return false;
		}
		else {
			nextToken = tokens.front();
			tokens.pop();
			return true;
		}
	}

	bool readNextCombinedToken(std::queue<std::string> &tokens, std::string &nextToken) {
		string tempToken = "", resultToken = "";

		//Read first token
		if (!readNextToken(tokens, tempToken))
			return false;
		if (tempToken.find_first_of('\"') != 0) {
			//No combined token
			nextToken = tempToken;
			return true;
		}
		if (tempToken.find_last_of('\"') == tempToken.length() - 1) {
			//Combined token without spaces
			nextToken = tempToken.substr(1, tempToken.length() - 2);
			return true;
		}

		//Read following tokens
		do {
			resultToken += tempToken + " ";
			if (!readNextToken(tokens, tempToken)) {
				// No second quotation mark
				nextToken = resultToken.substr(1);
				return true;
			}
		} while (tempToken.find_last_of('\"') != tempToken.length() - 1);

		//Return combined token
		resultToken += tempToken;
		nextToken = resultToken.substr(1, resultToken.length() - 2);
		return true;
	}


	/* -_-_-_Comands Begin_-_-_- */

	void loadMesh(std::queue<std::string> &tokens)
	{
		string paramName, fileName;

		//Read fileName Parameter
		if (readNextToken(tokens, paramName) && paramName == "-p") {
			fileName = "";
			readNextCombinedToken(tokens, fileName);

			acmd.aWriteCmd(CUI::ConsoleCommand::LOAD_MESH, fileName);
		}
		else
		{
			cout << "Missing path parameter '-p'" << endl;
		}
	}

	void generateParticles(std::queue<std::string> &tokens)
	{
		// TODO add correct dimensions
		// SphManager sph_manager = SphManager::SphManager(Vector3(10, 10, 10), 10, 1);
		acmd.aWriteCmd(CUI::ConsoleCommand::GENERATE_PARTICLES);
	}

	void simulate(std::queue<std::string> &tokens)
	{
		// TODO add correct dimensions
		acmd.aWriteCmd(CUI::ConsoleCommand::SIMULATE);
	}

	void render()
	{
		acmd.aWriteCmd(CUI::ConsoleCommand::RENDER);
	}

	void showHelp()
	{
		cout << "loadMesh -p" << endl
			<< "particleGen [-w] [-f] [-e]" << endl
			<< "moveShutter -t (-u/-d) [-l]" << endl
			<< "simulate -s -e -r -g -m -t" << endl
			<< "render" << endl
			<< "help" << endl
			<< "exit" << endl;
	}

	/* -_-_-_Comands End_-_-_- */

	void startCUI()
	{
		string inputLine, command;
		queue<string> tokens;

		while (acmd.aReadCmd() != CUI::ConsoleCommand::EXIT)
		{
			//Read command
			cout << endl << "Please enter a command or enter 'help' to show a list of all commands" << endl;
			getline(cin, inputLine);
			trim(inputLine);

			//Tokenize command
			istringstream tokenStream(inputLine);
			tokens = queue<string>();
			while (tokenStream >> command)
				tokens.push(command);

			//Execute command
			if (!tokens.empty())
			{
				command = tokens.front();
				tokens.pop();

				if (command == "loadMesh") {
					loadMesh(tokens);
				}
				else if (command == "particleGen") {
					generateParticles(tokens);
				}
				else if (command == "moveShutter") {
				}
				else if (command == "simulate") {
					simulate(tokens);
				}
				else if (command == "render")
				{
					render();
				}
				else if (command == "help" || command == "?") {
					showHelp();
				}
				else if (command == "exit") {
					acmd.aWriteCmd(CUI::ConsoleCommand::EXIT);
				}
				else {
					std::cout << "Unknown command. Enter 'help' to view a list of all available commands." << std::endl;
				}
			}
		}
	}

	//AsyncCommand
	AsyncCommand::AsyncCommand() :
		command(ConsoleCommand::NONE), param("")
	{
	}

	ConsoleCommand AsyncCommand::aReadCmd()
	{
		std::lock_guard<std::mutex> guard(this->cmdLock);
		return this->command;
	}

	void AsyncCommand::aWriteCmd(ConsoleCommand cmd)
	{
		std::lock_guard<std::mutex> guard(this->cmdLock);
		this->command = cmd;
	}

	ConsoleCommand AsyncCommand::aReadCmd(std::string & param)
	{
		std::lock_guard<std::mutex> guard(this->cmdLock);
		param = this->param;
		return this->command;
	}

	void AsyncCommand::aWriteCmd(ConsoleCommand cmd, std::string param)
	{
		std::lock_guard<std::mutex> guard(this->cmdLock);
		this->command = cmd;
		this->param = param;
	}
}
