#pragma once
#include <iostream>
#include <vector>
#include <string>
#include<queue>
#include <stdexcept>

using std::string;

struct Pair
{
	string manager;
	string worker;
	Pair(const string& manager, const string& worker) : manager(manager), worker(worker) {};
	friend bool operator>(const Pair& first, const Pair& second) {
		if (first.manager != second.manager)
			return (first.manager > second.manager);

		return (first.worker > second.worker);
	};
	friend std::ostream& operator<<(std::ostream& os, const Pair& obj)
	{
		os << obj.manager << "-" << obj.worker << "\n";
		return os;
	}
	
	string toString() {
		string result = manager;
		result.append("-").append(worker).append("\n");
		return result;
	}
};

struct Node {
	
	string name;
	Node* child;
	Node* brother;
	Node(const string& data, Node* child = nullptr, Node* brother = nullptr) : name(data), child(child), brother(brother) {};
	Node(const Node& other) : name(other.name), child(nullptr), brother(nullptr) {};
};


class Hierarchy {
	friend class TaskManager;
private:
	bool isModified;

	string name;
	Node* root;
	size_t size;

	void clear(Node* root) {

		if (root)
		{
			clear(root->child);
			clear(root->brother);
			delete root;
		}

	}
	Node* copy(Node* root) {

		if (root)
			return new Node(root->name, copy(root->child), copy(root->brother));
		else
			return root;
	}

public:

	string getName() { return name; };

	Hierarchy(const string& data) : Hierarchy()
	{
		int numberOfPairs = 0;
		int numberOfDashes = 0;

		if (data.size() == 0)
		{
			return;
		}
		addTheBoss();

		int j = 0;
		for (size_t i = 0, counter = 1; i < data.size(); i++, counter++)
		{
			if (data[i] == '\n')
			{
				numberOfPairs++;

				string input(data, j, counter - 1);
				counter = 0;
				hireFromFile(input);
				j = i + 1;
			
			}
			else if (data[i] == '-')
			{
				numberOfDashes++;
			}

		}

		if (numberOfPairs == 0)
		{
			throw std::exception("Invalid data!");
		}
		if (numberOfPairs != numberOfDashes)
		{
			throw std::exception("Invalid data!");
		}
	  
	};
	
	Hierarchy() : isModified(false), size(0), root(nullptr) {};
	Hierarchy(const Hierarchy& other) : root(copy(other.root)), size(other.size) {};
	Hierarchy(Hierarchy&& Hierarchy) noexcept : Hierarchy()
	{
		std::swap(root, Hierarchy.root);
		std::swap(size, Hierarchy.size);
	};
	void operator=(const Hierarchy&) = delete;
	
	~Hierarchy() noexcept { clear(root); };
	void setName(string name) {
		this->name = name;
	}

	string print() const {

		std::vector<Pair*> pairs = byLevels(root);

		string result("");

		for (size_t i = 0; i < pairs.size(); i++)
		{
			result.append(pairs[i]->toString());

			delete pairs[i];
		}
		return result;
	};


	int longest_chain() const { return longest_chain(root); }
	bool find(const string& name) const  {
		return findEmployee(root, name);
	};
	int num_employees() const { return size; }
	int num_overloaded(int level = 20) const { return num_overloaded(root, level); }

	string manager(const string& name) const {
		if (name == "Uspeshnia")
		{
			return "";
		}
		const Node* manager = getParent(root,name);
		if (manager)
		{
			return manager->name;
		}

		return "INVALID_MANAGER";
	}
	int num_subordinates(const string& name) const  {
		const Node* parent = findEmployee(root, name);

		return numberOfChildren(parent);
	}
	unsigned long getSalary(const string& who) const
	{
		const Node* employee = findEmployee(root, who);
		if (!employee)
		{
			return -1;
		}
		return getSalary(employee);
	}

	bool fire(const string& who) {
		if (who == "Uspeshnia") return false;
		return fire(root, who);
	}
	bool hire(const string& who, const string& boss) 
	{
		if (who == "Uspeshnia") return false;
		
		Node* director = findEmployee(root, boss);
		Node* employee = findEmployee(root, who);

		if (!director)
		{
			return false;
		}

		if (employee)
		{
			return rehire(root, who, director);
		}
		else
		{
			Node* it = director;
			if (it)
			{
				if (!it->child)
				{
					it->child = new Node(who);
				}
				else
				{
					it = it->child;

					while (it->brother != nullptr)
					{
						it = it->brother;
					}
					it->brother = new Node(who);
				}
				size++;
				return true;
			}
			else
			{
				std::cout << "Invalid Director!\n";
				return false;
			}
		}
	}
	

	void modernize() { modernize(root, 0, nullptr); }
	void incorporate() { incorporate(root); };

