#include "CUICommand.h"

CUICommand::CUICommand() {

}

CUICommand::CUICommand(std::string command, std::string input_line) :
	command_name(command),
	input_line(input_line),
	parameter_list(std::vector<CUICommandParameter>()) {

}

std::string& CUICommand::getInputLine() {
	return this->input_line;
}

CUICommand::Command& CUICommand::getCommand() {
	return this->command;
}

std::string& CUICommand::getCommandName() {
	return this->command_name;
}

std::vector<CUICommandParameter>& CUICommand::getParameterList() {
	return this->parameter_list;
}

CUICommandParameter& CUICommand::getParameter(int index)
{
	return parameter_list.at(index);
}

void CUICommand::addParameter(CUICommandParameter parameter) {
	parameter_list.push_back(parameter);
}

bool CUICommand::removeParameter(CUICommandParameter parameter_to_remove) {
	for (int i = 0; i < parameter_list.size(); i++) {
		CUICommandParameter& parameter = parameter_list.at(i);
		if (parameter == parameter_to_remove) {
			parameter_list.erase(parameter_list.begin() + i);
			return true;
		}
	}
	return false;
}

bool CUICommand::removeParameter(std::string parameter_name) {
	for (int i = 0; i < parameter_list.size(); i++) {
		CUICommandParameter& parameter = parameter_list.at(i);
		if (parameter.getParameterName() == parameter_name) {
			parameter_list.erase(parameter_list.begin() + i);
			return true;
		}
	}
	return false;
}