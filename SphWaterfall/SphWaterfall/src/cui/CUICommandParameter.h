#pragma once
#include <iostream>
#include <string>

class CUICommandParameter {
	public:
		CUICommandParameter();
		CUICommandParameter(std::string parameter_name, std::string value);

		friend bool operator==(const CUICommandParameter&, const CUICommandParameter&);

		std::string getParameterName() const;
		std::string getValue() const;
	private:
		std::string parameter_name;
		std::string value;
};