	Hierarchy join(const Hierarchy& right) const { 

		Hierarchy resultTree(*this);
		resultTree.myJoin(right.root->child, right.root);

		return resultTree;
	};
	
	
private:
	void addTheBoss()
	{
		root = new Node("Uspeshnia");
		size++;
	};
	void myJoin(Node* rightRoot,  Node* root)
	{
		if (!rightRoot)
			return;

		Node* employee = findEmployee(this->root, rightRoot->name);

		if (employee)
		{
			Node* thisTreeManager = getParent(this->root, employee->name);
			Node* otherTreeManager = getParent(root, rightRoot->name);

			if (thisTreeManager->name == otherTreeManager->name)
			{
				Node* it = rightRoot;
				while (it)
				{
					hire(it->name, thisTreeManager->name);
					it = it->brother;
				}
			}
			else
			{
				int thisManagerLevel = getLevel(this->root, thisTreeManager->name, 0);
				int otherManagerLevel = getLevel(root, otherTreeManager->name, 0);

				if (thisManagerLevel < otherManagerLevel)
				{
						hire(employee->name, thisTreeManager->name);
						
				}
				else if(thisManagerLevel > otherManagerLevel) 
				{
					Node* parent = findEmployee(this->root, otherTreeManager->name);
					Node* toHire = new Node(*rightRoot);
					fire(this->root, rightRoot->name);
					hire(toHire,parent);
					this->size++;
						
				}
				else
				{
					if (thisTreeManager->name < otherTreeManager->name)
					{
						hire(employee->name, thisTreeManager->name);
					}
					else
					{
						Node* parent = findEmployee(this->root, otherTreeManager->name);
						Node* toHire = new Node(*rightRoot);
						fire(this->root, rightRoot->name);
						hire(toHire, parent);
						this->size++;
					}
				}

			}
		}
		else
		{
			hire(rightRoot->name, getParent(root, rightRoot->name)->name);
		}

		myJoin(rightRoot->child, root);
		myJoin(rightRoot->brother,  root);
	}

	void modernize(Node*& root, int counter, Node* parent) {
		if (!root)
			return;

		modernize(root->child, counter + 1, root);
		modernize(root->brother, counter, parent);

		if (counter % 2 != 0)
		{
			if (root->child)
			{
				rehire(this->root, root->child->name, parent);
				fire(root);
			}
		}

	}

	void incorporate(Node*& root) {
		if (!root)
			return;

		incorporate(root->child);
		incorporate(root->brother);

		if (root->child)
		{
			if (root->child->brother)
			{
				Node* nextDirector = root->child;
				Node* it = nextDirector->brother;
				unsigned long currentSalary;
				unsigned long biggestSalary = getSalary(nextDirector);

				while (it)
				{
					currentSalary = getSalary(it);
				
					if (currentSalary > biggestSalary)
					{
						nextDirector = it;
						biggestSalary = currentSalary;
					}
					else if (currentSalary == biggestSalary)
					{
						if (it->name < nextDirector->name)
						{
							nextDirector = it;
						}
					}
					it = it->brother;
				}

				Node* toRehire = root->child;
				Node* next = toRehire->brother;

				while (toRehire)
				{
					if (toRehire == nextDirector )
					{
						toRehire = next;
						if (next) next = next->brother;
					}
					else
					{
						rehire(this->root, toRehire->name, nextDirector);
						toRehire = next;
						if (next) next = next->brother;
					}
				}
				
			}
		}

	}

	unsigned long getSalary(const Node* employee) const {
		int directEmployeeSubordinates = numberOfChildren(employee);
		int inDirectEmployeeSuboordinates = numberOfSuccessors(employee->child) - directEmployeeSubordinates;

		return (500 * directEmployeeSubordinates + 50 * inDirectEmployeeSuboordinates);
	};

