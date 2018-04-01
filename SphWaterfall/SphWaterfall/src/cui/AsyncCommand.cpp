#include "AsyncCommand.h"

AsyncCommand::AsyncCommand() :
	command(ConsoleCommand::NONE), parameter("") {
}

AsyncCommand::ConsoleCommand AsyncCommand::readCommand()
{
	std::string parameter = "";
	return readCommand(parameter);
}

AsyncCommand::ConsoleCommand AsyncCommand::readCommand(std::string & parameter)
{
	ConsoleCommand command;
	{
		std::unique_lock<std::mutex> guard(this->commandLock);
		conditionVariable.wait(guard, [this] { return !this->isCommandEmpty(); });
	}

	{
		std::lock_guard<std::mutex> guard(this->commandLock);
		parameter = this->parameter;
		command = this->command;
		this->command = ConsoleCommand::NONE;
	}
	conditionVariable.notify_one();

	return command;
}

void AsyncCommand::writeCommand(ConsoleCommand command)
{
	std::string temp = "";
	writeCommand(command, temp);
}

void AsyncCommand::writeCommand(ConsoleCommand command, std::string parameter)
{
	if (command == ConsoleCommand::NONE) {
		return;
	}

	{
		std::lock_guard<std::mutex> guard (this->commandLock);
		this->command = command;
		this->parameter = parameter;
	}
	this->conditionVariable.notify_one();
	{
		std::unique_lock<std::mutex> guard = std::unique_lock<std::mutex>(this->commandLock);
		conditionVariable.wait(guard, [this] { return this->isCommandEmpty(); });
	}
}

bool AsyncCommand::isCommandEmpty() {
	return this->command == ConsoleCommand::NONE;
}
