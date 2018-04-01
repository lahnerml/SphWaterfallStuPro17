#pragma once
#include <iostream>
#include <vector>

#include "CUICommandParameter.h"

class CUICommand {
	public:
		CUICommand();
		CUICommand(std::string parameter_name, std::string input_line);

		std::string& getInputLine();
		std::string& getCommand();
		std::vector<CUICommandParameter>& getParameterList();

		void addParameter(CUICommandParameter parameter);
	private:
		std::string input_line;
		std::string command;
		std::vector<CUICommandParameter> parameter_list;
};
