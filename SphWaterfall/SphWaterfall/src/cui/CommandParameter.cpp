#include "CommandParameter.h"

CommandParameter::CommandParameter() :
	parameter_name(""), 
	value (""){

}

CommandParameter::CommandParameter(std::string parameter_name, std::string value) :
	parameter_name(parameter_name),
	value(value) {

}

std::string& CommandParameter::getParameterName() {
	return this->parameter_name;
}

std::string& CommandParameter::getValue() {
	return this->value;
}
