#include "CUI.h"

CUI::CUI(CommandHandler command_handler) :
	command_handler(command_handler){

}

void CUI::start() {
	startWithStream(std::cin, true);
}

void CUI::startWithStream(std::istream &inputStream, bool broadcastExitCmd) {
	std::string input_line;

	printInputMessage();

	exit_programm = false;
	while (!exit_programm) {
		// read command_name
		getline(inputStream, input_line);
		trim(input_line);

		if (input_line == "") {
			continue;
		}

		// parses command to CUICommand object
		parseCommand(input_line);

		// clean command from not relevant parameters for each command and executes them
		cleanAndExecuteCommand(broadcastExitCmd);
	}
}

void CUI::parseCommand(std::string input_line) {
	std::string command;
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

		// is new parameter when string starts with '-' and there are no numbers in it
		bool isNewParameter = ((command.front() == '-') && (command.find_first_of("0123456789") == std::string::npos));

		if (!isNewParameter) {
			if (!last_read_parameter_value.isNull()) {
				last_read_parameter_value.set(last_read_parameter_value.getInternal() + " " + command);
			}
			else {
				last_read_parameter_value.set(command);
			}
		}

		if ((command_token_stream.rdbuf()->in_avail() == 0) || (!last_read_parameter_name.isNull() && isNewParameter)) {
			current_command.addParameter(CUICommandParameter(last_read_parameter_name.getInternal(), trimQuotemarks(last_read_parameter_value.getInternal())));
			last_read_parameter_name.reset();
			last_read_parameter_value.reset();
		}
		if (isNewParameter) {
			last_read_parameter_name.set(command);
			last_read_parameter_value.reset();
		}
	}
}

void CUI::cleanAndExecuteCommand(bool broadcastExitCmd) {
	std::string command = current_command.getCommandName();
	if (command.front() == '#') {
		//Comment
	}
	else if (command == "print") {
		printCommand();
	}
	else if (command == "loadmesh") {
		if (cleanLoadMesh()) {
			current_command.setCommand(CUICommand::LOAD_MESH);
			command_handler.handleCUICommand(current_command);
		}
		printInputMessage();
	}
	else if (command == "loadshutter") {
		if (cleanLoadShutter()) {
			current_command.setCommand(CUICommand::LOAD_SHUTTER);
			command_handler.handleCUICommand(current_command);
		}
		printInputMessage();
	}
	else if (command == "moveshutter") {
		if (cleanMoveShutter()) {
			current_command.setCommand(CUICommand::MOVE_SHUTTER);
			command_handler.handleCUICommand(current_command);
		}
		printInputMessage();
	}
	else if (command == "addsource") {
		if (cleanAddSource()) {
			current_command.setCommand(CUICommand::ADD_SOURCE);
			command_handler.handleCUICommand(current_command);
		}
		printInputMessage();
	}
	else if (command == "addsink") {
		if (cleanAddSink()) {
			current_command.setCommand(CUICommand::ADD_SINK);
			command_handler.handleCUICommand(current_command);
		}
		printInputMessage();
	}
	else if (command == "particlegen") {
		current_command.setCommand(CUICommand::GENERATE_PARTICLES);
		command_handler.handleCUICommand(current_command);
		printInputMessage();
	}
	else if (command == "simulate") {
		current_command.setCommand(CUICommand::SIMULATE);
		command_handler.handleCUICommand(current_command);
		printInputMessage();
	}
	else if (command == "render")
	{
		current_command.setCommand(CUICommand::RENDER);
		command_handler.handleCUICommand(current_command);
		printInputMessage();
	}
	else if (command == "loadconfig")
	{
		loadConfig();
		printInputMessage();
	}
	else if (command == "help" || command == "?") {
		showHelp();
		printInputMessage();
	}
	else if (command == "exit") {
		if (broadcastExitCmd) {
			exit_programm = true;
			current_command.setCommand(CUICommand::EXIT);
			command_handler.handleCUICommand(current_command);
		}
	}
	else {
		std::cout << "Unknown command. Enter 'help' to view a list of all available commands." << std::endl;
	}
}

void CUI::trim(std::string &string) {
	int positionLeft = string.find_first_not_of(" ");
	int positionRight = string.find_last_not_of(" ");
	if (!(positionLeft == -1 || positionRight == -1)) {
		string = string.substr(positionLeft, positionRight - positionLeft + 1);
	}
}

