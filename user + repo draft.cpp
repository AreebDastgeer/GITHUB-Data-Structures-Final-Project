#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;


class HashNode {
public:
	string username;
	string password;
	string email;
	bool loggedIn;
	HashNode* next;

	HashNode(string username, string password, string email) : username(username), password(password), email(email), loggedIn(false), next(nullptr) {}
};

class HashTable {
private:
	static const int TABLE_SIZE = 100;
	HashNode** table;

public:
	HashTable() {
		table = new HashNode*[TABLE_SIZE]();
		loadDataFromCSV("users.csv");
	}

	~HashTable() {
		for (int i = 0; i < TABLE_SIZE; ++i) {
			HashNode* entry = table[i];
			while (entry != nullptr) {
				HashNode* prev = entry;
				entry = entry->next;
				delete prev;
			}
		}
		delete[] table;
	}

	void loadDataFromCSV(const string& filename) {
		cout << "Loading file " << filename << ".... " << endl;
		ifstream inFile(filename);
		if (!inFile.is_open()) {
			cout << "Unable to open file '" << filename << endl;
			return;
		}

		string line;
		while (getline(inFile, line)) {
			stringstream ss(line);
			string username, password, email;
			getline(ss, username, ',');
			getline(ss, password, ',');
			getline(ss, email, ',');
			addUser(username, password, email);
		}

		inFile.close();
		cout << "Data loaded from CSV file '" << filename << "'." << endl;
	}

	int hashFunction(string key) {
		int hash = 0;
		for (char c : key) {
			hash = (hash * 31 + c) % TABLE_SIZE;
		}
		return hash;
	}

	void addUser(string username, string password, string email) {
		int index = hashFunction(username);
		if (table[index] == nullptr) {
			table[index] = new HashNode(username, password, email);
		}
		else {
			HashNode* entry = table[index];
			while (entry->next != nullptr) {
				entry = entry->next;
			}
			entry->next = new HashNode(username, password, email);
		}
	}

	HashNode* getUserNode(string username) {
		int index = hashFunction(username);
		HashNode* entry = table[index];
		while (entry != nullptr) {
			if (entry->username == username) {
				return entry;
			}
			entry = entry->next;
		}
		return nullptr;
	}

	bool isUsernameTaken(string username) {
		return getUserNode(username) != nullptr;
	}

	bool isEmailTaken(string email) {
		for (int i = 0; i < TABLE_SIZE; ++i) {
			HashNode* entry = table[i];
			while (entry != nullptr) {
				if (entry->email == email) {
					return true;
				}
				entry = entry->next;
			}
		}
		return false;
	}
	int getTableSize()
	{
		return TABLE_SIZE;
	}
	HashNode* getTable(int i)
	{
		return table[i];
	}
};

class User {
public:

	// CSV 
	static void writeToCSV(string username, string password, string email) {
		ofstream outFile("users.csv", ios::app);
		if (outFile.is_open()) {
			outFile << username << "," << password << "," << email << endl;
			outFile.close();
			cout << "User information saved to CSV file." << endl;
		}
		else {
			cout << "Error: Unable to open file for writing." << endl;
		}
	}

	// Registration
	static void registerUser(HashTable& hashTable, string username, string password, string email) {
		if (hashTable.isUsernameTaken(username)) {
			cout << "Username '" << username << "' is already taken. Please choose a different username." << endl;
			return;
		}
		if (hashTable.isEmailTaken(email)) {
			cout << "Email '" << email << "' is already registered. Please use a different email address." << endl;
			return;
		}
		hashTable.addUser(username, password, email);
		writeToCSV(username, password, email);
		cout << "User registered successfully!" << endl;
	}

	// Login
	static bool loginUser(HashTable& hashTable, string username, string password) {
		HashNode* userNode = hashTable.getUserNode(username);
		if (userNode != nullptr && userNode->password == password) {
			if (userNode->loggedIn) {
				cout << "User '" << username << "' is already logged in." << endl;
				return false;
			}
			userNode->loggedIn = true;
			cout << "Login successful. Welcome, " << username << "!" << endl;
			return true;
		}
		else {
			cout << "Invalid username or password." << endl;
			return false;
		}
	}

