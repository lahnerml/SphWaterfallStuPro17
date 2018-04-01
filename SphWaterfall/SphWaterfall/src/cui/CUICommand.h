#pragma once
#include <iostream>
#include <vector>

#include "CUICommandParameter.h"

class CUICommand {
	public:
		enum Command
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

		CUICommand();
		CUICommand(std::string parameter_name, std::string input_line);

		std::string& getInputLine();
		Command& getCommand();
		void setCommand(Command command);
		std::string& getCommandName();
		std::vector<CUICommandParameter>& getParameterList();
		CUICommandParameter& getParameter(int index);
		void addParameter(CUICommandParameter parameter);
		bool removeParameter(CUICommandParameter parameter);
		bool removeParameter(std::string parameter_name);
	private:
		std::string input_line;
		Command command;
		std::string command_name;
		std::vector<CUICommandParameter> parameter_list;
};
