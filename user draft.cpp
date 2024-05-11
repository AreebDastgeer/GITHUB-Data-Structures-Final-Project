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
		 cout << "Loading file " << filename << ".... "<< endl;
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

int main() {
	HashTable hashTable;

	// Register users
	User::registerUser(hashTable, "areeb", "password1", "areeb@example.com");
	User::registerUser(hashTable, "user2", "password2", "user2@example.com");
	User::registerUser(hashTable, "user3", "password3", "user3@example.com");

	// Login
	User::loginUser(hashTable, "areeb", "password1");
	User::loginUser(hashTable, "user2", "wrongpassword"); // Invalid login
	User::loginUser(hashTable, "areeb", "password1");

	//Logout
	User::logoutUser(hashTable, "areeb");

    // View profile
	User::viewProfile(hashTable, "areeb");
	User::viewProfile(hashTable, "user4"); // User not found

	system("pause");
	return 0;
}
