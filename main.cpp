#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>

using namespace std;

struct Student {
    string surname;
    string initials;
    int grades[5];
};

class HashTable {
private:
    static const int TABLE_SIZE = 256; // Розмір хеш-таблиці
    list<pair<string, shared_ptr<Student>>> table[TABLE_SIZE]; // Масив списків для обробки колізій

    // Хеш-функція
    int hashFunction(const string& key) {
        int hash = 0;
        for (char c : key) {
            hash = (hash + c) % TABLE_SIZE;
        }
        return hash;
    }

public:
    void insert(const string& key, shared_ptr<Student> data) {
        int hash = hashFunction(key);
        for (auto& pair : table[hash]) {
            if (pair.first == key) {
                pair.second = data; // Оновлення значення, якщо ключ вже існує
                return;
            }
        }
        table[hash].emplace_back(key, data); // Додавання нової пари ключ-значення
    }

    void remove(const string& key) {
        int hash = hashFunction(key);
        for (auto it = table[hash].begin(); it != table[hash].end(); ++it) {
            if (it->first == key) {
                table[hash].erase(it); // Видалення елемента зі списку
                return;
            }
        }
        cout << "Student not found!" << endl;
    }

    shared_ptr<Student> search(const string& key) {
        int hash = hashFunction(key);
        for (const auto& pair : table[hash]) {
            if (pair.first == key) {
                return pair.second;
            }
        }
        return nullptr;
    }

    void display() {
        bool found = false;
        for (int i = 0; i < TABLE_SIZE; ++i) {
            for (const auto& pair : table[i]) {
                found = true;
                cout << "Surname: " << pair.second->surname << endl;
                cout << "Initials: " << pair.second->initials << endl;
                cout << "Grades: ";
                for (int j = 0; j < 5; ++j) {
                    cout << pair.second->grades[j] << " ";
                }
                cout << endl << "---------------------------" << endl;
            }
        }
        if (!found) {
            cout << "No students found." << endl;
        }
    }
};

void displayMenu() {
    cout << "Menu:" << endl;
    cout << "1. Enter data" << endl;
    cout << "2. Remove data" << endl;
    cout << "3. Update data" << endl;
    cout << "4. Display all students" << endl;
    cout << "5. Display students with all 5s" << endl;
    cout << "6. Display students with any 3s" << endl;
    cout << "7. Display students with any 2s (including those with multiple 2s)" << endl;
    cout << "8. Exit" << endl;
}

void insertStudent(HashTable& table, const string& key, shared_ptr<Student> newData) {
    table.insert(key, newData);
}

void removeStudent(HashTable& table, const string& key) {
    table.remove(key);
}

void updateLists(HashTable& allStudentsTable, HashTable& allFivesTable, HashTable& anyThreesTable, HashTable& anyTwosTable, const string& key, shared_ptr<Student> newData) {
    int countTwos = count(begin(newData->grades), end(newData->grades), 2);
    int countThrees = count(begin(newData->grades), end(newData->grades), 3);
    int countFives = count(begin(newData->grades), end(newData->grades), 5);

    if (countTwos > 1) {
        removeStudent(allStudentsTable, key);
    } else {
        insertStudent(allStudentsTable, key, newData);
    }

    if (countFives == 5) {
        insertStudent(allFivesTable, key, newData);
    } else {
        removeStudent(allFivesTable, key);
    }

    if (countThrees > 0) {
        insertStudent(anyThreesTable, key, newData);
    } else {
        removeStudent(anyThreesTable, key);
    }

    if (countTwos > 0) {
        insertStudent(anyTwosTable, key, newData);
    } else {
        removeStudent(anyTwosTable, key);
    }
}

void enterData(HashTable& allStudentsTable, HashTable& allFivesTable, HashTable& anyThreesTable, HashTable& anyTwosTable) {
    string surname, initials;
    int grades[5];

    cout << "Enter surname: ";
    cin >> surname;
    cout << "Enter initials: ";
    cin >> initials;
    cout << "Enter grades for 5 subjects: ";
    for (int i = 0; i < 5; ++i) {
        cin >> grades[i];
    }

    string key = surname + initials;
    shared_ptr<Student> newData = make_shared<Student>(Student{ surname, initials, {grades[0], grades[1], grades[2], grades[3], grades[4]} });

    updateLists(allStudentsTable, allFivesTable, anyThreesTable, anyTwosTable, key, newData);

    cout << "Data entered successfully." << endl;
}

void removeData(HashTable& allStudentsTable, HashTable& allFivesTable, HashTable& anyThreesTable, HashTable& anyTwosTable) {
    string surname, initials;
    cout << "Enter surname: ";
    cin >> surname;
    cout << "Enter initials: ";
    cin >> initials;
    string key = surname + initials;

    removeStudent(allStudentsTable, key);
    removeStudent(allFivesTable, key);
    removeStudent(anyThreesTable, key);
    removeStudent(anyTwosTable, key);

    cout << "Data removed successfully." << endl;
}

void updateData(HashTable& allStudentsTable, HashTable& allFivesTable, HashTable& anyThreesTable, HashTable& anyTwosTable) {
    string surname, initials;
    int grades[5];

    cout << "Enter surname: ";
    cin >> surname;
    cout << "Enter initials: ";
    cin >> initials;
    cout << "Enter new grades for 5 subjects: ";
    for (int i = 0; i < 5; ++i) {
        cin >> grades[i];
    }

    string key = surname + initials;
    shared_ptr<Student> newData = make_shared<Student>(Student{ surname, initials, {grades[0], grades[1], grades[2], grades[3], grades[4]} });

    updateLists(allStudentsTable, allFivesTable, anyThreesTable, anyTwosTable, key, newData);

    cout << "Data updated successfully." << endl;
}

int main() {
    HashTable allStudentsTable, allFivesTable, anyThreesTable, anyTwosTable;
    bool exitProgram = false;

    while (!exitProgram) {
        displayMenu();
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                enterData(allStudentsTable, allFivesTable, anyThreesTable, anyTwosTable);
                break;
            case 2:
                removeData(allStudentsTable, allFivesTable, anyThreesTable, anyTwosTable);
                break;
            case 3:
                updateData(allStudentsTable, allFivesTable, anyThreesTable, anyTwosTable);
                break;
            case 4:
                cout << "All Students:" << endl;
                allStudentsTable.display();
                break;
            case 5:
                cout << "Students with all 5s:" << endl;
                allFivesTable.display();
                break;
            case 6:
                cout << "Students with any 3s:" << endl;
                anyThreesTable.display();
                break;
            case 7:
                cout << "Students with any 2s (including those with multiple 2s):" << endl;
                anyTwosTable.display();
                break;
            case 8:
                exitProgram = true;
                break;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 8." << endl;
        }
    }

    return 0;
}
