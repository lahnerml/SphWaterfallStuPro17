#pragma once
#include <iostream>
#include <mutex>
#include <condition_variable>

class AsyncCommand {
	public:
		enum ConsoleCommand
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

		AsyncCommand();

		ConsoleCommand readCommand();
		ConsoleCommand readCommand(std::string& parameter);

		void writeCommand(ConsoleCommand command);
		void writeCommand(ConsoleCommand command, std::string parameter);

	private:
		ConsoleCommand command;
		std::string parameter;
		std::mutex commandLock;
		std::condition_variable conditionVariable;

		bool isCommandEmpty();
};
