#pragma once
#include <iostream>
#include <vector>

#include "CommandParameter.h"

class CUICommand {
	public:
		CUICommand();
		CUICommand(std::string parameter_name, std::string input_line);

		std::string& getInputLine();
		std::string& getCommand();
		std::vector<CommandParameter>& getParameterList();

		void addParameter(CommandParameter parameter);
	private:
		std::string input_line;
		std::string command;
		std::vector<CommandParameter> parameter_list;
};
