#include "CUI.h"
//#include "../visualization/VisualizationManager.h"

namespace CUI {

	AsyncCommand acmd;

	void trim(std::string &str) {
		int pos1 = str.find_first_not_of(" ");
		int pos2 = str.find_last_not_of(" ");
		if (pos1 == -1 || pos2 == -1)
			return;
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
		std::string tempToken = "", resultToken = "";

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

	void printCommand(std::queue<std::string> &tokens)
	{
		std::string token;

		std::cout << "-> ";
		while(readNextToken(tokens, token)) {
			std::cout << token << " ";
		}
		std::cout << std::endl;
	}

	void loadMesh(std::queue<std::string> &tokens)
	{
		std::string paramName, fileName;

		//Read fileName Parameter
		if (readNextToken(tokens, paramName) && paramName == "-p") {
			fileName = "";
			readNextCombinedToken(tokens, fileName);

			acmd.writeCmd(CUI::ConsoleCommand::LOAD_MESH, fileName);
		}
		else
		{
			std::cout << "Missing path parameter '-p'" << std::endl;
		}
	}

	void moveShutter(std::queue<std::string> &tokens)
	{
		std::string paramName, shutterMoveFrame;

		//Read sinkHeight Parameter
		if (readNextToken(tokens, paramName) && paramName == "-t") {
			shutterMoveFrame = "";
			readNextToken(tokens, shutterMoveFrame);

			if (shutterMoveFrame.find_first_not_of("0123456789") == std::string::npos)
				acmd.writeCmd(CUI::ConsoleCommand::MOVE_SHUTTER, shutterMoveFrame);
			else
				std::cout << "'" << shutterMoveFrame << "' is not a number" << std::endl;
		}
		else
		{
			std::cout << "Missing parameter '-t'" << std::endl;
		}
	}

	void loadConfig(std::queue<std::string> &tokens)
	{
		std::string paramName, fileName;

		if (!readNextToken(tokens, paramName) || paramName != "-p")
			return;

		fileName = "";
		readNextCombinedToken(tokens, fileName);
		
		std::ifstream in(fileName);
		if (!in)
		{
			std::cerr << "Cannot open \"" << fileName << "\"" << std::endl;
		}
		else
		{
			std::cerr << "Config file \"" << fileName << "\" loaded and running..." << std::endl;
			startWithStream(in, false);
			std::cout << "Done reading config file." << std::endl;
		}
	}

	void addSource(std::queue<std::string> &tokens)
	{
		std::string paramName, sourcePos, sourcePosDim;

		//Read sinkHeight Parameter
		if (readNextToken(tokens, paramName) && paramName == "-v") {
			sourcePos = "";
			for (int i = 0; i < 3; i++)
			{
				sourcePosDim = "";
				if (!readNextToken(tokens, sourcePosDim))
				{
					std::cout << "Not enough numbers for param -v" << std::endl;
					return;
				}

				if (sourcePosDim.find_first_not_of("+-,.0123456789") != std::string::npos)
				{
					std::cout << "'" << sourcePosDim << "' is not a number" << std::endl;
					return;
				}

				sourcePos += sourcePosDim + " ";
			}
			
			trim(sourcePos);
			acmd.writeCmd(CUI::ConsoleCommand::ADD_SOURCE, sourcePos);
		}
		else
		{
			std::cout << "Missing parameter '-v'" << std::endl;
		}
	}

	void addSink(std::queue<std::string> &tokens)
	{
		std::string paramName, sinkHeight;

		//Read sinkHeight Parameter
		if (readNextToken(tokens, paramName) && paramName == "-h") {
			sinkHeight = "";
			readNextToken(tokens, sinkHeight);

			if(sinkHeight.find_first_not_of("+-,.0123456789") == std::string::npos)
				acmd.writeCmd(CUI::ConsoleCommand::ADD_SINK, sinkHeight);
			else
				std::cout << "'" << sinkHeight << "' is not a number" << std::endl;
		}
		else
		{
			std::cout << "Missing parameter '-h'" << std::endl;
		}
	}

	void showHelp()
	{
		std::cout 
			<< "print" << std::endl
			<< "loadMesh -p ..." << std::endl
			<< "particleGen [-w] [-f] [-e]" << std::endl
			<< "moveShutter -t 0" << std::endl
			<< "simulate -t 0" << std::endl
			<< "render" << std::endl
			<< "loadConfig -p ..." << std::endl
			<< "addSource -v 0 0 0" << std::endl
			<< "addSink -h 0" << std::endl
			<< "help" << std::endl
			<< "exit" << std::endl;
	}

	/* -_-_-_Comands End_-_-_- */

	void startCUI()
	{
		startWithStream(std::cin, true);
	}

	void startWithStream(std::istream &inputStream, bool broadcastExitCmd)
	{
		std::string inputLine, command;
		std::queue<std::string> tokens;
		acmd.printInputMessage();

		while (true)
		{
			//Read command
			getline(inputStream, inputLine);
			trim(inputLine);

			//Tokenize command
			if (inputLine == "")
				continue;
			std::istringstream tokenStream(inputLine);
			tokens = std::queue<std::string>();
			while (tokenStream >> command)
				tokens.push(command);

			//Execute command
			if (!tokens.empty())
			{
				command = tokens.front();
				tokens.pop();

				if (command == "#") {
					//Comment
				}
				else if (command == "print") {
					printCommand(tokens);
				}
				else if (command == "loadMesh") {
					loadMesh(tokens);
					acmd.printInputMessage();
				}
				else if (command == "particleGen") {
					acmd.writeCmd(CUI::ConsoleCommand::GENERATE_PARTICLES);
					acmd.printInputMessage();
				}
				else if (command == "moveShutter") {
					moveShutter(tokens);
					acmd.printInputMessage();
				}
				else if (command == "simulate") {
					acmd.writeCmd(CUI::ConsoleCommand::SIMULATE);
					acmd.printInputMessage();
				}
				else if (command == "render")
				{
					acmd.writeCmd(CUI::ConsoleCommand::RENDER);
					acmd.printInputMessage();
				}
				else if (command == "loadConfig")
				{
					loadConfig(tokens);
					acmd.printInputMessage();
				}
				else if (command == "addSource")
				{
					addSource(tokens);
					acmd.printInputMessage();
				}
				else if (command == "addSink")
				{
					addSink(tokens);
					acmd.printInputMessage();
				}
				else if (command == "help" || command == "?") {
					showHelp();
					acmd.printInputMessage();
				}
				else if (command == "exit") {
					if(broadcastExitCmd)
						acmd.writeCmd(CUI::ConsoleCommand::EXIT);
					break;
				}
				else {
					std::cout << "Unknown command. Enter 'help' to view a list of all available commands." << std::endl;
				}
			}
		}
	}

	void AsyncCommand::printInputMessage() {
		std::cout << std::endl << "Please enter a command or enter 'help' to show a list of all commands" << std::endl;
	}

	//AsyncCommand
	AsyncCommand::AsyncCommand() :
		command(ConsoleCommand::NONE), param("")
	{
	}

	ConsoleCommand AsyncCommand::readCmd()
	{
		std::string temp = "";
		return readCmd(temp);
	}

	void AsyncCommand::writeCmd(ConsoleCommand cmd)
	{
		std::string temp = "";
		writeCmd(cmd, temp);
	}

	ConsoleCommand AsyncCommand::readCmd(std::string & param)
	{
		CUI::ConsoleCommand cmd;
		{
			std::unique_lock<std::mutex> guard(this->cmdLock);
			cv.wait(guard, [this] { return !this->commandEmpty(); });
		}

		{
			std::lock_guard<std::mutex> guard(this->cmdLock);
			param = this->param;
			cmd = this->command;
			this->command = CUI::ConsoleCommand::NONE;
		}
		cv.notify_one();

		return cmd;
	}

	void AsyncCommand::writeCmd(ConsoleCommand cmd, std::string param)
	{
		if (cmd == CUI::ConsoleCommand::NONE)
			return;

		{
			std::lock_guard<std::mutex> guard(this->cmdLock);

			this->command = cmd;
			this->param = param;
		}
		this->cv.notify_one();

		{
			std::unique_lock<std::mutex> guard(this->cmdLock);
			cv.wait(guard, [this] { return this->commandEmpty(); });
		}
	}

	bool AsyncCommand::commandEmpty() {
		return this->command == CUI::ConsoleCommand::NONE;
	}
}