std::string CUI::trimQuotemarks(std::string string) {
	int positionLeft = string.find_first_not_of("\"");
	int positionRight = string.find_last_not_of("\"");
	if (!(positionLeft == -1 || positionRight == -1)) {
		return string.substr(positionLeft, positionRight - positionLeft + 1);		}
}

void CUI::printInputMessage() {
	std::cout << std::endl << "Please enter a command or enter 'help' to show a list of all commands" << std::endl;
}
	
/* -_-_-_Commands Begin_-_-_- */

void CUI::printCommand() {
	if (current_command.getInputLine().substr(0, 6) == "print ") {
		std::cout << "-> " << current_command.getInputLine().replace(0, 6, "") << std::endl;
	}
	else {
		std::cout << "-> " << current_command.getInputLine() << std::endl;
	}
}

void CUI::showHelp() {
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

void CUI::loadConfig() {
	for (CUICommandParameter& parameter : current_command.getParameterList()) {
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
			current_command.removeParameter(parameter);
			std::cout << "Missing path parameter '-p'" << std::endl;
		}
	}
}

bool CUI::cleanLoadMesh() {
	bool hasOnlyValidParameters = true;

	for (CUICommandParameter& parameter : current_command.getParameterList()) {
		if (parameter.getParameterName() != "-p") {
			current_command.removeParameter(parameter);
			hasOnlyValidParameters = false;
			std::cout << "Missing path parameter '-p'" << std::endl;
		}
	}

	return hasOnlyValidParameters;
}

bool CUI::cleanLoadShutter() {
	bool hasOnlyValidParameters = true;

	for (CUICommandParameter& parameter : current_command.getParameterList()) {
		if (parameter.getParameterName() != "-p") {
			current_command.removeParameter(parameter);
			hasOnlyValidParameters = false;
			std::cout << "Missing path parameter '-p'" << std::endl;
		}
	}

	return hasOnlyValidParameters;
}

bool CUI::cleanMoveShutter() {
	bool hasOnlyValidParameters = true;

	for (CUICommandParameter& parameter : current_command.getParameterList()) {
		if (parameter.getParameterName() == "-t") {
			std::string time_for_move = parameter.getValue();
			if (time_for_move.find_first_not_of("0123456789") != std::string::npos) {
				hasOnlyValidParameters = false;
				std::cout << "'" << parameter.getValue() << "' is not a number" << std::endl;
			}
		}
		else {
			current_command.removeParameter(parameter);
			hasOnlyValidParameters = false;
			std::cout << "Missing path parameter '-t'" << std::endl;
		}
	}

	return hasOnlyValidParameters;
}

bool CUI::cleanAddSource() {
	bool hasOnlyValidParameters = true;

	for (CUICommandParameter& parameter : current_command.getParameterList()) {
		if (parameter.getParameterName() == "-v") {
			std::string source_position = parameter.getValue();

			if (source_position.find_first_not_of("+-,.0123456789 ") != std::string::npos)
			{
				std::cout << "'" << source_position << "' is not a valid input" << std::endl;
				hasOnlyValidParameters = false;
			}

			int spaces = 0;
			for (char character : source_position) {
				if (character == ' ') {
					spaces++;
				}
			}

			if (spaces != 2) {
				std::cout << "'" << source_position << "' is not a valid input" << std::endl;
				hasOnlyValidParameters = false;
			}
		}
		else {
			current_command.removeParameter(parameter);
			hasOnlyValidParameters = false;
			std::cout << "Missing path parameter '-v'" << std::endl;
		}
	}

	return hasOnlyValidParameters;
}

bool CUI::cleanAddSink() {
	bool hasOnlyValidParameters = true;

	for (CUICommandParameter& parameter : current_command.getParameterList()) {
		if (parameter.getParameterName() == "-h") {
			std::string sink_height = parameter.getValue();
			if (sink_height.find_first_not_of("+-,.0123456789") != std::string::npos) {
				hasOnlyValidParameters = false;
				std::cout << "'" << sink_height << "' is not a number" << std::endl;
			}
		}
		else
		{
			current_command.removeParameter(parameter);
			hasOnlyValidParameters = false;
			std::cout << "Missing path parameter '-h'" << std::endl;
		}
	}

	return hasOnlyValidParameters;
}
/* -_-_-_Commands End_-_-_- */
