#include "CommandReader.h"
#include "TaskManager.h"
#include <iostream>
#include <string>

CommandReader::CommandReader()
{
	commandsSet.push_back("load");
	commandsSet.push_back("save");
	commandsSet.push_back("help");
	commandsSet.push_back("find");
	commandsSet.push_back("num_subordinates");
	commandsSet.push_back("manager");
	commandsSet.push_back("num_employees");
	commandsSet.push_back("overloaded");
	commandsSet.push_back("join");
	commandsSet.push_back("hire");
	commandsSet.push_back("fire");
	commandsSet.push_back("longest_chain"); 
	commandsSet.push_back("salary");
	commandsSet.push_back("incorporate");
	commandsSet.push_back("modernize");
	commandsSet.push_back("exit");

}

void CommandReader::print()
{
	for (size_t i = 0; i < commandsSet.size(); i++)
	{
		std::cout << commandsSet[i] << " ";
	}
}

void CommandReader::start()
{
	TaskManager manager;
	do
	{
		bool isCommand = false;
		char temp[256];
		std::cin.getline(temp, 256);
		command = temp;
		split();

		size_t i = 0;
		for (i; i < commandsSet.size(); i++)
		{
			if (splittedCommand[0] == commandsSet[i])
			{
				isCommand = true;
				break;
			}
		}

		if (!isCommand)
		{
			std::cout << "Invalid command! Type help for more info." << std::endl;
		}

		else {
			switch (i)
			{
			case 0: manager.loadLoader(splittedCommand); break;
			case 1: manager.saveLoader(splittedCommand); break;
			case 2: manager.help(); break;
			case 3: manager.find(splittedCommand); break;
			case 4: manager.num_subordinates(splittedCommand); break;
			case 5: manager.manager(splittedCommand); break;
			case 6: manager.num_employees(splittedCommand); break;
			case 7: manager.overloaded(splittedCommand); break;
			case 8: manager.join(splittedCommand); break;
			case 9: manager.hire(splittedCommand); break;
			case 10: manager.fire(splittedCommand); break;
			case 11: manager.longest_chain(splittedCommand); break;
			case 12: manager.salary(splittedCommand); break;
			case 13: manager.incorporate(splittedCommand); break;
			case 14: manager.modernize(splittedCommand); break;	
			default:
				break;
			}
		}

	} while (!(command == "exit"));

	manager.askEditedObjectsForSave();
}


void CommandReader::split()
{
	splittedCommand.clear();
	std::string currentWord = "";
	for (size_t i = 0; i < command.length(); i++)
	{
		if (command[i] == ' ')
		{
			splittedCommand.push_back(currentWord);
			currentWord = "";
		}

		else {
			currentWord += command[i];

			if (i == command.length() - 1)
			{
				splittedCommand.push_back(currentWord);
			}
		}
	}
}

