#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Student {
    string surname;
    string initials;
    int grades[5];
};

class TrieNode {
public:
    TrieNode* children[26];
    Student* data;
    TrieNode() {
        fill(begin(children), end(children), nullptr);
        data = nullptr;
    }
};

class Trie {
private:
    TrieNode* root;

    void deleteTrie(TrieNode* node) {
        if (node == nullptr) return;
        for (TrieNode* child : node->children) {
            deleteTrie(child);
        }
        delete node->data;
        delete node;
    }

public:
    Trie() {
        root = new TrieNode();
    }

    ~Trie() {
        deleteTrie(root);
    }

    void insert(const string& key, Student* data) {
        TrieNode* current = root;
        for (char c : key) {
            int index = c - 'a';
            if (!current->children[index]) {
                current->children[index] = new TrieNode();
            }
            current = current->children[index];
        }
        current->data = data;
    }

    void remove(const string& key) {
        TrieNode* current = root;
        TrieNode* prev = nullptr;
        int prevIndex = -1;
        for (char c : key) {
            int index = c - 'a';
            if (!current->children[index]) {
                return; // Key not found
            }
            prev = current;
            prevIndex = index;
            current = current->children[index];
        }
        if (current && current->data) {
            delete current->data;
            current->data = nullptr;
        }
        // Optionally: Remove nodes if they are no longer needed
        if (prev && current) {
            delete prev->children[prevIndex];
            prev->children[prevIndex] = nullptr;
        }
    }

    Student* search(const string& key) {
        TrieNode* current = root;
        for (char c : key) {
            int index = c - 'a';
            if (!current->children[index]) {
                return nullptr;
            }
            current = current->children[index];
        }
        return current->data;
    }

    void display(TrieNode* node, const string& path) {
        if (!node) return;
        if (node->data) {
            cout << "Surname: " << node->data->surname << endl;
            cout << "Initials: " << node->data->initials << endl;
            cout << "Grades: ";
            for (int i = 0; i < 5; ++i) {
                cout << node->data->grades[i] << " ";
            }
            cout << endl << "---------------------------" << endl;
        }
        for (int i = 0; i < 26; ++i) {
            if (node->children[i]) {
                display(node->children[i], path + char('a' + i));
            }
        }
    }

    void display() {
        display(root, "");
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
    cout << "7. Display students with any 2s (excluding those with multiple 2s)" << endl;
    cout << "8. Exit" << endl;
}

void enterData(Trie& allStudentsTrie, Trie& allFivesTrie, Trie& anyThreesTrie, Trie& anyTwosTrie) {
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
    Student* newData = new Student{ surname, initials, {grades[0], grades[1], grades[2], grades[3], grades[4]} };
    allStudentsTrie.insert(key, newData);

    if (all_of(begin(grades), end(grades), [](int g) { return g == 5; })) {
        allFivesTrie.insert(key, newData);
    }

    if (any_of(begin(grades), end(grades), [](int g) { return g == 3; })) {
        anyThreesTrie.insert(key, newData);
    }


    if (any_of(begin(grades), end(grades), [](int g) { return g == 2; })) {
        if (count(begin(grades), end(grades), 2) == 1) {
            anyTwosTrie.insert(key, newData);
        }
        else {
            allStudentsTrie.remove(key);
            cout << "Student " << surname << " " << initials << " has more than one 2 and is excluded from the main list." << endl;
            return;
        }
    }

    cout << "Data entered successfully." << endl;
}

void removeData(Trie& allStudentsTrie, Trie& allFivesTrie, Trie& anyThreesTrie, Trie& anyTwosTrie) {
    string surname, initials;
    cout << "Enter surname: ";
    cin >> surname;
    cout << "Enter initials: ";
    cin >> initials;
    string key = surname + initials;

    allStudentsTrie.remove(key);
    allFivesTrie.remove(key);
    anyThreesTrie.remove(key);
    anyTwosTrie.remove(key);

    cout << "Data removed successfully." << endl;
}

void updateData(Trie& allStudentsTrie, Trie& allFivesTrie, Trie& anyThreesTrie, Trie& anyTwosTrie) {
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
    Student* newData = new Student{ surname, initials, {grades[0], grades[1], grades[2], grades[3], grades[4]} };

    // Remove the old data from all tries
    removeData(allStudentsTrie, allFivesTrie, anyThreesTrie, anyTwosTrie);

    // Insert the new data
    allStudentsTrie.insert(key, newData);

    if (all_of(begin(grades), end(grades), [](int g) { return g == 5; })) {
        allFivesTrie.insert(key, newData);
    }

    if (any_of(begin(grades), end(grades), [](int g) { return g == 3; })) {
        anyThreesTrie.insert(key, newData);
    }

    if (any_of(begin(grades), end(grades), [](int g) { return g == 2; })) {
        if (count(begin(grades), end(grades), 2) == 1) {
            anyTwosTrie.insert(key, newData);
        }
        else {
            allStudentsTrie.remove(key);
            cout << "Student " << surname << " " << initials << " has more than one 2 and is excluded from the main list." << endl;
            return;
        }
    }

    cout << "Data updated successfully." << endl;
}

int main() {
    Trie allStudentsTrie, allFivesTrie, anyThreesTrie, anyTwosTrie;
    bool exitProgram = false;

    while (!exitProgram) {
        displayMenu();
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                enterData(allStudentsTrie, allFivesTrie, anyThreesTrie, anyTwosTrie);
                break;
            case 2:
                removeData(allStudentsTrie, allFivesTrie, anyThreesTrie, anyTwosTrie);
                break;
            case 3:
                updateData(allStudentsTrie, allFivesTrie, anyThreesTrie, anyTwosTrie);
                break;
            case 4:
                cout << "All Students:" << endl;
                allStudentsTrie.display();
                break;
            case 5:
                cout << "Students with all 5s:" << endl;
                allFivesTrie.display();
                break;
            case 6:
                cout << "Students with any 3s:" << endl;
                anyThreesTrie.display();
                break;
            case 7:
                cout << "Students with any 2s (excluding those with multiple 2s):" << endl;
                anyTwosTrie.display();
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
