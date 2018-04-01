#include "CUI.h"
//#include "../visualization/VisualizationManager.h"

namespace CUI {

	AsyncCommand asyncCommand;
	CUICommand current_command;

	void startCUI()
	{
		startWithStream(std::cin, true);
	}

	void startWithStream(std::istream &inputStream, bool broadcastExitCmd)
	{
		std::string input_line, command;
		printInputMessage();

		while (true) {
			//Read command
			getline(inputStream, input_line);
			trim(input_line);

			if (input_line == "") {
				continue;
			}

			std::cout << "Input line: " << input_line << std::endl;

			//Tokenize command
			std::istringstream command_token_stream = std::istringstream(input_line);
			NullableWrapper<std::string> last_read_parameter_name, last_read_parameter_value;
			bool first_loop_done = false;
			while (command_token_stream >> command) {
				if (!first_loop_done) {
					std::transform(command.begin(), command.end(), command.begin(), ::tolower);
					current_command = CUICommand(command, input_line);
					first_loop_done = true;
					continue;
				}

				bool isNewParameter = command.front() == '-';

				if (!isNewParameter) {
					if (!last_read_parameter_value.isNull()) {
						last_read_parameter_value.set(last_read_parameter_value.getInternal() + " " + command);
					}
					else {
						last_read_parameter_value.set(command);
					}
				}

				if ((command_token_stream.rdbuf()->in_avail() == 0) || (!last_read_parameter_name.isNull() && isNewParameter)) {
					current_command.addParameter(CommandParameter(last_read_parameter_name.getInternal(), trimQuotemarks(last_read_parameter_value.getInternal())));
					last_read_parameter_name.reset();
					last_read_parameter_value.reset();
				}
				if (isNewParameter) {
					last_read_parameter_name.set(command);
					last_read_parameter_value.reset();
				}
			}

			//for (auto parameter : current_command.getParameterList()) {
			//	std::cout << "command: " << current_command.getCommand() << " parameter name: " << parameter.getParameterName() 
			//		<< " parameter value: " << parameter.getValue() << std::endl;
			//}
			std::queue<std::string> tokens;
			//Execute command
			if (current_command.getCommand().front() == '#') {
				//Comment
			}
			else if (current_command.getCommand() == "print") {
				printCommand();
			}
			else if (current_command.getCommand() == "loadmesh") {
				loadMesh();
				printInputMessage();
			}
			else if (current_command.getCommand() == "loadshutter") {
				loadShutter();
				printInputMessage();
			}
			else if (current_command.getCommand() == "particlegen") {
				asyncCommand.writeCommand(AsyncCommand::GENERATE_PARTICLES);
				printInputMessage();
			}
			else if (current_command.getCommand() == "moveshutter") {
				moveShutter();
				printInputMessage();
			}
			else if (current_command.getCommand() == "simulate") {
				asyncCommand.writeCommand(AsyncCommand::SIMULATE);
				printInputMessage();
			}
			else if (current_command.getCommand() == "render")
			{
				asyncCommand.writeCommand(AsyncCommand::RENDER);
				printInputMessage();
			}
			else if (current_command.getCommand() == "loadconfig")
			{
				loadConfig();
				printInputMessage();
			}
			else if (current_command.getCommand() == "addsource")
			{
				addSource();
				printInputMessage();
			}
			else if (current_command.getCommand() == "addsink")
			{
				addSink();
				printInputMessage();
			}
			else if (current_command.getCommand() == "help" || current_command.getCommand() == "?") {
				showHelp();
				printInputMessage();
			}
			else if (current_command.getCommand() == "exit") {
				if (broadcastExitCmd)
					asyncCommand.writeCommand(AsyncCommand::EXIT);
				break;
			}
			else {
				std::cout << "Unknown command. Enter 'help' to view a list of all available commands." << std::endl;
			}
		}
	}

	void trim(std::string &string) {
		int positionLeft = string.find_first_not_of(" ");
		int positionRight = string.find_last_not_of(" ");
		if (!(positionLeft == -1 || positionRight == -1)) {
			string = string.substr(positionLeft, positionRight - positionLeft + 1);
		}
	}

	std::string trimQuotemarks(std::string string) {
		int positionLeft = string.find_first_not_of("\"");
		int positionRight = string.find_last_not_of("\"");
		if (!(positionLeft == -1 || positionRight == -1)) {
			return string.substr(positionLeft, positionRight - positionLeft + 1);
		}
	}

	bool readNextToken(std::queue<std::string> &tokens, std::string &nextToken) {
		if (!tokens.empty()) {
			nextToken = tokens.front();
			tokens.pop();
			return true;
		}
		return false;
	}

