#pragma once
#include <iostream>

class CommandParameter {
	public:
		CommandParameter();
		CommandParameter(std::string parameter_name, std::string value);

		std::string& getParameterName();
		std::string& getValue();
	private:
		std::string parameter_name;
		std::string value;
};
