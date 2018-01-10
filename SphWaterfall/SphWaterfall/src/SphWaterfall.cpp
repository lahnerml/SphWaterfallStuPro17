#include <iostream>
#include <string>
#include <sstream>
#include <vector>

void trim(std::string &str) {
	int pos1 = str.find_first_not_of(" ");
	int pos2 = str.find_last_not_of(" ");
	str = str.substr(pos1, pos2 - pos1 + 1);
}

bool readParameter(char paramName, std::istringstream &parameters)
{
	std::string nameToken, valueToken;
	return (parameters >> nameToken && nameToken == "-" + paramName);
}

char readParameter(std::istringstream &parameterStream, std::vector<std::string> &values)
{
	std::string nameToken, valueToken;
	
	if (parameterStream.peek() != '-')
	{
		std::cout << "Wrong token '" << parameterStream.peek() << "'." << std::endl;
		return EFAULT;
	}

	// Read parameter name
	if (!(parameterStream >> nameToken))
		return EFAULT;

	//Read values
	while (parameterStream.peek() != '-' && parameterStream.peek() != EOF)
	{
		parameterStream >> valueToken;
		values.push_back(valueToken);
	}

	std::cout << nameToken << std::endl;
	return nameToken[1];
}

/* -_-_-_Comands Begin_-_-_- */

void loadMesh(std::istringstream &parameters)
{
	std::vector<std::string> fileName;
	if (readParameter(parameters, fileName) != 'p') {
		std::cout << "Missing path parameter '-p'" << std::endl;
	}
}

void showHelp()
{
	std::cout << "loadMesh -p" << std::endl;
	std::cout << "particleGen [-w] [-f] [-e]" << std::endl;
	std::cout << "moveShutter -t (-u/-d) [-l]" << std::endl;
	std::cout << "simulate -s -e -r -g -m -t" << std::endl;
	std::cout << "help" << std::endl;
	std::cout << "exit" << std::endl;
}

/* -_-_-_Comands End_-_-_- */

void readCommands()
{
	std::string inputLine, command;

	while (true)
	{
		std::cout << std::endl << "Please enter a command or enter 'help' to show a list of all commands" << std::endl;
		
		std::getline(std::cin, inputLine);
		trim(inputLine);
		std::istringstream tokenStream(inputLine);

		if (tokenStream >> command)
		{
			if (command == "loadMesh")
				loadMesh(tokenStream);
			else if (command == "help" || command == "?")
				showHelp();
			else if (command == "exit")
				break;
			else
				std::cout << "Unknown command. Enter 'hlep' to view a list of all available commands." << std::endl;
		}
	}
}

int main()
{
	readCommands();
}