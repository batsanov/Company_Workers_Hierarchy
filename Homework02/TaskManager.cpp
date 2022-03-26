#include "TaskManager.h"
#include <fstream>
#include <iostream>



void TaskManager::help()
{
	std::cout << "The following commands are supported :" << std::endl;
	std::cout << "load object_name file_name - loads hierarchy associated"
		<< "with object_name with given information by file_name"
		<< ". If there is no given file it is expected to get the information"
		<< "from console until(ctrl + z / ctrl + d) is met" << std::endl;
	std::cout << "save object_name file_name - saves the information about the hierarchy in the given file_name"
		         ". If there is no given file, it is expected to output in the console " << std::endl;
	std::cout << "find object_name employee_name - checks if in object_name exists employee_name" << std::endl;
	std::cout << "num_subordinates object_name employee_name - outputs the number of direct subordinates to employee_name" << std::endl;
	std::cout << "help - outputs list of supported commands" << std::endl;
	std::cout << "manager object_name employee_name - outputs the name of the director of employee_name" << std::endl;
	std::cout << "num_employees object_name - outputs the number of the employees in object_name" << std::endl;
	std::cout << "overloaded object_name - outputs the number of the employees, for whom the number of subordinates (direct or not) is bigger than 20" << std::endl;
	std::cout << "join object_name_1 object_name_2 object_name_result - unites 2 given objects to new one with name object_name_result" << std::endl;
	std::cout << "fire object_name employee_name - fires employee_name from object_name" << std::endl;
	std::cout << "hire object_name employee_name director_name - hires employee_name to direct subordinate to director_name" << std::endl;
	std::cout << "salary object_name employee_name - outputs employee_name's salary" << std::endl;
	std::cout << "incorporate object_name - incorporates object_name" << std::endl;
	std::cout << "modernize object_name - morednizes object_name" << std::endl;
	std::cout << "exit - finishes the program. For all new or changed objects asks if it is needed to be saved in file" << std::endl;

}

void TaskManager::load(std::string treeName, std::string fileName)
{
	std::ifstream in;
	in.open(fileName);
	if (in)
	{
		in.seekg(0, std::ios::end);
		size_t size = in.tellg();
		string buffer(size, ' ');
		in.seekg(0);
		in.read(&buffer[0], size);

		Hierarchy* hierarchy = new Hierarchy(buffer);
		hierarchy->setName(treeName);
		container.push_back(hierarchy);

		in.close();
	}
	else
	{
		std::cout << "File does not exist!\n";
	}
}

void TaskManager::load(std::string treeName)
{
	Hierarchy* hierarchy = new Hierarchy();
	hierarchy->addTheBoss();

	hierarchy->setName(treeName);

	std::string input;
	std::getline(std::cin, input);

	while (true)
	{
		if (std::cin.eof()) break;

		int i = 0, j = 0;
		char firstName[64];
		char secondName[64];
		int counter = 0;
		while (input[i] != '-')
		{

			if (input[i] == ' ')
			{
				i++;
				continue;
			}
			else
			{
				firstName[i] = input[i];
				i++;
				counter++;
			}
		}

		i++;
		std::string director(firstName, counter);
		counter = 0;

		while (input[i] != '\0')
		{
			if (input[i] == ' ')
			{
				i++;
				continue;
			}

			secondName[j++] = input[i];
			i++;
			counter++;
		}


		std::string worker(secondName, counter);

		hierarchy->hire(worker, director);

		std::getline(std::cin, input);
	}
	std::cin.clear();
	
	std::cout << treeName << " loaded successfully!\n";
	hierarchy->isModified = true;
	container.push_back(hierarchy);
}

void TaskManager::find(std::vector<std::string> splittedCommand)
{
	if (splittedCommand.size() > 3)
	{
		std::cout << "Invalid command parameters! Type help for more info!\n";
		return;
	}
	string objectName = splittedCommand[1];
	string employeeName = splittedCommand[2];
	Hierarchy* obj = getWantedObject(objectName);

	if (obj)
	{
		if (obj->find(employeeName))
		{
			std::cout << employeeName << " is employed in " << objectName << ".\n";
		}
		else
		{
			std::cout << "There is no " << employeeName << " in " << objectName << ".\n";
		}
		return;
	}
	return;
}

