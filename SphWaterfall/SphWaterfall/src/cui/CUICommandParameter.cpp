#include "CUICommandParameter.h"

CUICommandParameter::CUICommandParameter() :
	parameter_name(""), 
	value (""){

}

CUICommandParameter::CUICommandParameter(std::string parameter_name, std::string value) :
	parameter_name(parameter_name),
	value(value) {

}

bool operator==(const CUICommandParameter& first_parameter, const CUICommandParameter& second_parameter) {
	return ((first_parameter.getValue() == second_parameter.getValue()) && (first_parameter.getParameterName() == second_parameter.getParameterName()));
}

int CUICommandParameter::sizeInByte() {
	return sizeof(parameter_name) + sizeof(value);
}

std::string CUICommandParameter::getParameterName() const {
	return this->parameter_name;
}

std::string CUICommandParameter::getValue() const{
	return this->value;
}