	// Logout
	static void logoutUser(HashTable& hashTable, string username) {
		HashNode* userNode = hashTable.getUserNode(username);
		if (userNode != nullptr && userNode->loggedIn) {
			userNode->loggedIn = false;
			cout << "Logout successful. Goodbye, " << username << "!" << endl;
		}
		else {
			cout << "User '" << username << "' is not logged in." << endl;
		}
	}

	// Profile View
	static void viewProfile(HashTable& hashTable, string username) {
		HashNode* userNode = hashTable.getUserNode(username);
		if (userNode != nullptr) {
			cout << "Username: " << userNode->username << endl;
			cout << "Email: " << userNode->email << endl;
		}
		else {
			cout << "User not found." << endl;
		}
	}
};



class LinkedListNode {
public:
	string id;
	string message;
	string timestamp;
	string content;
	LinkedListNode* next;

	LinkedListNode(string id, string message,string time, string content) : id(id), message(message), timestamp(time), content(content),next(nullptr) {}
};

class LinkedList {
private:
	LinkedListNode* head;

public:
	LinkedList() : head(nullptr) {}

	~LinkedList() {
		LinkedListNode* current = head;
		while (current != nullptr) {
			LinkedListNode* next = current->next;
			delete current;
			current = next;
		}
	}

	void addNode(string id, string message,string time,string content) {
		LinkedListNode* newNode = new LinkedListNode(id, message,time,content);
		newNode->next = head;
		head = newNode;
	}

	void modifyNode(string id, string newMessage,string newContent) {
		LinkedListNode* current = head;
		while (current != nullptr) {
			if (current->id == id) {
				current->message = newMessage;
				current->content = newContent;
				return;
			}
			current = current->next;
		}
		cout << "Node with ID " << id << " not found." << endl;
	}

	void displayNodes() {
		LinkedListNode* current = head;
		while (current != nullptr) {
			cout << "ID: " << current->id << ", Message: " << current->message << ", Timestamp: " << current->timestamp  << ", Content: " << current->content << endl;
			current = current->next;
		}
	}
	LinkedListNode* getHead()
	{
		return head;
	}

};

class RepoNode {
public:
	string repoName;
	string username;
	LinkedList* commitList;
	LinkedList* fileList;
	RepoNode* left;
	RepoNode* right;

	RepoNode(string name, string user) : repoName(name), commitList(new LinkedList()), fileList(new LinkedList()), username(user), left(nullptr), right(nullptr) {}
};

class Repository {
private:
	RepoNode* root;
	HashTable& hashTable;

	void destroyTree(RepoNode* node) {
		if (node != nullptr) {
			destroyTree(node->left);
			destroyTree(node->right);
			delete node;
		}
	}

public:
	Repository()
	{
		root = nullptr;
		loadRepositoriesForAllUsers(hashTable);
	}

	~Repository() {
		destroyTree(root);
	}

	void loadRepositoriesFromCSV(string username) {
		string filename = username + ".csv";
		ifstream file(filename);
		if (file.is_open()) {
			string line;
			while (getline(file, line)) {
				stringstream ss(line);
				string repoName;
				getline(ss, repoName, ',');
				// Add the repository to the user's repository tree
				addRepository(repoName, username);
			}
			file.close();
		}
		else {
			cout << "Error: Unable to open file " << filename << endl;
		}
	}

	void loadRepositoriesForAllUsers(HashTable& hashTable) {
		for (int i = 0; i < hashTable.getTableSize(); ++i) {
			HashNode* entry = hashTable.getTable(i);
			while (entry != nullptr) {
				loadRepositoriesFromCSV(entry->username);
				entry = entry->next;
			}
		}
	}

	// Function to add a repository for a specific user
	void addRepository(string repoName, string username) {
		if (root == nullptr) {
			root = new RepoNode(repoName, username);
		}
		else {
			addRepositoryRecurr(root, repoName, username);
		}
	}