void TaskManager::num_employees(std::vector<std::string> splittedCommand)
{
	if (splittedCommand.size() > 2)
	{
		std::cout << "Invalid command parameters! Type help for more info!\n";
		return;
	}

	string objectName = splittedCommand[1];
	Hierarchy* obj = getWantedObject(objectName);
	if (obj)
	{
		int employees = obj->num_employees();
		if (employees == 0)
		{
			std::cout << "There aren't any employees in " << objectName;
		}
		else if (employees == 1) 
		{
			std::cout << "There is 1 employee in " << objectName;
		}
		else
		{
			std::cout << "There are " << employees << " employees in " << objectName;
		}

		std::cout << ".\n";
	}
}

void TaskManager::hire(std::vector<std::string> splittedCommand)
{
	if (splittedCommand.size() > 4)
	{
		std::cout << "Invalid command parameters! Type help for more info!\n";
		return;
	}
	string objectName = splittedCommand[1];
	string workerName = splittedCommand[2];
	string directorName = splittedCommand[3];
	

	Hierarchy* obj = getWantedObject(objectName);
	if(obj)
	{
		if (obj->hire(workerName, directorName))
		{
			obj->isModified = true;
			std::cout << workerName << " was hired.\n";
		}
		else std::cout << "Invalid data!\n";
	}
	
	
}

void TaskManager::fire(std::vector<std::string> splittedCommand)
{
	if (splittedCommand.size() > 3)
	{
		std::cout << "Invalid command parameters! Type help for more info!\n";
		return;
	}
	string objectName = splittedCommand[1];
	string workerName = splittedCommand[2];
	Hierarchy* obj = getWantedObject(objectName);

	if (obj)
	{
		if (obj->fire(workerName) )
		{
			std::cout << workerName << " is fired from " << objectName << "!\n";
			obj->isModified = true;
		}
		else
		{
			std::cout << workerName << " isn't fired from " << objectName << ", because he wasn't found!\n";
		}
	}
}

void TaskManager::num_subordinates(std::vector<std::string> splittedCommand) 
{
	if (splittedCommand.size() > 3)
	{
		std::cout << "Invalid command parameters! Type help for more info!\n";
		return;
	}

	string objectName = splittedCommand[1];
	string employeeName = splittedCommand[2];
	Hierarchy* obj = getWantedObject(objectName);

	if (obj)
	{
		int subordinates = obj->num_subordinates(employeeName);
		if (subordinates == -1)
		{
			std::cout << "There is no " << employeeName << " in " << objectName << ".";
		}
		else if (subordinates == 0)
		{
			std::cout << employeeName << " has no subordinates!";
		}
		else if (subordinates == 1)
		{
			std::cout << employeeName << " has 1 subordinate!";
		}
		else
		{
			std::cout << employeeName << " has " << obj->num_subordinates(employeeName) << " subordinates!";
		}
			std::cout << std::endl;
	}
}

void TaskManager::manager(std::vector<std::string> splittedCommand)
{
	if (splittedCommand.size() > 3)
	{
		std::cout << "Invalid command parameters! Type help for more info!\n";
		return;
	}

	string objectName = splittedCommand[1];
	string employeeName = splittedCommand[2];
	Hierarchy* obj = getWantedObject(objectName);
	
	if (obj)
	{
		string managerName = obj->manager(employeeName);

		if (managerName == "INVALID_MANAGER")
		{
			std::cout << "There is no " << employeeName << " in " << objectName << ".\n";
		}
		else if (managerName == "")
		{
			std::cout << "Uspeshnia hasn't got a manager.\n";
		}
		else
		{
			std::cout << "The manager of " << employeeName << " is " << managerName << ".\n";
		}
	}
}

void TaskManager::longest_chain(std::vector<std::string> splittedCommand)
{
	if (splittedCommand.size() > 2)
	{
		std::cout << "Invalid command parameters! Type help for more info!\n";
		return;
	}
	string objectName = splittedCommand[1];
	Hierarchy* obj = getWantedObject(objectName);
	std::cout << "The longest chain in " << objectName << " is " << obj->longest_chain() << "\n";
}

void TaskManager::overloaded(std::vector<std::string> splittedCommand)
{
	if (splittedCommand.size() > 2)
	{
		std::cout << "Invalid command parameters! Type help for more info!\n";
		return;
	}
	string objectName = splittedCommand[1];
	Hierarchy* obj = getWantedObject(objectName);

	if (obj)
	{
		int numOverloaded = obj->num_overloaded(20);

		if (numOverloaded == 0)
		{
			std::cout << "No overloaded employees in " << objectName << ".\n";
		}
		else if (numOverloaded == 1)
		{
			std::cout << "1 overloaded employee in " << objectName << ".\n";
		}
		else
		{
			std::cout << numOverloaded << " overloaded employee in " << objectName << ".\n";
		}
	}
}

