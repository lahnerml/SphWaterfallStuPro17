#include <iostream>
#include <string>
#include <sstream>

int main()
{
	char a;
	std::cout << "Hello World!" << std::endl;
	std::cin >> a;
	// Hallo Welt
}

void trim(std::string &str) {
	int pos1 = str.find_first_not_of(" ");
	int pos2 = str.find_last_not_of(" ");
	str = str.substr(pos1, pos2 - pos1 + 1);
}

void readCommand()
{
	std::string inputLine;
	std::getline(std::cin, inputLine);
	trim(inputLine);
	
	std::istringstream commandTokens(inputLine);
	std::string command;

	while (commandTokens >> command)
	{
		
	}
}