	//Recuurrsive function
	void addRepositoryRecurr(RepoNode* node, string repoName, string username) {
		if (repoName < node->repoName) {
			if (node->left == nullptr) {
				node->left = new RepoNode(repoName, username);
			}
			else {
				addRepositoryRecurr(node->left, repoName, username);
			}
		}
		else if (repoName > node->repoName) {
			if (node->right == nullptr) {
				node->right = new RepoNode(repoName, username);
			}
			else {
				addRepositoryRecurr(node->right, repoName, username);
			}
			saveRepositoryToCSV(repoName, username);
		}
		ofstream userFile(username + ".csv", ios::app);
		if (userFile.is_open()) {
			// Write the repository information to the file
			userFile << repoName << "," << getCommitListAsString(root) << "," << getFileListAsString(root) << endl;
			userFile.close(); // Close the file
		}
		else {
			cout << "Error: Unable to open file for writing." << endl;
		}
	}
	void saveRepositoryToCSV(string repoName, string username) {
		ofstream outFile("repositories.csv", ios::app);
		if (outFile.is_open()) {
			// Construct the line to write to the file
			string line = username + "," + repoName + "," + getCommitListAsString(root) + "," + getFileListAsString(root);
			outFile << line << endl;
			outFile.close();
			cout << "Repository information saved to CSV file." << endl;
		}
		else {
			cout << "Error: Unable to open file for writing." << endl;
		}
	}
	string getCommitListAsString(RepoNode* node) {
		string commitListString;
		LinkedListNode* current = node->commitList->getHead();
		while (current != nullptr) {
			// Append the commit ID to the string
			commitListString += current->id;
			// Move to the next commit node
			current = current->next;
			// If there are more commits, add a comma separator
			if (current != nullptr) {
				commitListString += ",";
			}
		}
		return commitListString;
	}
	string getFileListAsString(RepoNode* node) {
		string fileListString;
		LinkedListNode* current = node->fileList->getHead();
		while (current != nullptr) {
			// Append the file name to the string
			fileListString += current->id;
			// Move to the next file node
			current = current->next;
			// If there are more files, add a comma separator
			if (current != nullptr) {
				fileListString += ",";
			}
		}
		return fileListString;
	}

	// Function to display repositories for a specific user
	void displayRepositories(string username) {
		displayRepositoriesRecurr(root, username);
	}

	// Recursive function to display repositories for a specific user
	void displayRepositoriesRecurr(RepoNode* node, string username) {
		if (node != nullptr) {
			displayRepositoriesRecurr(node->left, username);
			if (node->username == username) {
				cout << node->repoName << endl;
			}
			displayRepositoriesRecurr(node->right, username);
		}
	}

	void addCommit(string id, string message, string timestamp, string content) {
		root->commitList->addNode(id, message, timestamp, content);
	}

	
	void modifyCommit(string id, string newMessage, string newContent) {
		root->commitList->modifyNode(id, newMessage, newContent);
	}

	
	void addFile(string id, string message, string timestamp, string content) {
		root->fileList->addNode(id, message, timestamp, content);
	}

	void modifyFile(string id, string newMessage, string newContent) {
		root->fileList->modifyNode(id, newMessage, newContent);
	}

	//// Function to search for a repository by name within a user's repository tree
	//RepoNode* searchRepositoryInTree(RepoNode* root, string Name) {
	//	if (root == nullptr) {
	//		return nullptr;
	//	}
	//	if (root->repoName == Name) {
	//		return root;
	//	}
	//	RepoNode* leftResult = searchRepositoryInTree(root->left, Name);
	//	RepoNode* rightResult = searchRepositoryInTree(root->right, Name);
	//	return (leftResult != nullptr) ? leftResult : rightResult;
	//}

	// Function to search for repositories in "repositories.csv"
	void searchRepositories(string searchQuery) {
		ifstream repoFile("repositories.csv");
		if (repoFile.is_open()) {
			string line;
			while (getline(repoFile, line)) {
				stringstream ss(line);
				string repoName, username;
				getline(ss, repoName, ',');
				getline(ss, username, ',');

				// Check if the repository name matches the search query
				if (repoName.find(searchQuery) != string::npos) {
					cout << "Repository: " << repoName << ", Username: " << username << endl;
				}
			}
			repoFile.close();
		}
		else {
			cout << "Error: Unable to open repositories file." << endl;
		}
	}

};
int main() {
	

	system("pause");
	return 0;
}