	bool readNextCombinedToken(std::queue<std::string> &tokens, std::string &nextToken) {
		std::string tempToken = "", resultToken = "";

		//Read first token
		if (!readNextToken(tokens, tempToken)) {
			return false;
		}

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

	void printInputMessage() {
		std::cout << std::endl << "Please enter a command or enter 'help' to show a list of all commands" << std::endl;
	}
	
	/* -_-_-_Commands Begin_-_-_- */

	void printCommand()
	{
		if (current_command.getInputLine().substr(0, 6) == "print ") {
			std::cout << "-> " << current_command.getInputLine().replace(0, 6, "") << std::endl;
		}
		else {
			std::cout << "-> " << current_command.getInputLine() << std::endl;
		}
	}

	void loadMesh()
	{
		for (CommandParameter& parameter : current_command.getParameterList()) {
			if (parameter.getParameterName() == "-p") {
				std::string file_path = parameter.getValue();
				asyncCommand.writeCommand(AsyncCommand::LOAD_MESH, file_path);
			}
			else
			{
				std::cout << "Missing path parameter '-p'" << std::endl;
			}
		}
	}

	void loadShutter()
	{
		for (CommandParameter& parameter : current_command.getParameterList()) {
			if (parameter.getParameterName() == "-p") {
				std::string file_path = parameter.getValue();
				asyncCommand.writeCommand(AsyncCommand::LOAD_SHUTTER, file_path);
			}
			else
			{
				std::cout << "Missing path parameter '-p'" << std::endl;
			}
		}
	}

	void moveShutter()
	{
		for (CommandParameter& parameter : current_command.getParameterList()) {
			if (parameter.getParameterName() == "-t") {
				std::string time_for_move = parameter.getValue();
				if (time_for_move.find_first_not_of("0123456789") == std::string::npos) {
					asyncCommand.writeCommand(AsyncCommand::MOVE_SHUTTER, time_for_move);
				}
				else {
					std::cout << "'" << parameter.getValue() << "' is not a number" << std::endl;
				}
			}
			else
			{
				std::cout << "Missing path parameter '-t'" << std::endl;
			}
		}
	}

	void loadConfig()
	{
		for (CommandParameter& parameter : current_command.getParameterList()) {
			if (parameter.getParameterName() == "-p") {
				std::string file_path = parameter.getValue();
				std::ifstream in(file_path);
				if (!in)
				{
					std::cerr << "Cannot open \"" << file_path << "\"" << std::endl;
				}
				else
				{
					std::cerr << "Config file \"" << file_path << "\" loaded and running..." << std::endl;
					startWithStream(in, false);
					std::cout << "Done reading config file." << std::endl;
				}
			}
			else
			{
				std::cout << "Missing path parameter '-p'" << std::endl;
			}
		}
	}

	void addSource()
	{
		for (CommandParameter& parameter : current_command.getParameterList()) {
			if (parameter.getParameterName() == "-v") {
				std::string source_position = parameter.getValue();

				if (source_position.find_first_not_of("+-,.0123456789 ") != std::string::npos)
				{
					std::cout << "'" << source_position << "' is not a valid input" << std::endl;
					return;
				}

				int spaces = 0;
				for (char character : source_position) {
					if (character == ' ') {
						spaces++;
					}
				}

				if (spaces != 2) {
					std::cout << "'" << source_position << "' is not a valid input" << std::endl;
					return;
				}

				asyncCommand.writeCommand(AsyncCommand::ADD_SOURCE, source_position);
			}
			else
			{
				std::cout << "Missing path parameter '-v'" << std::endl;
			}
		}
	}

	void addSink()
	{
		for (CommandParameter& parameter : current_command.getParameterList()) {
			if (parameter.getParameterName() == "-h") {
				std::string sink_height = parameter.getValue();
				if (sink_height.find_first_not_of("+-,.0123456789") == std::string::npos) {
					asyncCommand.writeCommand(AsyncCommand::ADD_SINK, sink_height);
				}
				else {
					std::cout << "'" << sink_height << "' is not a number" << std::endl;
				}
			}
			else
			{
				std::cout << "Missing path parameter '-h'" << std::endl;
			}
		}
	}

	void showHelp()
	{
		std::cout
			<< "print" << std::endl
			<< "loadmesh -p ..." << std::endl
			<< "particlegen" << std::endl
			<< "moveshutter -t 0" << std::endl
			<< "simulate -t 0" << std::endl
			<< "render" << std::endl
			<< "loadconfig -p ..." << std::endl
			<< "addsource -v x y z" << std::endl
			<< "addsink -h 0" << std::endl
			<< "help" << std::endl
			<< "exit" << std::endl;
	}
	/* -_-_-_Commands End_-_-_- */
}