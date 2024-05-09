#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

struct List {
    string data;
    List* next;
    List() {
        next = nullptr;
    }
};

struct RepositoryNode {
    string repositoryName;
    List* commits;
    List* files;
    RepositoryNode* left;
    RepositoryNode* right;

    RepositoryNode() {
        left = nullptr;
        right = nullptr;
        commits = nullptr;
        files = nullptr;
    }
};

struct User {
    string username;
    string password;
    RepositoryNode* repositories;
    User* followers[50];
};

struct Node {
    User user;
    Node* next;
};

class HashTable {
private:
    static const int TABLE_SIZE = 100;
    Node* table[TABLE_SIZE];

public:
    HashTable() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            table[i] = nullptr;
        }

        loadDataFromFile("loginCredential.csv");
    }

    int hash(const string& key) {
        int hashValue = 0;
        int multiplier = 1;
        for (char c : key) {
            hashValue += c * multiplier;
            multiplier++;
        }
        return hashValue % TABLE_SIZE;
    }

    bool addUser(const string& username) {
        if (findUser(username)) {
            cout << "User already exists." << endl;
            return false;
        }

        int index = hash(username);

        Node* newNode = new Node();
        newNode->user.username = username;
        newNode->next = nullptr;

        if (table[index] == nullptr) {
            table[index] = newNode;
        } else {
            Node* temp = table[index];
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
        return true;
    }

    bool findUser(const string& username) {
        int index = hash(username);

        Node* temp = table[index];
        while (temp != nullptr) {
            if (temp->user.username == username) {
                return true;
            }
            temp = temp->next;
        }
        return false;
    }

    void storeDataToFile(const string& filename) {
        ofstream file(filename);
        if (!file) {
            cout << "Error: Unable to open file." << endl;
            return;
        }

        for (int i = 0; i < TABLE_SIZE; ++i) {
            Node* temp = table[i];
            while (temp != nullptr) {
                file << temp->user.username << endl;
                temp = temp->next;
            }
        }

        file.close();
        cout << "Data stored to file successfully." << endl;
    }

    void loadDataFromFile(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cout << "Error: Unable to open file." << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string username;
            getline(ss, username, ',');

            addUser(username);
        }

        file.close();
        cout << "Data loaded from file successfully." << endl;
    }

    // access user account by username
    User* accessUser(const string& username) {
        int index = hash(username);

        Node* temp = table[index];
        while (temp != nullptr) {
            if (temp->user.username == username) {
                return &temp->user;
            }
            temp = temp->next;
        }
        return nullptr;
    }
};

class Repositories : public HashTable {
private:
    RepositoryNode* node;

public:
    Repositories() {
        HashTable();
        node = nullptr;
    }

    void addRepository(const string& username, const string& repositoryName, const string& CommitedData) {
        string filename = username + "_repositories.csv";
        ofstream file(filename, ios::app);
        if (!file) {
            cout << "Error: Unable to open file." << endl;
            return;
        }

        file << repositoryName << ",";
        file.close();
    }

    void searchRepositories(const string& username) {
        string filename = username + "_repositories.csv";
        ifstream file(filename);
        if (!file) {
            cout << "No repositories found for user " << username << endl;
            return;
        }

        string repository;
        cout << "Repositories for user " << username << ":" << endl;
        while (getline(file, repository)) {
            cout << repository << endl;
        }
        file.close();
    }
};
class UserAcc : public Repositories{
    string username;
    string password;
    RepositoryNode* repositories;
    User* followers[50];
};
int main() {
    UserAcc U;
    string username, repositoryName;
    int choice;
    cout << "Welcome to GitLite" << endl;
    cout << "Choose an option:" << endl;
    cout<<"1. login"<<endl;
    cout<<"2. signup"<<endl;
    cout << "3. Add Repository" << endl;
    cout << "4. Search Repositories" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    string commit, password;
    switch (choice) {
        case 1:
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            U.accessUser(username);
            break;
        case 2:
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            U.addUser(username);
            break;
        case 3:
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter repository name: ";
            cin >> repositoryName;
            cout << "Enter the data for the commit: ";
            cin >> commit;
            U.addRepository(username, repositoryName,commit);
            cout << "Repository added successfully." << endl;
            break;
        case 4:
            cout << "Enter username: ";
            cin >> username;
            U.searchRepositories(username);
            break;
        default:
            cout << "Invalid choice." << endl;
    }

    return 0;
}