void TaskManager::salary(std::vector<std::string> splittedCommand)
{
	if (splittedCommand.size() > 3)
	{
		std::cout << "Invalid command parameters! Type help for more info!\n";
		return;
	}
	string objectName = splittedCommand[1];
	string employeeName = splittedCommand[2];
	Hierarchy* obj = getWantedObject(objectName);
	if (obj)
	{
		std::cout << employeeName << "'s salary is " << obj->getSalary(employeeName) << "\n";
	}
}

void TaskManager::modernize(std::vector<std::string> splittedCommand)
{
	if (splittedCommand.size() > 2)
	{
		std::cout << "Invalid command parameters! Type help for more info!\n";
		return;
	}
	string objectName = splittedCommand[1];
	Hierarchy* obj = getWantedObject(objectName);

	if (obj)
	{
		obj->modernize();
		obj->isModified = true;
		std::cout << objectName << " modernized.\n";
	}
}

void TaskManager::incorporate(std::vector<std::string> splittedCommand)
{
	if (splittedCommand.size() > 2)
	{
		std::cout << "Invalid command parameters! Type help for more info!\n";
		return;
	}
	string objectName = splittedCommand[1];
	Hierarchy* obj = getWantedObject(objectName);

	if (obj) {
		obj->incorporate();
		obj->isModified = true;
		std::cout << objectName << " incorporated.\n";
	}

}

void TaskManager::save(std::string treeName, std::string fileName)
{
	Hierarchy* obj = getWantedObject(treeName);

	if (obj) {
		std::ofstream out;
		out.open(fileName);
		if (out)
		{
				out << obj->print();
				std::cout << treeName << " saved." << std::endl;
				obj->isModified = false;
		}
		else
		{
			std::cout << "An error occurred while saving the file! Please enter new command!" << std::endl;
		}
	}
	
}

void TaskManager::save(std::string treeName)
{
	Hierarchy* obj = getWantedObject(treeName);

	if (obj) {
		std::cout << obj->print();
	}
}

void TaskManager::join(std::vector<std::string> splittedCommand)
{
	if (splittedCommand.size() != 4)
	{
		std::cout << "Invalid parameters given! Type help for more information!\n";
		return;
	}

	Hierarchy* objOne = getWantedObject(splittedCommand[1]);
	Hierarchy* objTwo = getWantedObject(splittedCommand[2]);
	Hierarchy* result = new Hierarchy(objOne->join(*objTwo));

	result->isModified = true;
	result->setName(splittedCommand[3]);

	container.push_back(result);
	std::cout << splittedCommand[3] << " created.\n";
}



void TaskManager::loadLoader(std::vector<std::string> splittedCommand)
{
	if (splittedCommand.size() == 2)
	{
		string objectName = splittedCommand[1];
		load(objectName);
	}
	else if (splittedCommand.size() == 3)
	{
		string objectName = splittedCommand[1];
		string fileName = splittedCommand[2];
		load(objectName, fileName);
	}
	else
		std::cout << "Invalid parameters given! Type help for more information!\n";
}

void TaskManager::saveLoader(std::vector<std::string> splittedCommand)
{
	if (splittedCommand.size() == 2)
	{
		string objectName = splittedCommand[1];
		save(objectName);
	}
	else if (splittedCommand.size() == 3)
	{
		string objectName = splittedCommand[1];
		string fileName = splittedCommand[2];
		save(objectName, fileName);
	}
	else
		std::cout << "Invalid parameters given! Type help for more information!\n";
}

void TaskManager::askEditedObjectsForSave()
{
	for (size_t i = 0; i < container.size(); i++)
	{
		if (container[i]->isModified)
		{
			string fileName;
			std::cout << container[i]->name << " is modified, but not saved.\n"
				<< "Enter file name to save it: ";
			std::cin >> fileName;
			save(container[i]->name, fileName);
		}
		
	}
	std::cout << "Goodbye!";
}

Hierarchy* TaskManager::getWantedObject(string objectName)
{
	for (size_t i = 0; i < container.size(); i++)
	{
		if (container[i]->getName() == objectName)
		{
			return container[i];
		}
	}
	std::cout << objectName << " is an unknown office!" << std::endl;
	return nullptr;
}