	void hireFromFile(string& input) {
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
				firstName[j++] = input[i];
				i++;
				counter++;
			}
		}

		i++;
		std::string director(firstName, counter);
		j = 0;
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

		if (!hire(worker, director))
		{

			throw std::exception("Invalid Director!");
		}
	}
	bool rehire(Node*& root, const string& who, Node*& boss) {
		if (!root)
			return false;

		if (root->brother && root->brother->name == who)
		{
			Node* toRehire = root->brother;
			root->brother = root->brother->brother;

			toRehire->brother = nullptr;
			return hire(toRehire, boss);
		}
		else if (root->child && root->child->name == who)
		{
			Node* toRehire = root->child;
			root->child = root->child->brother;

			toRehire->brother = nullptr;
			return hire(toRehire, boss);

		}
		else
		{
			return rehire(root->brother, who, boss) || rehire(root->child, who, boss);
		}
	}
	void rehireEntireTeam(Node*& oldBoss, Node*& newBoss) {
		Node* team = oldBoss->child;
		oldBoss->child = nullptr;
		Node* it = newBoss->child;
		while (it->brother)
		{
			it = it->brother;
		}
		it->brother = team;
	}
	bool hire(Node*& employee, Node*& boss) {
		
		Node* it = boss;
		if (it)
		{
			if (!it->child)
			{
				it->child = employee;
			}
			else
			{
				it = it->child;

				while (it->brother != nullptr)
				{
					it = it->brother;
				}
				it->brother = employee;
			}
			
			return true;
		}
		else
		{
			std::cout << "Invalid Director!\n";
			return false;
		}
	}

	bool fire(Node*& root) {
		Node* toDelete = root;
		if (!root->child)
		{
			root = root->brother;
		}
		else if (root->brother)
		{
			Node* it = root;

			while (it->brother)
			{
				it = it->brother;
			}
			it->brother = root->child;
			root = root->brother;
		}
		else
		{
			root = root->child;
		}
		--size;
		delete toDelete;
		return true;
	}
	bool fire(Node*& root, const string& name)
	{
		if (!root)
			return false;

		if (root->name == name)
		{
			Node* toDelete = root;
			if (!root->child)
			{
				root = root->brother;
			}
			else if (root->brother)
			{
				Node* it = root;

				while (it->brother)
				{
					it = it->brother;
				}
				it->brother = root->child;
				root = root->brother;
			}
			else
			{
				root = root->child;
			}
			--size;
			delete toDelete;
			return true;
		}
		else
		{
			return fire(root->brother, name) || fire(root->child, name);
		}
	};

	const Node* findEmployee(Node* root, const string& name) const {
		if (root) {
			if (root->name == name)
				return root;
			else
			{
				const Node* foundNode = findEmployee(root->brother, name);
				if (!foundNode)
				{
					foundNode = findEmployee(root->child, name);
				}
				return foundNode;
			}
		}
		else
			return nullptr;
	}
	Node* findEmployee(Node* root, const string& name) {
		if (root) {
			if (root->name == name)
				return root;
			else
			{
				Node* foundNode = findEmployee(root->brother, name);
				if (!foundNode)
				{
					foundNode = findEmployee(root->child, name);
				}
				return foundNode;
			}
		}
		else
			return nullptr;
	}

	const Node* getParent(Node* root, const string& name) const {
		if (root) {
			if (isChild(root, name))
				return root;
			else
			{
				const Node* parent = getParent(root->brother, name);
				if (!parent)
				{
					parent = getParent(root->child, name);
				}
				return parent;
			}
		}
		else
			return nullptr;
	}
	Node* getParent(Node* root, const string& name){
		if (root) {
			if (isChild(root, name))
				return root;
			else
			{
				Node* parent = getParent(root->brother, name);
				if (!parent)
				{
					parent = getParent(root->child, name);
				}
				return parent;
			}
		}
		else
			return nullptr;
	}

	bool isChild(const Node* parent, const string& name) const {
		Node* it = parent->child;
		while (it)
		{
			if (it->name == name)
				return true;
			it = it->brother;
		}
		return false;
	}
	int numberOfChildren(const Node* parent) const {
		if (parent)
		{
			size_t counter = 0;
			Node* it = parent->child;

			while (it)
			{
				counter++;
				it = it->brother;
			}
			return counter;
		}
		return -1;
	}
	int longest_chain(const Node* root) const {
		if (!root)
			return 0;

		return std::max(1 + longest_chain(root->child), longest_chain(root->brother));
	};
	int numberOfSuccessors(Node* successor) const {
		if (!successor)
			return 0;

		return 1 + numberOfSuccessors(successor->child) + numberOfSuccessors(successor->brother);
	}
	int num_overloaded(Node* root, int num)const {
		if (!root)
			return 0;
		return (numberOfSuccessors(root->child) > num) + num_overloaded(root->child, num) + num_overloaded(root->brother, num);
	}

	std::vector<Pair*> byLevels(const Node* root) const {


		std::vector<Pair*> printInfo;
		if (!root) return printInfo;
		std::vector<Pair*> sorter;
		std::queue<const Node*> front;
		front.push(root);
		front.push(nullptr);

		for (;;) {
			const Node* current = front.front();
			front.pop();
			if (!current)
			{
				if (front.empty())
				{
					return printInfo;
				}
				front.push(nullptr);

				for (int i = 0; i < sorter.size(); i++)
				{
					for (int j = 0; j < sorter.size() - 1; j++)
					{
						if (*(sorter[j]) > *(sorter[j + 1]))
							std::swap(sorter[j], sorter[j + 1]);
					}
				}


				for (size_t i = 0; i < sorter.size(); i++)
				{
					printInfo.push_back(sorter[i]);
				}
				sorter.clear();
			}
			else
			{

				for (const Node* it = current->child; it; it = it->brother) {
					Pair* pair = new Pair(current->name, it->name);

					sorter.push_back(pair);

					front.push(it);

				}


			}
		}
	};
	int getLevel(const Node* root, string name, int counter) {
		if (!root) return -1;
		std::queue<const Node*> front;
		front.push(root);
		front.push(nullptr);
		for (;;) {
			const Node* current = front.front();
			front.pop();
			if (!current) {
				if (front.empty()) return -1;
				front.push(nullptr);
				counter++;
			}
			else {
				if (current->name == name) return counter;

				for (const Node* it = current->child; it; it = it->brother) {
					front.push(it);
				}
			}
		}
	}
};



/*Hierarchy& operator=(const Hierarchy& other) {
		if (this != &other)
		{
			clear(root);
			root = copy(other.root);
			size = other.size;
		}
		return *this;
	}
	Hierarchy& operator=(Hierarchy&& other) noexcept {
		if (this != &other)
		{
			std::swap(root, other.root);
			std::swap(size, other.size);

		}
		return *this;
	}*/