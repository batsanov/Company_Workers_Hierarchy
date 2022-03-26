#pragma once
#include "interface.h"

class TaskManager {
private:
	std::vector<Hierarchy*> container;
public:
	TaskManager() = default;

	void help();
	void load(std::string treeName, std::string fileName); 
	void load(std::string treeName);
	void find(std::vector<std::string> splittedCommand);
	void num_employees(std::vector<std::string> splittedCommand);
	void hire(std::vector<std::string> splittedCommand);
	void fire(std::vector<std::string> splittedCommand);
	void num_subordinates(std::vector<std::string> splittedCommand);
	void manager(std::vector<std::string> splittedCommand);
	void longest_chain(std::vector<std::string> splittedCommand);
	void overloaded(std::vector<std::string> splittedCommand);
	void salary(std::vector<std::string> splittedCommand);
	void modernize(std::vector<std::string> splittedCommand);
	void incorporate(std::vector<std::string> splittedCommand);

	void save(std::string treeName, std::string fileName);
	void save(std::string treeName);
	void join(std::vector<std::string> splittedCommand);
	
	void loadLoader(std::vector<std::string> splittedCommand); 
	void saveLoader(std::vector<std::string> splittedCommand);

	void askEditedObjectsForSave();
	Hierarchy* getWantedObject(string objectName);
};