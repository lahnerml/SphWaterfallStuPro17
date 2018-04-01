#include "CUICommandParameter.h"

CUICommandParameter::CUICommandParameter() :
	parameter_name(""), 
	value (""){

}

CUICommandParameter::CUICommandParameter(std::string parameter_name, std::string value) :
	parameter_name(parameter_name),
	value(value) {

}

std::string& CUICommandParameter::getParameterName() {
	return this->parameter_name;
}

std::string& CUICommandParameter::getValue() {
	return this->value;
}
