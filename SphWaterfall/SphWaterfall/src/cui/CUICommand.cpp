#include "CUICommand.h"

CUICommand::CUICommand() {

}

CUICommand::CUICommand(std::string command, std::string input_line) :
	command(command),
	input_line(input_line),
	parameter_list(std::vector<CUICommandParameter>()) {

}

std::string& CUICommand::getInputLine() {
	return this->input_line;
}

std::string& CUICommand::getCommand() {
	return this->command;
}

std::vector<CUICommandParameter>& CUICommand::getParameterList() {
	return this->parameter_list;
}

void CUICommand::addParameter(CUICommandParameter parameter) {
	parameter_list.push_back(parameter);
}