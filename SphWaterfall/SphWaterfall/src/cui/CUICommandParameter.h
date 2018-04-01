#pragma once
#include <iostream>

class CUICommandParameter {
	public:
		CUICommandParameter();
		CUICommandParameter(std::string parameter_name, std::string value);

		std::string& getParameterName();
		std::string& getValue();
	private:
		std::string parameter_name;
		std::string value;
};
