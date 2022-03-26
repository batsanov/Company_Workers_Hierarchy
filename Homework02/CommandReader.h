#pragma once
#include "vector"
#include "string"

class CommandReader {
private:
	std::vector<std::string> commandsSet;
	std::string command;
	std::vector<std::string> splittedCommand;
public:
	CommandReader();
	void print();
	void start();
	void split();

};
